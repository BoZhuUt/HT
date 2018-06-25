/**
  ******************************************************************************
  * @file    flash.c
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

/* Includes -------------------------------------------------------------------*/

#include "flash.h"
#include "products.h"
#include "string.h"
#include "app.h"
#include <stdio.h>

/* Private_Macros -------------------------------------------------------------*/

//待存储的寄存器类型数
#define REG_TYPE_NUM	6

/* Private_TypesDefinitions ---------------------------------------------------*/

typedef struct
{
	//指向modbus相关寄存器的指针
	void *p_reg;
	
	//一个类型的寄存器结构体中所含寄存器的个数
	uint8_t reg_num;
	
}RegStoreType;

//存入待存储寄存器的首地址和寄存器个数
RegStoreType regs_sturct[REG_TYPE_NUM]=
{
	{&system_status,SYSREG_NREGS},
	{&comm_settings,COMSREG_NREGS},	
	{&measure_settings,MSREG_NREGS},
	{&calib_settings,CALSREG_NREGS},
	{&filter_settings,FSREG_NREGS},
	//{&measure_values,MVREG_NREGS},
	{&sensor_param,PHREG_NREGS}  
};

/* Private_Variables ----------------------------------------------------------*/

/* Private_Functions ----------------------------------------------------------*/

/**
  * @brief  读取stm32内部flash数据（半字）
  * @param  地址
  * @retval 读出的数据
  */
uint16_t STMFLASH_ReadHalfWord(uint32_t rd_addr)
{
	return *( (__IO uint16_t *)rd_addr );
}

/**
  * @brief  检查是否需要擦除
  * @param  地址
  * @retval 执行状态
  */
static FLASH_Status STMFLASH_Erase(uint32_t e_addr)
{
	uint16_t i,tmp;
	FLASH_Status status=FLASH_COMPLETE;
	
	//flash解锁
	FLASH_Unlock();
	
	//检查扇区是否已被擦除
	for(i=0;i < (PAGE_SIZE/2);i++)
	{
		tmp = *( (uint16_t*)(REG_STORAGE_ADDR + i*2));
		if(tmp != 0xffff)
			break;
	}
	
	//扇区未擦除，执行擦除操作
	if(i<PAGE_SIZE)
	{
		status=FLASH_ErasePage(REG_STORAGE_ADDR);
// 		/////1us   ---zb
 		delay_us(2);
		if(status!=FLASH_COMPLETE)
			return status;
	}
	
	//flash上锁
	FLASH_Lock();
	
	return status;
}

/**
  * @brief  保存数据到stm32内部flash
  * @param  pdata-指向待保存数据的指针，w_addr-起始地址，num_to_write-半字的个数
  * @retval 执行状态
  */
static FLASH_Status STMFLASH_Write(void *pdata,uint8_t num_to_write)
{
	static uint16_t cnt=0;
	uint16_t i,*pd=pdata;
	uint32_t w_addr;
	FLASH_Status status=FLASH_COMPLETE;
	
	//flash解锁
	FLASH_Unlock();
	
	//结构体存储起始地址
	w_addr=REG_STORAGE_ADDR+cnt;	
	
	//写入数据
	for(i=0;i<num_to_write && status==FLASH_COMPLETE;i++)
	{
		status=FLASH_ProgramHalfWord(w_addr,*pd);		
		w_addr += 2;
		pd++;
	}		
	
	//下一个结构体的存储起始地址
	cnt += BYTE_OF_PER_REG;
	//zb  消除偏移
	if(cnt == REG_TYPE_NUM*BYTE_OF_PER_REG)///7*100
	{
		cnt=0;
	}
	
	//flash上锁
	FLASH_Lock();
	
	return status;
}

/**
  * @brief  存储modbus寄存器
  * @param  无
  * @retval 执行状态
  */
FLASH_Status StoreModbusReg(void)
{
	u8 i;
	FLASH_Status s_status;
	
	//擦除flash
	s_status=STMFLASH_Erase(REG_STORAGE_ADDR);
	if(s_status != FLASH_COMPLETE)
		return s_status;
	
	//保存寄存器到flash
	for( i=0 ; i < REG_TYPE_NUM ; i++ )
	{
		s_status=STMFLASH_Write(regs_sturct[i].p_reg , regs_sturct[i].reg_num);
		if(s_status != FLASH_COMPLETE)
			return s_status;
	}
	
	return s_status;
}

/**
  * @brief  从内部flash恢复modbus寄存器数据
  * @param  无
  * @retval 无
  */
void RestoreModbusReg(void)
{	
	system_status= *((__IO SYS_STATUS_T *)SYS_STATUS_SADDR);
	
	comm_settings= *((__IO COMM_SETTINGS_T *)COM_SET_SADDR);
	
	measure_settings= *((__IO MEASURE_SETTINGS_T *)MEASURE_SET_SADDR);
	
	calib_settings= *((__IO CALIB_SETTINGS_T *)CAL_SET_SADDR);
	
	filter_settings= *((__IO FILTER_SETTINGS_T *)FILTER_SET_SADDR);
	
	sensor_param= *((__IO SENSOR_PARAM_T *)CONDUCT_PARA_SADDR);
	
	if(system_status.configStatus!=1)
	{
		SENSOR_MeasureParameterReset();	
	}
	
	system_status.calibStatus=0;
	calib_settings.calibCommand=0;

	memset(system_status.hardwareVer,0,sizeof(system_status.hardwareVer));
	memset(system_status.softwareVer,0,sizeof(system_status.softwareVer));
	strcpy(system_status.hardwareVer,HW_VERSION);
	strcpy(system_status.softwareVer,SW_VERSION);
}

/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/
