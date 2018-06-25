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
#include "LTC2630ISC6.h"

void LTC2630ISC6_GPIO_INIT()
{
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_14;   //DAC_CS
	GPIO_Init(GPIOB,&g);
	
	g.GPIO_Pin=GPIO_Pin_13;   //SPI_CLOCK_BB
	GPIO_Init(GPIOB,&g);
	
	g.GPIO_Pin=GPIO_Pin_12;   //SPI_SLAVE_IN_BB
	GPIO_Init(GPIOB,&g);
}

void dacWriteByte(u8 byte)
{
	u8 i;
	SPI_CLOCK_LOW;
	for (i=0;i<8;i++)
	{
		if (byte&(0x80>>i))
		{
			SPI_SLAVE_IN_BB_HIGH;
		} 
		else
		{
			SPI_SLAVE_IN_BB_LOW;
		}
		SPI_CLOCK_HIGH;
		delay_us(5);
		SPI_CLOCK_LOW;
	}
}

void write_to_LTC2630ISC6(u8 Cmd, u16 Dat)
{
	if(Dat>4095)
	{
		Dat=4095;
	}
	Dat<<=4;
	DAC_CS_LOW;
	dacWriteByte(Cmd);
	dacWriteByte(Dat>>8);
	dacWriteByte(Dat|0x00ff);
	DAC_CS_HIGH;
}

void LTC2630ISC6_Init(void)
{
	 LTC2630ISC6_GPIO_INIT();
	 write_to_LTC2630ISC6(0X60,0); //select internal reference(power-on reset default)
	 write_to_LTC2630ISC6(0X30,1000); //write to and update(power up)DAC register, 1000/4095*2.5=0.61v
}
