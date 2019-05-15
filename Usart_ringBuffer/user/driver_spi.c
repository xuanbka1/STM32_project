/*********************************************************************************************************
 ref https://github.com/boyisgood86/stm32-spi-W25x16/blob/master/stm32_W25x%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F.txt
*********************************************************************************************************/

#include <stm32f10x.h>
#include "driver_spi.h"


// add new driver //
void SPI_FLASH_Init(void)
{
	// Initialization struct
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
    // Step 1: Initialize SPI
	#ifdef SPI1_ACTIVE
    RCC_APB2PeriphClockCmd(SPIx_RCC, ENABLE);
	#endif
	
	#ifdef SPI2_ACTIVE
    RCC_APB1PeriphClockCmd(SPIx_RCC, ENABLE);
	#endif
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    SPI_Init(SPIx, &SPI_InitStruct); 
    SPI_Cmd(SPIx, ENABLE);
 
    // Step 2: Initialize GPIO
    RCC_APB2PeriphClockCmd(SPI_GPIO_RCC, ENABLE);
    // GPIO pins for MOSI, MISO, and SCK
    GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
    // GPIO pin for SS
    GPIO_InitStruct.GPIO_Pin = SPI_PIN_SS;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
 
    // Disable SPI slave device
		SPI_FLASH_CS_HIGH(); 

}

uint8_t SPI_FLASH_SendByte(uint8_t data)
{
    // Write data to be transmitted to the SPI data register
    SPIx->DR = data;
    // Wait until transmit complete
    while (!(SPIx->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPIx->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPIx->SR & (SPI_I2S_FLAG_BSY));
    // Return received data from SPI data register
    return SPIx->DR;
}

void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(WriteEnable);
  SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WriteDisable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(WriteDisable);
  SPI_FLASH_CS_HIGH();
}

u8 SPI_Flash_ReadStatusRegister(void)
{
  u8 StatusRegister = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReadStatusRegister);
  StatusRegister = SPI_FLASH_ReceiveByte();
  SPI_FLASH_CS_HIGH();
  return StatusRegister;
}

void SPI_Flash_WriteStatusRegister(u8 Byte)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(WriteStatusRegister);
  SPI_FLASH_SendByte(Byte);
  SPI_FLASH_CS_HIGH();
}



u8 SPI_FLASH_ReceiveByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

u8 SPI_FLASH_ByteRead(u32 ReadAddr)
{
  u32 Temp = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(Read_Data);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
  Temp = SPI_FLASH_ReceiveByte();
  //Temp = SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_CS_HIGH();
  return Temp;
}

u8 SPI_FLASH_FasttRead(u32 ReadAddr)
{
  u32 Temp = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(FastReadData);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  Temp = SPI_FLASH_ReceiveByte();
  //Temp = SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_CS_HIGH();
  return Temp;
}

void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(Read_Data);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while(NumByteToRead--)
  {
    *pBuffer = SPI_FLASH_ReceiveByte();
    pBuffer++;
  }
  SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SectorErace);
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(u32 BlockAddr)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(BlockErace);
  
  SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(BlockAddr & 0xFF);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_ChipErase(void)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ChipErace);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PowerDown()
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(Power_Down);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_ReleasePowerDown()
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReleacePowerDown);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

u8 SPI_FLASH_ReadDeviceID(void)
{
  u8 DeviceID = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReadDeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  DeviceID = SPI_FLASH_ReceiveByte();
  SPI_FLASH_CS_HIGH();      
  return DeviceID;
}

u16 SPI_FLASH_ReadManuID_DeviceID(u32 ReadManu_DeviceID_Addr)
{
  u16 ManuID_DeviceID = 0;
  u8 ManufacturerID = 0,  DeviceID = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReadManuIDDeviceID);
  
  SPI_FLASH_SendByte((ReadManu_DeviceID_Addr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadManu_DeviceID_Addr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadManu_DeviceID_Addr & 0xFF);
  
  if(ReadManu_DeviceID_Addr==1)
  {
    DeviceID = SPI_FLASH_ReceiveByte();
    ManufacturerID = SPI_FLASH_ReceiveByte();
  }
  else 
  {
    ManufacturerID = SPI_FLASH_ReceiveByte();
    DeviceID = SPI_FLASH_ReceiveByte();
  }
  ManuID_DeviceID = ((ManufacturerID<<8) | DeviceID);
  SPI_FLASH_CS_HIGH();      
  return ManuID_DeviceID;
}


u32 SPI_FLASH_ReadJedecID(void)
{
  u32 JEDECID = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReadJedec_ID);
  Temp0 = SPI_FLASH_ReceiveByte();
  Temp1 = SPI_FLASH_ReceiveByte();
  Temp2 = SPI_FLASH_ReceiveByte();
  SPI_FLASH_CS_HIGH();  
  JEDECID = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return JEDECID;
}


void SPI_FLASH_ByteWrite(u8 Byte, u32 WriteAddr)
{
  SPI_FLASH_WriteEnable(); 
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(Page_Program);
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);  
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
  
  SPI_FLASH_SendByte(Byte); 
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

/* spi flash write multibyte to flash */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  SPI_FLASH_WriteEnable(); 
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(Page_Program);
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);  
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
  
  while(NumByteToWrite--)
  {
    SPI_FLASH_SendByte(*pBuffer);
    pBuffer++; 
  }
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
  
  if(Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */ 
    {
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;  
      }    
     
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
   }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if(NumOfPage== 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;
      
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count; 
        
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
      
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;  
     
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
      
      if(NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(ReadStatusRegister);
  do
  {
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  } while((FLASH_Status & WriteStatusRegister) == SET);

  SPI_FLASH_CS_HIGH();
}