/**
  ******************************************************************************
  * @file    LTC2630ISC6.c
  * @author  bo.zhu
  * @version V1.0
  * @date    2016-12-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  *  
  ******************************************************************************
  */ 
#include "spi.h"

void LTC2630ISC6_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=DAC_CS_PIN;   
	GPIO_Init(DAC_CS_PORT,&g);
	
	g.GPIO_Pin=SPI_CLOCK_PIN;   
	GPIO_Init(SPI_CLOCK_PORT,&g);
	
	g.GPIO_Pin=SPI_SLAVE_IN_PIN;   
	GPIO_Init(SPI_SLAVE_IN_PORT,&g);
}

void dacWriteByte(u8 byte)
{
	u8 i;
	SPI_CLOCK_L;
	for (i=0;i<8;i++)
	{
		if (byte&(0x80>>i))
		{
			SPI_SLAVE_IN_H;
		} 
		else
		{
			SPI_SLAVE_IN_L;
		}
		SPI_CLOCK_H;
		delay_us(5);
		SPI_CLOCK_L;
	}
}

void write_to_LTC2630ISC6(u8 Cmd, u16 Dat)
{
	if(Dat>4095)
	{
		Dat=4095;
	}
	Dat<<=4;
	DAC_CS_L;
	dacWriteByte(Cmd);
	dacWriteByte(Dat>>8);
	dacWriteByte(Dat|0x00ff);
	DAC_CS_H;
}

void LTC2630ISC6_Init(void)
{
	 LTC2630ISC6_GPIOInit();
	 write_to_LTC2630ISC6(0X60,0); //select internal reference(power-on reset default)
	 write_to_LTC2630ISC6(0X30,1000); //write to and update(power up)DAC register, 1000/4095*2.5=0.61v
}
