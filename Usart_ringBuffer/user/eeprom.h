#include <stm32f10x.h>
#include <stm32f10x_flash.h>
#include <stdio.h>

#define EEPROM_START_ADDRESS ((uint32_t)0x0801F800)
#define PAGE_SIZE            1

void Flash_Erase(uint32_t addr);
void Flash_Write(uint32_t addr, uint16_t data);
uint16_t Flash_Read(uint32_t addr);