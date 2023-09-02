#include "fifo.h"
#include "gd32e23x_usart.h"
int8_t Uart_Transmit(int8_t data)
{
    usart_data_transmit(USART1, (uint8_t)data);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return data;
}

int8_t Uart_Receive(FIFO_BUFFER* rx_fifo)
{
		uint8_t data;
		while( SET == usart_flag_get(USART1,USART_FLAG_RBNE))
		{
			data = (uint8_t)usart_data_receive(USART1);
			FIFO_Put(rx_fifo,data);
		}
		
    return data;
}