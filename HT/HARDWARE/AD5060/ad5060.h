#ifndef __AD5060_H

#define __AD5060_H
#include "stm32f10x.h"
#include "delay.h"

#define  AD5060_CS_PIN          GPIO_Pin_13
#define  AD5060_CS_PORT         GPIOB
#define  AD5060_DIO_PIN         GPIO_Pin_14
#define  AD5060_DIO_PORT        GPIOB
#define  AD5060_CLK_PIN         GPIO_Pin_12
#define  AD5060_CLK_PORT        GPIOB

#define  AD5060_CS_H      GPIO_SetBits(AD5060_CS_PORT,AD5060_CS_PIN)
#define  AD5060_CS_L      GPIO_ResetBits(AD5060_CS_PORT,AD5060_CS_PIN)
#define  AD5060_DIO_H     GPIO_SetBits(AD5060_DIO_PORT,AD5060_DIO_PIN)
#define  AD5060_DIO_L     GPIO_ResetBits(AD5060_DIO_PORT,AD5060_DIO_PIN)
#define  AD5060_CLK_H     GPIO_SetBits(AD5060_CLK_PORT,AD5060_CLK_PIN)
#define  AD5060_CLK_L     GPIO_ResetBits(AD5060_CLK_PORT,AD5060_CLK_PIN)

void ad5060GpioInit(void);
void ad5060Out(u16 DATA);

#endif