#include <stm32f10x.h>
#include "usart.h"
//#define CTRT
#include "stdio.h"
#include "string.h"

//#define USART_PC  USART3

UART_data_t USART_data1;
telegram_buffer_t telegram1;

UART_data_t USART_data2;
telegram_buffer_t telegram2;

UART_data_t USART_data3;
telegram_buffer_t telegram3;

void UART2_Configuration (void);
void UART3_Configuration (void);

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

void UART2_Configuration (void)
	{
		
		GPIO_InitTypeDef			GPIO_InitStructure;
		USART_InitTypeDef			UART_InitStructure;
		NVIC_InitTypeDef			NVIC_InitStructure;		
		
		/*Cap clock cho USART v� port su dung*/
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
			UART_InitStructure.USART_BaudRate = 115200;
			UART_InitStructure.USART_WordLength = USART_WordLength_8b;
			UART_InitStructure.USART_StopBits = USART_StopBits_1;
			UART_InitStructure.USART_Parity = USART_Parity_No;
			UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART2, &UART_InitStructure);
				
		/* Cau hinh vector ngat va muc uu tien */
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			/* xoa co ngat nhan cho lan dau su dung*/
			USART_ClearFlag(USART2, USART_IT_RXNE);
			
			/* Cau hinh cho phep ngat nhan*/
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
			USART_Cmd(USART2, ENABLE);
	
	}

	// usart3 config //
	
	void UART3_Configuration (void)
	{
		
		GPIO_InitTypeDef			GPIO_InitStructure;
		USART_InitTypeDef			UART_InitStructure;
		NVIC_InitTypeDef			NVIC_InitStructure;		
		
		/*Cap clock cho USART v� port su dung*/
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
			UART_InitStructure.USART_BaudRate = 115200;
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
			USART_ClearFlag(USART3, USART_IT_RXNE);
			
			/* Cau hinh cho phep ngat nhan*/
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
			USART_Cmd(USART3, ENABLE);
	
	}
	
	
uint8_t  data;
int main(void)
{
	u32 i=0xffffff;
	SystemInit();

	UART2_Configuration();
	UART3_Configuration();
	usart_Configuration();	
	NVIC_Configuration();
	
	
UART_PutString(USART1, " hello stm32 usart11111");
UART_PutString(USART2, " hello stm32 usart22222");
UART_PutString(USART3, " hello stm32 usart33333");

  	while(1)
	{
		while(USART_data2.RXin_ptr != USART_data2.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_data2);
		telegram_PushChar(USART3, &telegram2, data);
		}
		
		while(USART_data1.RXin_ptr != USART_data1.RXout_ptr)
		{
		data = UART_RXBufferGetChar(&USART_data1);
		telegram_PushChar(USART3, &telegram1, data);
		}
		
	}

}


///////////////////////////////////////////////////////////////////////////////////
/* brief -- process interrupt */
/*interrupt usart1 */
void USART1_IRQHandler(void)
{  	
	//uint16_t data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{ 
			uint16_t data = USART_ReceiveData(USART1);
			UART_GetbyteIrq(&USART_data1,(uint8_t)data); 
  	}
	
}

/*interrupt usart2 */
void USART2_IRQHandler(void)
{
	uint16_t data;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {	
		uint16_t data = USART_ReceiveData(USART2);
		UART_GetbyteIrq(&USART_data2,(uint8_t)data);    
  }
}

/*interrupt usart3 */
void USART3_IRQHandler(void)
{
	uint16_t data;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {	
		uint16_t data = USART_ReceiveData(USART3);
		UART_GetbyteIrq(&USART_data3,(uint8_t)data);    
  }
}
