#ifndef __LTC2440_H__
#define __LTC2440_H__

// #include "stm32f10x_lib.h"
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


#define SCLOCK_1   	GPIO_SetBits(GPIOB,GPIO_Pin_5);
#define SCLOCK_0   	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

#define ADC_CS_1  	GPIO_SetBits(GPIOB,GPIO_Pin_7);
#define ADC_CS_0  	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

#define SDI_1      	GPIO_SetBits(GPIOC,GPIO_Pin_15);
#define SDI_0      	GPIO_ResetBits(GPIOC,GPIO_Pin_15);

#define SDO_1     	GPIO_SetBits(GPIOB,GPIO_Pin_6);
#define SDO_0     	GPIO_ResetBits(GPIOB,GPIO_Pin_6);

void LTC2440_INIT(void);
void getCond(void);
void test(void);

void COND(void);

#endif
