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
// Device header

#define LEDPIN PA_5
#define LEDSEMIPERIOD 5000000 //In minisec
#define BUFF_SIZE 6 // Conscious decision since AEM consists of only 5 digits
#define QUEUE_SIZE 20

static void MX_TIM2_Init(void);

Queue rx_queue;
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

static void checkAEM(char *buff)
{
  int aem = atoi(buff);
  // TODO if AEM%2, ISR -> LED toggle every 0.5 sec
  if (aem%2) //odd
	{
    timer_enable();
	}

  // TODO elif AEM%2==0, ISR -> timer off, LED same state
  else //even
    timer_disable();
}

static bool checkUart(uint32_t *buff_index, char *buff)
{
	uint8_t rx_char = 0;
	if (queue_dequeue(&rx_queue, &rx_char)) {
		// Handle backspace character
		if (rx_char == 0x08) { 
			if (*buff_index > 0) {
				(*buff_index)--; 
				uart_tx(rx_char); 
			}
		}
		else if (rx_char == '\r'){ 
		  uart_print("\r\n");

		  // mάλλον θα χρειαστεί μετα
		    checkAEM(buff);
		    *buff_index = 0;
		    memset(buff,'\0',BUFF_SIZE);
		    uart_print("\r\nEnter your AEM:");
		}
		else if ( *buff_index >= BUFF_SIZE) {
		    uart_print("Stop trying to overflow my buffer! I resent that!\r\n"); 
		}
		else {
			// Store and echo the received character back
			buff[(*buff_index)++] = (char)rx_char; // Store character in buffer
			uart_tx(rx_char); // Echo character back to terminal
		}
		return true;
	    }
	    return false;
}


int main()
{
		gpio_set_mode(LEDPIN, Output);
	
		timerInitialize(1000);
		timerEnable();
		timerSetCallback(timer_callback_isr);
	
    uart_init(115200);
    uart_set_rx_callback(uart_rx_isr);
    uart_enable(); 
		uart_print("\r\nInitializing\r\n");
    __enable_irq(); 

  //**************************************************
  // P_SW Button initialization
  //**************************************************
  gpio_set_mode(PA_0, Input);        
  gpio_set_callback(PA_0, TouchSensorISR);
  gpio_set_trigger(PA_0, Rising);
	while(1)
	{
		__WFI();
  }

    __disable_irq();
}
