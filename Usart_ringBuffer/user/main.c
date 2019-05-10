#include <stm32f10x.h>
#include "usart.h"
//#define CTRT
#include "stdio.h"
#include "string.h"

#include "AT24C02.h"
#include "GSM.h"

#define USART_PC  USART3

UART_data_t USART_data1;
telegram_buffer_t telegram1;



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
	uint16_t Addr;
	uint8_t WriteBuffer[2],ReadBuffer[2];
	
	
int main(void)
{
	
	
	SystemInit();

	usart_Configuration(USART1);	
	NVIC_Configuration();
	I2C_Configuration();
	
UART_PutString(USART1," i2c eeprom control via uart and i2c");


  	while(1)
	{
				
		// get data form PC via USART1 and forward to USART2 //
		while(USART_data1.RXin_ptr != USART_data1.RXout_ptr)
		{
			data = UART_RXBufferGetChar(&USART_data1);
			//telegramPC_PushChar(USART2, &telegram1, data);
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


void telegram_Process(USART_TypeDef* USARTx,uint8_t *telegram, uint8_t len)
{
	char buff[50];
		UART_PutString(USARTx,(char*)telegram);
	if(telegram[0]== 'a')
	{
		
		uint8_t data = telegram[1] - 0x30 +1;
		
		uint8_t* str = telegram+1;
		uint8_t x = (uint8_t)atoi(str);
		
   	WriteBuffer[0]= data;	
			WriteBuffer[1]= x +10;	
		I2C_Write(I2C1,ADDR_24LC02,0,WriteBuffer,sizeof(WriteBuffer) );
	}
	
	if(telegram[0] == 'b')
	{
		I2C_Read(I2C1,ADDR_24LC02,0,ReadBuffer,sizeof(WriteBuffer) );
		 
			for(Addr=0; Addr<2; Addr++){
		sprintf(buff, " rom is %d", ReadBuffer[Addr]);
		UART_PutString(USART1, buff);}
	}
	
}