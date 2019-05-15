#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"


#define SPI1_ACTIVE
//#define SPI2_ACTIVE

#ifdef SPI2_ACTIVE

#define SPIx_RCC      RCC_APB1Periph_SPI2
#define SPIx          SPI2
#define SPI_GPIO_RCC  RCC_APB2Periph_GPIOB
#define SPI_GPIO      GPIOB
#define SPI_PIN_MOSI  GPIO_Pin_15
#define SPI_PIN_MISO  GPIO_Pin_14
#define SPI_PIN_SCK   GPIO_Pin_13
#define SPI_PIN_SS    GPIO_Pin_12

#define SPI_FLASH_CS_LOW()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()       GPIO_SetBits(GPIOB, GPIO_Pin_12)

#endif  /* end if define SPI2 */

// define for SPI2 //
#ifdef SPI1_ACTIVE

#define SPIx_RCC      RCC_APB2Periph_SPI1
#define SPIx          SPI1
#define SPI_GPIO_RCC  RCC_APB2Periph_GPIOA
#define SPI_GPIO      GPIOA
#define SPI_PIN_MOSI  GPIO_Pin_7
#define SPI_PIN_MISO  GPIO_Pin_6
#define SPI_PIN_SCK   GPIO_Pin_5
#define SPI_PIN_SS    GPIO_Pin_4

#define SPI_FLASH_CS_LOW()        GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH()       GPIO_SetBits(GPIOA, GPIO_Pin_4)

#endif /* end if define SPI1 */

//define for w25x16 winbond flash IC 
// ref: https://media.digikey.com/pdf/Data%20Sheets/Winbond%20PDFs/W25X16,16A,32,64.pdf

#define countof(a)                (sizeof(a) / sizeof(*(a)))
#define BufferSize               (countof(Tx_Buffer)-1)
#define SPI_FLASH_PageSize        256
#define Dummy_Byte                0xA5


#define WriteEnable               0x06       //???,???????
#define WriteDisable              0x04       //???
#define ReadStatusRegister        0x05       //??????
#define WriteStatusRegister       0x01       //??????
#define Read_Data                 0x03       //???????
#define FastReadData              0x0B       //?????????
#define FastReadDualOutput        0x3B       //????????????????
#define Page_Program              0x02       //????--???

#define BlockErace                0xD8       //???
#define SectorErace               0x20       //????
#define ChipErace                 0xC7       //???
#define Power_Down                0xB9       //????

#define ReleacePowerDown          0xAB       //??????
#define ReadDeviceID              0xAB       //????ID??


#define ReadDeviceID              0xAB       //?????????ID??
#define ReadManuIDDeviceID        0x90       //??????ID?????ID??
#define ReadJedec_ID              0x9F       //JEDEC?ID??
#define SPI_FLASH_PageSize        256

void SPI_FLASH_Init(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
u8 SPI_Flash_ReadStatusRegister(void);
void SPI_Flash_WriteStatusRegister(u8 Byte);

u8 SPI_FLASH_SendByte(u8 byte);
u8 SPI_FLASH_ReceiveByte(void);

u8 SPI_FLASH_ByteRead(u32 ReadAddr);
u8 SPI_FLASH_FasttRead(u32 ReadAddr);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(u32 BlockAddr);
void SPI_FLASH_ChipErase(void);

void SPI_FLASH_PowerDown();
void SPI_FLASH_ReleasePowerDown();
u8 SPI_FLASH_ReadDeviceID(void);
u16 SPI_FLASH_ReadManuID_DeviceID(u32 ReadManu_DeviceID_Addr);
u32 SPI_FLASH_ReadJedecID(void);

void SPI_FLASH_ByteWrite(u8 Byte, u32 WriteAddr);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

void SPI_FLASH_WaitForWriteEnd(void);




#endif /*__SPI_H*/
