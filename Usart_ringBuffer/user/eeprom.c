#include "eeprom.h"

/*ref https://tapit.vn/cac-thao-tac-doc-ghi-va-xoa-bo-nho-flash-tren-mcu-stm32f103c8t6/*/
 void Flash_Erase(uint32_t addr)
 {
	 FLASH->CR |= 0x00000002;
	 FLASH->AR |= addr;
	 FLASH->CR |= 0x00000004;
	 while(FLASH->SR & 0x00000001);
	 FLASH->CR &= ~(0x00000042); 
 }

 
void Flash_Write(uint32_t addr, uint16_t data)
{
	//Unlock flash before write//
	FLASH_Unlock();
	
	//Flash erase //
	Flash_Erase(addr);
	
	//delay //
	//int i=1000000;
//	for(i=1000000; i>0; i--)
	
	//write flash//
	FLASH->CR |= 0x00000001;   /*< programing */
	*(uint16_t*) addr = data;
	while(FLASH->SR & 0x00000042);
	
	// flash lock//
	FLASH_Lock();
}


uint16_t Flash_Read(uint32_t addr)
{
	uint16_t* val = (uint16_t *) addr;
	return *val;
}