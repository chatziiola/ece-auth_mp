/*
  Author: chatziiola (Lamprinos Chatziioannou)
  Date: 2024-05-10
  Description: Initial draft for what is asked in MP lab 2
*/

// WARNING: The microprocesor part of this is largely untested

// Note to reader: make sure to use the proper input function when testing
// Task list: I've inserted all tasks as %TODO% elements, to be easily verifiable

// Conscious decision since AEM consists of only 
#define BUFF_SIZE 6
#define QUEUE_SIZE 20

// Uncomment to play on platform
/* #define USE_ON_PLATFORM  */

#include <stdio.h>
#include <gpio.h>
#include "platform.h"
#include "queue.h"
#include "uart.h"
#include "stm32f4xx.h"                  // Device header

#include "led.h"
#include "uartCompl.h"
#include "switches.h"

int switchPresses;

void aem_isr(int aem)
{
  if (aem%2)
    {
      //odd
    }

  else
    {
      //even
    }
}

/* void aemIsEven_isr(void *arg) */
/* { */
/*    ... /\* ISR code *\/ */
/* } */


void switchPressed_isr(void)
{
  switchPresses++;
  // Now does this mean that it should set timer_disable(); I do not know

  // Should be outside of ISR
  //  uart_print(Switchpresses);
  
}

int main()
{
  char input[BUFF_SIZE];
  switchPresses = 0;


// TODO repeat for multiple aems (while true loop)
  while(1)
    {
    // TODO get AEM through UART
    #ifdef USE_ON_PLATFORM
	get_uart_input(input);
    #else
	scanf("Enter your AEM:\n%s", input);
    #endif

    __enable_irq();

    // Πως γίνεται να στέλνουμε ISR ;ετσι -> xrειάζεται για να μπορούμε να 
    // τυπώνουμε όλες τις αλλαγές με ένα queue loop ;οπως το σκέφτομαι από την main.
    // TODO if AEM%2, ISR -> LED toggle every 0.5 sec
    // TODO elif AEM%2==0, ISR -> timer off, LED same state
    int aem = int(input);
    if (aem%2)
    {
	aemIsOdd_isr(void *arg)
    }
    else
    {
      aemIsEven_isr(void *arg)
    }
    // TODO siwtch -> ISR, turn on led, count switchpresses
    
    // TODO switch and LED -> ISR, turn off led, count switchpresses
    // switch should cause ISR but how?
    /* if (switch_get(P_SW)) */

    // TODO everychange in LED state, UART out
    while(queue_dequeue(&led_queue, &state))
      {
	/* uart_print(changed state); */
      }
    __disable_irq();
    }
}
