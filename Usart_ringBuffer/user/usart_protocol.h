#ifndef _USART_PROTOCOL_H
#define _USART_PROTOCOL_H

#include <stm32f10x.h>

#define  RX_BUFLEN 255
#define  TX_BUFLEN 255
#define  MS_RX_BUFSIZE 100



//private variables
volatile uint8_t  uart_rx_buf[RX_BUFLEN];
volatile uint8_t  uart_tx_buf[TX_BUFLEN];
uint8_t ser_rx_buffer[MS_RX_BUFSIZE];

volatile struct
{
	uint8_t in_ptr;
	uint8_t out_ptr;
} uart_txc;

volatile struct
{
	uint8_t in_ptr;
	uint8_t out_ptr;
} uart_rxc;

volatile struct
{
	uint8_t rx_chars;
	uint8_t tx_chars;
} uart_control;


volatile uint8_t ser_rx_count = 0;

void ser_receive_tel( uint8_t *tp, uint8_t len );
void UART_Tx_String(char *str);

void UART_ringbufferInit()
{
		
	uart_control.rx_chars = 0;
	uart_control.tx_chars = 0;
	uart_txc.in_ptr=3;
	uart_txc.out_ptr=3;
	uart_rxc.in_ptr=5;
	uart_rxc.out_ptr=5;
	
}

uint8_t uart_ringbufferGet(void)
{
	uint8_t rxc;

	if(!uart_control.rx_chars) return 0;
	
	rxc = uart_rx_buf[uart_rxc.out_ptr++];
	if(uart_rxc.out_ptr >= RX_BUFLEN)
	uart_rxc.out_ptr=0;
	uart_control.rx_chars--;
	return rxc;
}


#endif /*_USART_H*/