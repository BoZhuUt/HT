#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "stm32f10x.h"
#include "delay.h"

#define  LED_SW1_H  GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define  LED_SW1_L  GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define  EN_H  GPIO_SetBits(GPIOA,GPIO_Pin_9)
#define  EN_L  GPIO_ResetBits(GPIOA,GPIO_Pin_9)

#define  LED_SW2_H  GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define  LED_SW2_L  GPIO_ResetBits(GPIOB,GPIO_Pin_15)

void switch_GPIOInit(void);
void turnLed1(void);
void turnOffLeds(void);
void turnLed2(void);

#endif
