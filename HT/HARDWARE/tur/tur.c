#include "tur.h"

u8 isFirstValue=1;
float lastNTU=0;
float s365F=1.0;

/* ADC_ReadChannel的采样次数不可以超过DMA接收数组的长度 */
void measureTurb(void)
{
	float ntu,S365,darks365F,adcF;
	
	turnOffLeds();
	delay_ms(15);
	darks365F=ADC_ReadChannel(S365CHANNEL,100);
	filter_settings.darks365=darks365F*10;
	
	write_to_LTC2630ISC6(0X30,filter_settings.cs365);
	turnLed1();
	delay_ms(15);
	adcF=ADC_ReadChannel(S365CHANNEL,200);
	//turnOffLeds();
	
	if(adcF>darks365F)
	{
		s365F=adcF-darks365F;
		filter_settings.s365=s365F*10;
		filter_settings.errorCode&=~GROUND_ERR;
	}
	else
	{
		filter_settings.s365=filter_settings.s365di;
		filter_settings.errorCode|=GROUND_ERR;
	}
	S365=s365F*10/((float)filter_settings.s365di);
	ntu=filter_settings.slope*(S365-1.0);
	if(isFirstValue==1)
	{
		isFirstValue=0;
	}
	else
	{
		ntu=measure_settings.smoothingFactor*ntu+(1.0-measure_settings.smoothingFactor)*lastNTU;
	}
	lastNTU=ntu;
	if(ntu<0) ntu=0;
	measure_values.sensorValue=ntu;
}

float Calib_S365(void)
{
	float darkC,adcC,s365C;
	
	turnOffLeds();
	delay_ms(15);
	darkC=ADC_ReadChannel(S365CHANNEL,100);
	filter_settings.darks365=darkC*10;
	
	write_to_LTC2630ISC6(0X30,filter_settings.cs365);
	turnLed1();
	delay_ms(15);
	adcC=ADC_ReadChannel(S365CHANNEL,200);
	//turnOffLeds();
	
	if(adcC>darkC)
	{
		s365C=adcC-darkC;
		filter_settings.s365=s365C*10;
		system_status.calibStatus&=~GROUND_ERR;
	}
	else
	{
		s365C=1.0;
		filter_settings.s365=filter_settings.s365di;
		system_status.calibStatus|=GROUND_ERR;
	}
	return s365C;
}
