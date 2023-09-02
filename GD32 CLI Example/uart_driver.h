#ifndef UART_DRV_H
#define UART_DRV_H
#include <stdint.h>
#include <stdio.h>
#include "fifo.h"
uint8_t Uart_Transmit(int8_t);
int8_t Uart_Receive(FIFO_BUFFER*);


#endif