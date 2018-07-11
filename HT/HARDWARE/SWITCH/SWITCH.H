#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "stm32f10x.h"

#define  LED_SW1_PIN   GPIO_Pin_14
#define  LED_SW1_PORT  GPIOC
#define  LED_SW2_PIN   GPIO_Pin_15
#define  LED_SW2_PORT  GPIOC

#define  LED_SW1_H     GPIO_SetBits(LED_SW1_PORT,LED_SW1_PIN)
#define  LED_SW1_L     GPIO_ResetBits(LED_SW1_PORT,LED_SW1_PIN)
#define  LED_SW2_H     GPIO_SetBits(LED_SW2_PORT,LED_SW2_PIN)
#define  LED_SW2_L     GPIO_ResetBits(LED_SW2_PORT,LED_SW2_PIN)

void switch_GPIOInit(void);
void turnOnLed1(void);
void turnOffLeds(void);
void turnOnLed2(void);

#endif
