/*
  Author: selivanof(George Selivanof), chatziiola (Lamprinos Chatziioannou)
  Date: 2024-05-10
  Description: Initial draft for what is asked in MP lab 2
*/

// WARNING: The microprocesor part of this is largely untested

// Note to reader: make sure to use the proper input function when testing
// Task list: I've inserted all tasks as %TODO% elements, to be easily verifiable

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

// Device header

#define LEDPIN PA_5
#define LEDSEMIPERIOD 500000 //In minisec
#define BUFF_SIZE 6 // Conscious decision since AEM consists of only 5 digits
#define QUEUE_SIZE 20

Queue rx_queue;
static Queue msg_queue;
static int switchPresses;

// TODO switch -> ISR, turn on led, count switchpresses
// TODO switch and LED -> ISR, turn off led, count switchpresses
void switchPressed_isr(int status) {
  timer_disable();
	gpio_toggle(LEDPIN);
  queue_enqueue(&msg_queue,gpio_get(LEDPIN)>>LEDPIN);
  queue_enqueue(&msg_queue, 2);
  
}

void timer_callback_isr(void) {
  // Avoiding to overpopulate the isr
  gpio_toggle(LEDPIN);
  queue_enqueue(&msg_queue, gpio_get(LEDPIN)>>LEDPIN);
}

void uart_rx_isr(uint8_t rx) {
	// Check if the received character is a printable ASCII character
	if (rx >= 0x0 && rx <= 0x7F ) {
		// Store the received character
		queue_enqueue(&rx_queue, rx);
	}
}

bool checkMessg()
{
    int lastState = -1;
    char buffer[50];
    if (queue_dequeue(&msg_queue,&lastState))
		{
		switch(lastState)
				{
				case 0: sprintf(buffer, "Led is off.\r\n");break;
				case 1: sprintf(buffer, "Led is on.\r\n");break;
				case 2: sprintf(buffer, "Keypresses pressed %d .\r\n",++switchPresses);break;
				}
				uart_print(buffer);
				return true;
		}
		return false;
}

void checkAEM(char *buff)
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

bool checkUart(uint32_t *buff_index, char *buff)
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

// switch should cause ISR but how?
/* if (switch_get(P_SW)) */
int main()
{
		
    char buff[BUFF_SIZE];
    switchPresses = 0;
    queue_init(&msg_queue, QUEUE_SIZE);
    uart_init(115200);
    uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
    uart_enable(); // Enable UART module
    gpio_set_mode(P_SW, Input); // thus, active low
    gpio_set_callback(P_SW, switchPressed_isr);
		gpio_set_trigger(P_SW, Rising);
	
		gpio_set_mode(LEDPIN, Output);
	  timer_init(LEDSEMIPERIOD);
		timer_set_callback(timer_callback_isr);
		timer_disable();


    __enable_irq(); // Enable interrupts

    // Initialize the receive queue and UART
    queue_init(&rx_queue, 128);
    uint32_t buff_index= 0; // Reset buffer index
    // TODO repeat for multiple aems (while true loop)
    // TODO get AEM through UART
    uart_print("\r\nEnter your AEM:");

    // TODO check for valid input
    while(1) // for every letter/message
    {
	// TODO everychange in LED state, UART out
	if (!( checkUart(&buff_index,buff) || checkMessg() ))
	    __WFI();
    }
    __disable_irq();
}
