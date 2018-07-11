#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"
#include "delay.h"

/********************* LTC2630 CS ***********************/
#define DAC_CS_PIN          GPIO_Pin_13
#define DAC_CS_PORT         GPIOC
/********************* ADS1120 CS DRDY ***********************/
#define ADS1120_CS_PIN      GPIO_Pin_1
#define ADC1120_CS_PORT     GPIOB
#define ADS1120_DRDY_PIN    GPIO_Pin_2
#define ADS1120_DRDY_PORT   GPIOB

#define SPI_CLOCK_PIN       GPIO_Pin_0
#define SPI_CLOCK_PORT      GPIOB
#define SPI_SLAVE_IN_PIN    GPIO_Pin_11
#define SPI_SLAVE_IN_PORT   GPIOB
#define SPI_SLAVE_OUT_PIN   GPIO_Pin_10
#define SPI_SLAVE_OUT_PORT  GPIOB

#define DAC_CS_H              GPIO_SetBits(DAC_CS_PORT,DAC_CS_PIN)
#define DAC_CS_L              GPIO_ResetBits(DAC_CS_PORT,DAC_CS_PIN)

#define ADS1120_CS_H          GPIO_SetBits(ADC1120_CS_PORT,ADS1120_CS_PIN)
#define ADS1120_CS_L          GPIO_ResetBits(ADC1120_CS_PORT,ADS1120_CS_PIN)
#define READ_ADS1120_DRDY     GPIO_ReadInputDataBit(ADS1120_DRDY_PORT,ADS1120_DRDY_PIN)
#define READ_SPI_SLAVE_OUT    GPIO_ReadInputDataBit(SPI_SLAVE_OUT_PORT,SPI_SLAVE_OUT_PIN)

#define SPI_SLAVE_IN_H        GPIO_SetBits(SPI_SLAVE_IN_PORT,SPI_SLAVE_IN_PIN)
#define SPI_SLAVE_IN_L        GPIO_ResetBits(SPI_SLAVE_IN_PORT,SPI_SLAVE_IN_PIN)

#define SPI_CLOCK_H           GPIO_SetBits(SPI_CLOCK_PORT,SPI_CLOCK_PIN)
#define SPI_CLOCK_L           GPIO_ResetBits(SPI_CLOCK_PORT,SPI_CLOCK_PIN)

#define LTC2630ISC6_POWER_DOWN_ 0X70
#define LTC2630ISC6_WRITE_TO_AND_UPDATE 0X30

/********************* ADS1120 CMD ***********************/
#define RESET      0X06
#define START      0X08
#define POWERDOWN  0X02
#define RDATA      0X10
#define RREG       0X20
#define WREG       0X40

enum ADC_CHANNEL
{
	T420=0,
	S365=1,
	T365=2,
	S420=3
};

extern u16 adcResults[4];

void LTC2630ISC6_Init(void );
void write_to_LTC2630ISC6(u8 Cmd, u16 Dat);

void ADS1120_Init(void);
void ADS1120_ReadChannel(u8 channel,u16 times);

#endif
