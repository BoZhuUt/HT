#include "switch.h"
#include "spi.h"

void turnOffLeds(void);
void switch_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=LED_SW1_PIN | LED_SW2_PIN;   
	GPIO_Init(LED_SW1_PORT,&g);
	
	turnOffLeds();
}

void turnOnLed1(void)
{
	 LED_SW1_L;
	 LED_SW2_H;
}
void turnOnLed2(void)
{
	 LED_SW1_H;
	 LED_SW2_H;
}

void turnOffLeds(void)
{
	 LED_SW1_L;
	 LED_SW2_L;
	 write_to_LTC2630ISC6(0X30,0);
}
