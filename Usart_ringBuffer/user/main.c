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


	
	
uint8_t  data;
	
		uint8_t num=0;
	uint8_t ID[4];
	uint8_t data_spi[25];
int main(void)
{
	SystemInit();

	usart_Configuration(USART1);	
	NVIC_Configuration();
	
	

		
	AT45DBXX_Init();
	printf("AT45DBXX had been Init!\r\n");
	AT45DBXX_Read_ID(ID);
	printf("AT45DBXX ID is");
	for(num=0;num<4;num++)
	{
		printf(" 0x%x ",ID[num]);
	}
	printf("\r\nWrite 255 byte data to buff1:\r\n");
	for(num=0;num<255;num++){
		write_buffer(num,num);
		printf(" %d ",num);
	}
	printf("\r\nRead 255 byte data from buff1:\r\n");
	for(num=0;num<255;num++){
		data_spi[num] = read_buffer(num);
		printf(" %d ",data_spi[num]);
	}
	
	
UART_PutString(USART1," solution SPi Flash !!!! ");


  	while(1)
	{
		
		
		while(USART_data1.RXin_ptr != USART_data1.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_data1);
		telegram_PushChar(USART1, &telegram1, data);
		}
		
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

