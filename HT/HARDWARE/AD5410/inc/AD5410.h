
#ifndef __AD5410_H__
#define __AD5410_H__

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

#define  DA_latch_high GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define  DA_latch_low  GPIO_ResetBits(GPIOB,GPIO_Pin_11)

#define DA_CLK_HIGH    GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define DA_CLK_LOW     GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define DA_SIMO_HIGH   GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define DA_SIMO_LOW    GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define DA_CLEAR_HIGH   GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define DA_CLEAR_LOW    GPIO_ResetBits(GPIOB,GPIO_Pin_10)


void AD5410_init(void);
void AD5410_IOUT(float DATA);   //0<=DATA<=20
void TEST(unsigned char addr);
void ReadDataFromAD5410(unsigned char addr);

#endif
