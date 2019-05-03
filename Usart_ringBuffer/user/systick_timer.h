#ifndef __SYSTICK_H_
#define __SYSTICK_H_

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <stm32f10x.h>
/*******************************************************************************/
/* DECLARATION OF DATATYPE                                              */
/*******************************************************************************/
#ifndef BOOLEAN
#define BOOLEAN       uint8_t
#endif /* #ifndef BOOLEAN */

#ifndef TRUE
#define TRUE      1
#endif /* #ifndef BOOLEAN */

#ifndef FALSE
#define FALSE     0
#endif /* #ifndef BOOLEAN */

/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/**
  * @brief  The function inits SysTick modules
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void);
/**
  * @brief  Delay in milisecond
  * @param  None
  * @retval None
  */
 void SysTick_DelayMs(uint32_t nTime);
/**
  * @brief  Get current value from SySTick
  * @param  None
  * @retval None
  */
 void SysTick_DelayUs(uint32_t nTime);
/**
  * @brief  Get current value from SySTick
  * @param  None
  * @retval None
  */
uint32_t SysTick_GetTickCount(void);

/**
  * @brief  Start one Timeout in milisecond
  * @param  None
  * @retval None
  */
void SysTick_StartTimeout(uint32_t nTime);

/**
  * @brief  Start one timeout in milisecond
  * @param  None
  * @retval None
  */
BOOLEAN SysTick_IsReachTimeout(void);
#endif /*__SYSTICK_H_*/