#include "SWITCH.h"

void switch_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_8;   //SW1
	GPIO_Init(GPIOA,&g);
	
	g.GPIO_Pin=GPIO_Pin_9;   //EN
	GPIO_Init(GPIOA,&g);
	
	g.GPIO_Pin=GPIO_Pin_15;   //SW2
	GPIO_Init(GPIOB,&g);
}

void turnLed1(void)
{
	 EN_H;
	 LED_SW1_H;
	 LED_SW2_L;
}
void turnLed2(void)
{
	 EN_H;
	 LED_SW1_H;
	 LED_SW2_H;
}

void turnOffLeds(void)
{
	EN_L;
}
