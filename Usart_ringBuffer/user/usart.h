#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include <stm32f10x.h>


//uint16_t data;

#define  RX_BUFLEN 255
#define  TX_BUFLEN 255
#define  MS_RX_BUFSIZE 255

/* \brief  Receive buffer size: 2,4,8,16,32,64,128 or 256 bytes. */
#define USART_RX_BUFFER_SIZE 256
/* \brief Transmit buffer size: 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 256
/* \brief Receive buffer mask. */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
/* \brief Transmit buffer mask. */
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )


#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif


/* \brief USART transmit and receive ring buffer. */
typedef struct
{
	uint8_t buffer[USART_RX_BUFFER_SIZE];
	uint16_t RXin_ptr;
	uint16_t RXout_ptr;
	uint16_t count;

	uint16_t TXin_ptr;
	uint16_t TXout_ptr;
} UART_data_t;

typedef struct
{
	uint8_t buffer[USART_RX_BUFFER_SIZE];
	uint16_t count;
} telegram_buffer_t;

extern UART_data_t USART_data1;
extern telegram_buffer_t telegram1;

extern UART_data_t USART_data2;
extern telegram_buffer_t telegram2;

void UART_Initialize(UART_data_t * uart_data, telegram_buffer_t* telegram_buffer);
void UART_PutString(USART_TypeDef* USARTx, char* str);
void UART_PutChar(USART_TypeDef* USARTx, char data);
void UART_GetbyteIrq( UART_data_t* uart_data, uint8_t data);
uint8_t UART_RXBufferGetChar(UART_data_t* uart_rxc);

uint8_t telegram_PushChar(USART_TypeDef* USARTx, telegram_buffer_t* telegram_buffer, uint8_t data);
void telegram_Process(USART_TypeDef* USARTx, uint8_t *telegram, uint8_t len);


void Rcc_Configuration(void);
void UsartGPIO_Configuration(void);
void usart_Configuration(USART_TypeDef* USARTx);
void UsartGPIO_CTRT_Configuration(void);
void USART_CTRT_Configuartion(void);

#endif /*_USART_H*/
