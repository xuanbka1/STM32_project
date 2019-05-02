#include "usart.h"


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void Rcc_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
}

void UsartGPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void usart_Configuration(USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
	USART_InitTypeDef USART_InitStruct;

	Rcc_Configuration();

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);

	UsartGPIO_Configuration();
	}
}

void UsartGPIO_CTRT_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;/*GPIO_Pin_1 | GPIO_Pin_2;*/
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;/*GPIO_Pin_0 | GPIO_Pin_3;*/
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void USART_CTRT_Configuartion(void)
{
	USART_InitTypeDef USART_InitStruct;

	Rcc_Configuration();

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);

	UsartGPIO_CTRT_Configuration();
}

//不使用半主机模式
#if 1 //如果没有这段，则需要在target选项中选择使用USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

_sys_exit(int x)  
{  
	x = x;  
}
#endif

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(u8)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}


void UART_PutChar(USART_TypeDef* USARTx, char ch)
{
  while(!(USARTx->SR & USART_SR_TXE));
  USARTx->DR = ch;  
}

void UART_PutString(USART_TypeDef* USARTx, char* str)
{
  while(*str != 0)
  {
    UART_PutChar(USARTx, *str);
    str++;
  }
}

void UART_Initialize(UART_data_t * uart_data, telegram_buffer_t* telegram_buffer)
 {
	 // all right with all MCU //
	 uart_data->count = 0;
	 uart_data->RXin_ptr=0;
	 uart_data->RXout_ptr=0;

	 telegram_buffer->count = 0;
 }


 //////////////////////////////////////////////////////
 uint8_t UART_RXBufferGetChar(UART_data_t* uart_rxc)
 {
	 uint8_t rxc;

	 if(uart_rxc->RXin_ptr == uart_rxc->RXout_ptr) return 0;

	 rxc = uart_rxc->buffer[uart_rxc->RXout_ptr++];
	 if(uart_rxc->RXout_ptr >= USART_RX_BUFFER_SIZE)
	 uart_rxc->RXout_ptr=0;

	 return rxc;
 }

/*! \brief Get byte received from Interrupt UART then push to uart buffer. */
 void UART_GetbyteIrq( UART_data_t* uart_data, uint8_t data)
 {
	 // Push into buffer
	 uart_data->buffer[uart_data->RXin_ptr++] = data;
	 if(uart_data->RXin_ptr >= USART_RX_BUFFER_SIZE) // over buffer //
	 uart_data->RXin_ptr=0;
 }


/*! \brief push one character to telegram buffer. */
 uint8_t telegram_PushChar(USART_TypeDef* USARTx,telegram_buffer_t* telegram_buffer, uint8_t data )
 {
	 uint8_t ret;

	 if( (data == '\n') || (data == '\r') )	//Normal serial operating
	 {
		 if( telegram_buffer->count )
		 {
			 telegram_buffer->buffer[telegram_buffer->count] = 0;   //make a proper c-string
			 ret = telegram_buffer->count;
			 telegram_buffer->count = 0;
			 
			 //Process telegram
				telegram_Process(USARTx, telegram_buffer->buffer, ret);					
		 }
		 telegram_buffer->count=0;
	 }
	 else
	 {
		 telegram_buffer->buffer[telegram_buffer->count++] = data;
		 if( telegram_buffer->count >= (USART_RX_BUFFER_SIZE-1) )
		 telegram_buffer->count=0; //prevent buffer overflow
	 }
	 
	 return 0;
 }

 
 void telegram_Process(USART_TypeDef* USARTx,uint8_t *telegram, uint8_t len)
{
	if(USARTx == USART1)
	{
		UART_PutString(USARTx,"GSM");
		UART_PutString(USARTx,(char*)telegram);
	}
	else if(USARTx == USART2)
	{
		UART_PutString(USART2,(char*)telegram);
		UART_PutString(USART2,"\r");
	}
}

