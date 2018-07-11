#include "sensor.h"
#include "app.h"

u16 dark[4]={1};

 void measure(void)
{
	u8 j=0;
	
	turnOffLeds();
	delay_ms(TIME_DELAY);
	for(j=0;j<4;j++)
	{
		ADS1120_ReadChannel(j,20);
		dark[j]=adcResults[j];
	}
	/*********************** 365 ***********************/
	turnOnLed1();
	write_to_LTC2630ISC6(0X30,sensor_param.ct365);
	delay_ms(TIME_DELAY);
	ADS1120_ReadChannel(T365,20);
	if(adcResults[T365]>dark[T365])
	{
		sensor_param.t365=adcResults[T365]-dark[T365];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}
	
	write_to_LTC2630ISC6(0X30,sensor_param.cs365);
	delay_ms(TIME_DELAY);
	ADS1120_ReadChannel(S365,20);
	if(adcResults[S365]>dark[S365])
	{
		sensor_param.s365=adcResults[S365]-dark[S365];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}
	/************************ 420 ***********************/
	turnOnLed2();
	write_to_LTC2630ISC6(0X30,sensor_param.ct420);
	delay_ms(TIME_DELAY);
	ADS1120_ReadChannel(T420,20);
	if(adcResults[T420]>dark[T420])
	{
		sensor_param.t420=adcResults[T420]-dark[T420];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}
	
	write_to_LTC2630ISC6(0X30,sensor_param.cs420);
	delay_ms(TIME_DELAY);
	ADS1120_ReadChannel(S420,20);
	if(adcResults[S420]>dark[S420])
	{
		sensor_param.s420=adcResults[S420]-dark[S420];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}	
	turnOffLeds();
	
}
