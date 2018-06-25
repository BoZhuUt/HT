/**
  ******************************************************************************
  * @file    flash.h
  * @author  AndyChen
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  *
  *  
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion --------------------------------------*/

#ifndef __FLASH_H

#define __FLASH_H

/* Includes -------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#include "app.h"
#include "embreg.h"

/* Exported_Types -------------------------------------------------------------*/

/* Exported_Macros ------------------------------------------------------------*/

//flash页尺寸
#define PAGE_SIZE	1024

//modbus寄存器保存偏移地址
#define REG_STORAGE_OFFSET	(62*1024)

//modbus寄存器保存起始地址
#define REG_STORAGE_ADDR	(0x08000000+REG_STORAGE_OFFSET)

//每个结构体占用的字节数
#define BYTE_OF_PER_REG		100

//system_status存储位置起始地址
#define SYS_STATUS_SADDR		(REG_STORAGE_ADDR + 0*BYTE_OF_PER_REG)

//comm_settings存储位置起始地址
#define COM_SET_SADDR			(REG_STORAGE_ADDR + 1*BYTE_OF_PER_REG)
#define COM_SET_SADDR_OLD			(REG_STORAGE_ADDR + 1*BYTE_OF_PER_REG_OLD)

//measure_settings存储位置起始地址
#define MEASURE_SET_SADDR		(REG_STORAGE_ADDR + 2*BYTE_OF_PER_REG)
#define MEASURE_SET_SADDR_OLD		(REG_STORAGE_ADDR + 2*BYTE_OF_PER_REG_OLD)

//calib_settings存储位置起始地址
#define CAL_SET_SADDR			(REG_STORAGE_ADDR + 3*BYTE_OF_PER_REG)
#define CAL_SET_SADDR_OLD			(REG_STORAGE_ADDR + 3*BYTE_OF_PER_REG_OLD)

//filter_settings存储位置起始地址
#define FILTER_SET_SADDR		(REG_STORAGE_ADDR + 4*BYTE_OF_PER_REG)
#define FILTER_SET_SADDR_OLD		(REG_STORAGE_ADDR + 4*BYTE_OF_PER_REG_OLD)

//sensor_param存储位置起始地址
#define CONDUCT_PARA_SADDR		(REG_STORAGE_ADDR + 5*BYTE_OF_PER_REG)  //sensor_param 暂未启用

/* Exported_Functions ---------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t rd_addr);
uint32_t STMFLASH_ReadWord(uint32_t rd_addr);
float STMFLASH_ReadFloatData(uint32_t rd_addr);
FLASH_Status StoreModbusReg(void);
void RestoreModbusReg(void);

#endif
/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/
