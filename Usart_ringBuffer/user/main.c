#include <stm32f10x.h>
#include "usart.h"
#include "eeprom.h"
#include <stm32f10x_flash.h>
//#define CTRT
#include "stdio.h"
#include "string.h"

#include "GSM.h"

#define USART_PC  USART3

UART_data_t USART_data1;
telegram_buffer_t telegram1;

UART_data_t USART_data2;
telegram_buffer_t telegram2;

// usart data PC command //
UART_data_t USART_PCdata;
telegram_buffer_t telegram_PC;

void UART2_Configuration (USART_TypeDef* USARTx);
void UART3_Configuration (USART_TypeDef* USARTx);

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

void UART2_Configuration (USART_TypeDef* USARTx)
	{
		if(USARTx == USART2)
		{
			GPIO_InitTypeDef			GPIO_InitStructure;
			USART_InitTypeDef			UART_InitStructure;
			NVIC_InitTypeDef			NVIC_InitStructure;		
		
			/*Cap clock cho USART và port su dung*/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
			/* Cau Tx mode AF_PP, Rx mode FLOATING  */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
			/*Cau hinh USART*/
			UART_InitStructure.USART_BaudRate = 9600;
			UART_InitStructure.USART_WordLength = USART_WordLength_8b;
			UART_InitStructure.USART_StopBits = USART_StopBits_1;
			UART_InitStructure.USART_Parity = USART_Parity_No;
			UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USARTx, &UART_InitStructure);
				
			/* Cau hinh vector ngat va muc uu tien */
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			/* xoa co ngat nhan cho lan dau su dung*/
			USART_ClearFlag(USARTx, USART_IT_RXNE);
			
			/* Cau hinh cho phep ngat nhan*/
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
			USART_Cmd(USARTx, ENABLE);
		}
	
	}

	// usart3 config //
	
	void UART3_Configuration (USART_TypeDef* USARTx)
	{
		
		if(USARTx == USART3)
		{
		GPIO_InitTypeDef			GPIO_InitStructure;
		USART_InitTypeDef			UART_InitStructure;
		NVIC_InitTypeDef			NVIC_InitStructure;		
		
		/*Cap clock cho USART và port su dung*/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
			/* Cau Tx mode AF_PP, Rx mode FLOATING  */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*Cau hinh USART*/
			UART_InitStructure.USART_BaudRate = 9600;
			UART_InitStructure.USART_WordLength = USART_WordLength_8b;
			UART_InitStructure.USART_StopBits = USART_StopBits_1;
			UART_InitStructure.USART_Parity = USART_Parity_No;
			UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART3, &UART_InitStructure);
				
		/* Cau hinh vector ngat va muc uu tien */
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			/* xoa co ngat nhan cho lan dau su dung*/
			USART_ClearFlag(USARTx, USART_IT_RXNE);
			
			/* Cau hinh cho phep ngat nhan*/
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
			USART_Cmd(USARTx, ENABLE);
		}
	}
	
	
uint8_t  data;
int main(void)
{
	SystemInit();

	UART2_Configuration(USART2);
	UART3_Configuration(USART3);
	usart_Configuration(USART1);	
	NVIC_Configuration();
	
	
UART_PutString(USART1," hello stm32 usart11111");
UART_PutString(USART2," hello stm32 usart22222");
UART_PutString(USART3," hello stm32 usart33333");

  	while(1)
	{
		
		while(USART_data2.RXin_ptr != USART_data2.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_data2);
		telegram_PushChar(USART1, &telegram2, data);
		}
		
		
		
		// get data form PC via USART1 and forward to USART2 //
		while(USART_data1.RXin_ptr != USART_data1.RXout_ptr)
		{
			data = UART_RXBufferGetChar(&USART_data1);
			//telegramPC_PushChar(USART2, &telegram1, data);
			telegram_PushChar(USART2, &telegram1, data);
		}
		
		
		
		while(USART_PCdata.RXin_ptr != USART_PCdata.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_PCdata);
		telegram_PushChar(USART1, &telegram_PC, data);
		}
		
		//Flash_Write(EEPROM_START_ADDRESS + 1, 1234);
		
		
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

/*interrupt usart2 */
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {	
		uint16_t data = USART_ReceiveData(USART2);
		UART_GetbyteIrq(&USART_data2,(uint8_t)data);    
  }
}

/*interrupt usart3 */
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART_PC, USART_IT_RXNE) != RESET)
  {	
		uint16_t data = USART_ReceiveData(USART_PC);
		UART_GetbyteIrq(&USART_PCdata,(uint8_t)data);    
  }
}



