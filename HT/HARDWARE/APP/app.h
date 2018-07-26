#ifndef __APP_H

#define __APP_H
/* Includes -------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "port.h"
#include "mbport.h"
#include "mb.h"
#include "regmap.h"
#include "flash.h"

#define HW_VERSION  "1.0"
#define SW_VERSION  "1.2"

#define MODBUS_PARITY_NONE 	0
#define MODBUS_PARITY_ODD 	1
#define MODBUS_PARITY_EVEN 	2

/*-------------------功能轮询命令宏-------------------*/
#define CMD_SAVE_FACTORY_PARAM  6  //备份出厂参数
#define CMD_DI_CALIB	4		  //DI校准
#define CMD_SLOPE_CALIB	5		//斜率校准
#define CMD_PARA_RESET	3		//参数复位
#define CMD_JUMP_BOOTLOADER  2  //进入BootLoader
#define CMD_NONE      0    //NONE

#define GROUND_ERR	0x01

#define S365CHANNEL ((uint8_t)0x04)
#define S420CHANNEL ((uint8_t)0x05)
#define T365CHANNEL ((uint8_t)0x06)
#define T420CHANNEL ((uint8_t)0x07)

extern u8 bitmodbus;
extern u8 isMeasureFlg;

//系统相关寄存器声明
extern SYS_STATUS_T system_status;

extern COMM_SETTINGS_T comm_settings;

extern MEASURE_SETTINGS_T measure_settings;

extern CALIB_SETTINGS_T calib_settings;

extern USER_INFO_T user_info;

extern MEASURE_VALUES_T measure_values;

extern SENSOR_PARAM_T sensor_param;

void FunctionPoll(void);
void SENSOR_MeasureParameterReset(void);
void Rset(void);
void TIM2_MeasureInit(void);
void TIM1_ModpollInit(void);

#endif
