#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H
#include "stm32f10x_spi.h"

static void SPI_Rcc_Configuration(void);
static void GPIO_Configuration(void);
void SPI_Configuration(void);
void SPI1_Send_byte(u16 data);
u16 SPI1_Receive_byte(void);

/* brief funtion for AT45BD flash memory */
#define BUF1_WRITE 0x84
#define BUF2_WRITE 0x87
#define BUF1_READ 0xD4
#define BUF2_READ 0xD6
#define BBUF1_TO_MM_PAGE_PROG_WITH_ERASE 0x83
#define BBUF2_TO_MM_PAGE_PROG_WITH_ERASE 0x86
#define MM_PAGE_TO_B1_XFER 0x53
#define MM_PAGE_TO_B2_XFER 0x55
#define PAGE_ERASE 0x81   // 512/528 bytes per page
#define SECTOR_ERASE 0x7C // 128k bytes per sector
#define READ_STATE_REGISTER 0xD7
#define Read_ID 0x9F

#define AT45DBXX_Enable 	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
#define AT45DBXX_Disable 	GPIO_SetBits(GPIOA,GPIO_Pin_4);

void AT45DBXX_Init(void);
static void AT45DBXX_BUSY(void);
void AT45DBXX_Read_ID(u8 *Data);
void write_buffer(u8 BufferOffset,u8 Data);
u8 read_buffer(u8 BufferOffset);

#endif /*__SPI_H*/
