/**
  ******************************************************************************
  * @file    embreg.h
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

#ifndef __EMBREG_H

#define __EMBREG_H

/* Includes -------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "mb.h"
#include "mbutils.h"
#include "port.h"
#include "regmap.h"
#include "app.h"
extern  u8 ModbusStore;
/* Exported_Macros ------------------------------------------------------------*/

/*----------------------系统相关寄存器宏--------------------*/
//系统状态寄存器地址标志
#define SYSREG_FLAG		41	
//系统状态寄存器起始地址
#define SYSREG_SADDR	(SYSREG_FLAG * 1000 +1)
//系统状态寄存器个数
#define SYSREG_NREGS	(sizeof(SYS_STATUS_T)>>1)

//通信设置寄存器地址标志
#define COMSREG_FLAG	42
//通信设置寄存器起始地址
#define COMSREG_SADDR	(COMSREG_FLAG * 1000 + 1)
//通信设置寄存器个数
#define COMSREG_NREGS	(sizeof(COMM_SETTINGS_T)>>1)

//测量设置寄存器地址标志
#define MSREG_FLAG		43
//测量设置寄存器起始地址
#define MSREG_SADDR		(MSREG_FLAG * 1000 + 1)	
//测量设置寄存器个数
#define MSREG_NREGS		(sizeof(MEASURE_SETTINGS_T)>>1)

//校准设置寄存器地址标志
#define CALSREG_FLAG	44	
//校准设置寄存器起始地址
#define CALSREG_SADDR	(CALSREG_FLAG * 1000 + 1)		
//校准设置寄存器个数
#define CALSREG_NREGS	(sizeof(CALIB_SETTINGS_T)>>1)

//滤波器设置寄存器地址标志
#define UIREG_FLAG		45	
//滤波器设置寄存器地址标志
#define UIREG_SADDR		(UIREG_FLAG * 1000 + 1)		
//滤波器设置寄存器个数
#define UIREG_NREGS		(sizeof(USER_INFO_T)>>1)
	
//测量值寄存器地址标志
#define MVREG_FLAG		46	
//测量值寄存器起始地址
#define MVREG_SADDR		(MVREG_FLAG * 1000 + 1)			
//测量值寄存器个数
#define MVREG_NREGS		(sizeof(MEASURE_VALUES_T)>>1)
	
//pH\ORP专有寄存器地址标志
#define PHREG_FLAG		48		
//pH\ORP专有寄存器起始地址
#define PHREG_SADDR		(PHREG_FLAG * 1000 + 1)			
//pH\ORP专有寄存器个数
#define PHREG_NREGS		(sizeof(SENSOR_PARAM_T)>>1)

/* Exported_Types -------------------------------------------------------------*/

/* Exported_Functions ---------------------------------------------------------*/

#endif
/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/
