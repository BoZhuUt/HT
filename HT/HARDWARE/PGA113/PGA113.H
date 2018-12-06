#ifndef __PGA113_H__
#define __PGA113_H__

#include "stm32f10x.h"

#define  PGA113_CS_PIN          GPIO_Pin_9
#define  PGA113_CS_PORT         GPIOA
#define  PGA113_DIO_PIN         GPIO_Pin_8
#define  PGA113_DIO_PORT        GPIOA
#define  PGA113_CLK_PIN         GPIO_Pin_15
#define  PGA113_CLK_PORT        GPIOB

#define  PGA113_CS_H      GPIO_SetBits(PGA113_CS_PORT,PGA113_CS_PIN)
#define  PGA113_CS_L      GPIO_ResetBits(PGA113_CS_PORT,PGA113_CS_PIN)
#define  PGA113_DIO_H     GPIO_SetBits(PGA113_DIO_PORT,PGA113_DIO_PIN)
#define  PGA113_DIO_L     GPIO_ResetBits(PGA113_DIO_PORT,PGA113_DIO_PIN)
#define  PGA113_CLK_H     GPIO_SetBits(PGA113_CLK_PORT,PGA113_CLK_PIN)
#define  PGA113_CLK_L     GPIO_ResetBits(PGA113_CLK_PORT,PGA113_CLK_PIN)

#define ch0 0x00
#define ch1 0x01

#define pgaGain1     0x00
#define pgaGain2     0x10
#define pgaGain4     0x20
#define pgaGain8     0x30
#define pgaGain16    0x40
#define pgaGain32    0x50
#define pgaGain64    0x60
#define pgaGain128   0x70

void PGA113_GPIOInit(void);
void configPGA113(u8 ch,u8 gain);

#endif