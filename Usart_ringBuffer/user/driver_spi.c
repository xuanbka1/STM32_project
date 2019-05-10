/*********************************************************************************************************

*********************************************************************************************************/

#include <stm32f10x.h>
#include "driver_spi.h"


static void SPI_Rcc_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
}

static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;

	SPI_Rcc_Configuration();

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);
	
	GPIO_Configuration();

	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_Send_byte(u16 data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,data);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI1);
}

u16 SPI1_Receive_byte(void)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x00);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

/* brief : Function for Flash memory */
void AT45DBXX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_Configuration();
//	printf("SPI is ready!\r\n");

	/*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);*/

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	AT45DBXX_Disable;
}

static void AT45DBXX_BUSY(void)
{
	u8 AT45DBXXStruct;
	AT45DBXX_Enable;
	SPI1_Send_byte(READ_STATE_REGISTER);
	while(!(AT45DBXXStruct & 0x80))
		AT45DBXXStruct = SPI1_Receive_byte();
	
	AT45DBXX_Disable;	
}

void AT45DBXX_Read_ID(u8 *IData)
{
	u8 i;
	AT45DBXX_BUSY();
	AT45DBXX_Enable;
	SPI1_Send_byte(Read_ID); //÷¥––∂¡»°id√¸¡Ó		  	
  	for(i=0;i<4;i++)
  	{
  		IData[i] = SPI1_Receive_byte();
  	}
  	AT45DBXX_Disable;
}

void write_buffer(u8 BufferOffset,u8 Data)
{
	u16 add;
	add = 0x0000 | BufferOffset; 
	AT45DBXX_BUSY();			
	AT45DBXX_Enable;
	SPI1_Send_byte(BUF1_WRITE);
	SPI1_Send_byte(0xff);
	SPI1_Send_byte((u8)add>>8);
	SPI1_Send_byte((u8)add);
	//SPI1_Send_byte(0xff);		
	SPI1_Send_byte(Data);			   	
	AT45DBXX_Disable;
}

u8 read_buffer(u8 BufferOffset)
{		
	u8 temp;
	u16 add;
	add = 0x0000 | BufferOffset;
	AT45DBXX_BUSY();
	AT45DBXX_Enable;
 	SPI1_Send_byte(BUF1_READ);
	SPI1_Send_byte(0xff);
	SPI1_Send_byte((u8)add>>8);
	SPI1_Send_byte((u8)add);
	SPI1_Send_byte(0xff);
	temp=SPI1_Receive_byte();
	AT45DBXX_Disable;
	return temp;		

}

