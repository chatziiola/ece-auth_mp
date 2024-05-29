#include <stdio.h>
#include "gpio.h"
#include "platform.h"
#include "queue.h"
#include "uart.h"
#include "stm32f4xx.h" 
#include "timer.h"
#include "stdbool.h"
#include <stdlib.h>
#include <string.h>
#include "dht.h"
#include "tim.h"
#include <ctype.h>
// Device header

#define LEDPIN PA_5
#define LEDSEMIPERIOD 5000000 //In minisec
#define BUFF_SIZE 6 // Conscious decision since AEM consists of only 5 digits
#define QUEUE_SIZE 20

static Queue rx_queue;
static Queue msg_queue;
static int switchPresses;

static void TouchSensorISR(int status) {
		float temperature, humidity;
		ReadDHT11(PA_1, &temperature, &humidity);
		char buffer[200];
		sprintf(buffer, "Temperature: %.2f°C, Humidity: %.2f%%\r\n", (double)temperature, (double)humidity);
		uart_print(buffer);
}

static void timer_callback_isr(void) {
  // Avoiding to overpopulate the isr
  gpio_toggle(LEDPIN);
  //queue_enqueue(&msg_queue, gpio_get(LEDPIN)>>LEDPIN);
}
static void uart_rx_isr(uint8_t rx) {
	// Check if the received character is a printable ASCII character
	if (rx >= 0x0 && rx <= 0x7F ) {
		// Store the received character
		queue_enqueue(&rx_queue, rx);
	}
}

static bool checkMessg()
{
    int lastState = -1;
    char buffer[50];
    if (queue_dequeue(&msg_queue,&lastState))
		{
		switch(lastState)
				{
				case 0: sprintf(buffer, "Led is off.\r\n");break;
				case 1: sprintf(buffer, "Led is on.\r\n");break;
				case 2: sprintf(buffer, "Button pressed %d times.\r\n",++switchPresses);break;
				}
				uart_print(buffer);
				return true;
		}
		return false;
}

static int checkAEM(char *buff)
{
  for(int i = 0; i<BUFF_SIZE; i++) {
		if (!isdigit(buff[i])) {
			return 0;
		}
	return 1;
	}
}
const char* get_uart_input(char* buff) {
	// Variables to help with UART read
	uint8_t rx_char = 0;
	uint32_t buff_index;
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
	__enable_irq(); // Enable interrupts
	
	uart_print("\r\n");// Print newline
	

	// Prompt the user to enter their string
	uart_print("Enter your AEM:");
	buff_index = 0; // Reset buffer index
	
	do {
		// Wait until a character is received in the queue
		while (!queue_dequeue(&rx_queue, &rx_char))
			__WFI(); // Wait for Interrupt

		if (rx_char == 0x08) { // Handle backspace character
			if (buff_index > 0) {
				buff_index--; // Move buffer index back
				uart_tx(rx_char); // Send backspace character to erase on terminal
			}
		} else {
			// Store and echo the received character back
			buff[buff_index++] = (char)rx_char; // Store character in buffer
			uart_tx(rx_char); // Echo character back to terminal
		}
	} while (rx_char != '\r' && buff_index < (BUFF_SIZE - 1)); // Continue until Enter key or buffer full
	// BUFF_SIZE - 1: Allow only 5 digit AEM and terminate with \0
	
	// Replace the last character with null terminator to make it a valid C string
	//buff[buff_index - 1] = '\0';
	uart_print("\r\n"); // Print newline
	
	// Check if buffer overflow occurred
	if (buff_index > BUFF_SIZE) {
		uart_print("Stop trying to overflow my buffer! I resent that!\r\n");
	}
	return buff;
}



int main()
{
		char buff[BUFF_SIZE];

		gpio_set_mode(LEDPIN, Output);
	
		// Initialize the receive queue and UART
		queue_init(&rx_queue, 128);
		uart_init(115200);
		uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
		uart_enable(); // Enable UART module
	

	
		timerInitialize(1000);
		timerEnable();
		timerSetCallback(timer_callback_isr);
	
		uart_print("\r\nInitializing\r\n");
    __enable_irq(); 

  //**************************************************
  // P_SW Button initialization
  //**************************************************
  gpio_set_mode(PA_0, Input);        
  gpio_set_callback(PA_0, TouchSensorISR);
  gpio_set_trigger(PA_0, Rising);
	
	do {
				// Variables to help with UART read
	uint8_t rx_char = 0;
	uint32_t buff_index;
	

	
	uart_print("\r\n");// Print newline
	

	// Prompt the user to enter their string
	uart_print("Enter your AEM:");
	buff_index = 0; // Reset buffer index
	
	do {
		// Wait until a character is received in the queue
		while (!queue_dequeue(&rx_queue, &rx_char))
			__WFI(); // Wait for Interrupt

		if (rx_char == 0x08) { // Handle backspace character
			if (buff_index > 0) {
				buff_index--; // Move buffer index back
				uart_tx(rx_char); // Send backspace character to erase on terminal
			}
		} else {
			// Store and echo the received character back
			buff[buff_index++] = (char)rx_char; // Store character in buffer
			uart_tx(rx_char); // Echo character back to terminal
		}
	} while (rx_char != '\r' && buff_index < (BUFF_SIZE - 1)); // Continue until Enter key or buffer full
	// BUFF_SIZE - 1: Allow only 5 digit AEM and terminate with \0
	
	// Replace the last character with null terminator to make it a valid C string
	buff[buff_index - 1] = '\0';
	uart_print("\r\n"); // Print newline
	
	// Check if buffer overflow occurred
	if (buff_index > BUFF_SIZE) {
		uart_print("Stop trying to overflow my buffer! I resent that!\r\n");
	}
	uart_print(buff);
	} while(!checkAEM(buff));
	uart_print(buff);
	
	while(1)
	{
		__WFI();
  }

    __disable_irq();
}
