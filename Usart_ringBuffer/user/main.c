#include <stm32f10x.h>
#include "usart.h"
//#define CTRT
#include "stdio.h"
#include "string.h"

#include "driver_spi.h"
#include "GSM.h"

#define USART_PC  USART3

UART_data_t USART_data1;
telegram_buffer_t telegram1;

UART_data_t USART_data2;
telegram_buffer_t telegram2;

// usart data PC command //
UART_data_t USART_PCdata;
telegram_buffer_t telegram_PC;


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);	
}
	//// for eeprom //
#define SPI_BUFFER_SIZE 255
u8 DataByte=0;
u8 Tx_Buffer[SPI_BUFFER_SIZE];
u8 Rx_Buffer[SPI_BUFFER_SIZE];

vu32 FLASH_ID = 0;

uint8_t  data;
uint8_t num=0;
uint8_t ID[4];
uint8_t data_spi[25];
int i;
int main(void)
{
	SystemInit();

	usart_Configuration(USART1);	
	NVIC_Configuration();
	
	SPI_FLASH_Init();
	
UART_PutString(USART1," solution SPi Flash !!!! ");
printf(" hello spi1");

	for(i=0; i<1000; i++)
	{
		Tx_Buffer[i]  = i;
	}
	
	while(1)
	{		
		while(USART_data1.RXin_ptr != USART_data1.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_data1);
		telegram_PushChar(USART1, &telegram1, data);
		}		
	}
}

// process buffer data //
 void telegram_Process(USART_TypeDef* USARTx,uint8_t *telegram, uint8_t len)
{
	
	UART_PutString(USARTx,(char*)telegram);
	UART_PutString(USARTx,"\r");
	// struct of telegram //
	/*
	telegram[0] : w = write , r = read
	telegram[1] : address 
	telegram[2] : val
	*/
	if(telegram[0] == 'w')
	{
		uint8_t add = telegram[1] - 0x30;
		uint8_t val = telegram[2] - 0x30;
		SPI_FLASH_ByteWrite(add, val);
		printf(" write %d to add %d \n\r ", val, add);
	}
	
	if(telegram[0] == 'r')
	{
		
		uint8_t add = telegram[1] - 0x30;
		uint8_t val =	SPI_FLASH_ByteRead(add);
		printf(" val from add %d is :%d \n\r  ",add, val);
	}
	
	// write multibyte //
	
	SPI_FLASH_PageWrite(Tx_Buffer, 0x01F01F, SPI_BUFFER_SIZE);
  for(i =0; i<SPI_BUFFER_SIZE; i++)
	{
			printf(" write %d \n\r", Tx_Buffer[i]);
	}
	
	printf("----------------------------------------------\n\r");
	
  SPI_FLASH_BufferRead(Rx_Buffer, 0x01F01F, SPI_BUFFER_SIZE);
	 for( i =0; i<SPI_BUFFER_SIZE; i++)
	{
			printf(" data read from flash %d \n\r", Rx_Buffer[i]);
	}
}


///////////////////////////////////////////////////////////////////////////////////
/* brief -- process interrupt */
/*interrupt usart1 */
void USART1_IRQHandler(void)
{  	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{ 
			uint16_t data = USART_ReceiveData(USART1);
			UART_GetbyteIrq(&USART_data1,(uint8_t)data); 
  	}
	
}

