/**
  ******************************************************************************
  * @file    color.c
  * @author  
  * @version V1.0
  * @date    2018-06-14
  * @brief   Colorimetric measurement
  ******************************************************************************
  * @attention
  *  
  ******************************************************************************
  */ 
#include "color.h"
#include "app.h"

void measurePD1(u8 channel)
{
	float dark;
	turnOffLeds();
	dark=Get_Adc_Average(channel,100);
}

 void measureUrea(void)
{
	float darks365f,adcf,s365f;
	
	turnOffLeds();
	delay_ms(15);
	darks365f=ADC_ReadChannel(T420CHANNEL,100);
	filter_settings.darks365=darks365f*10;
	
	write_to_LTC2630ISC6(0X30,filter_settings.cs365);
	turnLed2();
	delay_ms(15);
	adcf=ADC_ReadChannel(T420CHANNEL,200);
	turnOffLeds();
	
	if(adcf>darks365f)
	{
		s365f=adcf-darks365f;
		filter_settings.s365=s365f*10;
		filter_settings.errorCode&=~GROUND_ERR;
	}
	else
	{
		filter_settings.s365=filter_settings.s365di;
		filter_settings.errorCode|=GROUND_ERR;
	}
}
/* ADC_ReadChannel的采样次数不可以超过DMA接收数组的长度 */
void measureClF(void)
{
	float darks365f,adcf,s365f;
	
	turnOffLeds();
	delay_ms(15);
	darks365f=ADC_ReadChannel(T365CHANNEL,100);
	filter_settings.darks365=darks365f*10;
	
	write_to_LTC2630ISC6(0X30,filter_settings.cs365);
	turnLed1();
	delay_ms(15);
	adcf=ADC_ReadChannel(T365CHANNEL,200);
	turnOffLeds();
	
	if(adcf>darks365f)
	{
		s365f=adcf-darks365f;
		filter_settings.s365=s365f*10;
		filter_settings.errorCode&=~GROUND_ERR;
	}
	else
	{
		filter_settings.s365=filter_settings.s365di;
		filter_settings.errorCode|=GROUND_ERR;
	}
}
