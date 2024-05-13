/*
  Author: chatziiola (Lamprinos Chatziioannou)
  Date: 2024-05-13
  Description: Nothing fancy just uart I/O functions
*/

#include "uart.h"
#include "queue.h"

void uart_rx_isr(uint8_t rx);
const char* get_uart_input(char* buff) ;
