#include "sensor.h"
#include "app.h"

u16 dark[4]={1};

 void measure(void)
{
	u8 j=0;
	float fluorescein;
	turnOffLeds();
	delay_ms(TIME_DELAY);
	for(j=0;j<4;j++)
	{
		ADS1120_ReadChannel(j,20);
		//ads1120ReadChannel(j,20);
		dark[j]=adcResults[j];
	}
	turnOnLed2();
	sensor_param.darks365=dark[S365];
	write_to_LTC2630ISC6(0X30,sensor_param.cs365);
	delay_ms(TIME_DELAY);
	ADS1120_ReadChannel(S365,50);
	if(adcResults[S365]>dark[S365])
	{
		sensor_param.s365=adcResults[S365]-dark[S365];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}
//	write_to_LTC2630ISC6(0X30,sensor_param.cs420);
//	delay_ms(10);
	ADS1120_ReadChannel(S420,50);
	if(adcResults[S420]>dark[S420])
	{
		sensor_param.s420=adcResults[S420]-dark[S420];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
	}	
	turnOffLeds();
	fluorescein=sensor_param.slope*((float)sensor_param.s365/(float)sensor_param.s365di-1.0)/((float)sensor_param.s420/(float)sensor_param.s420di);
	if(fluorescein<0) fluorescein=0;
	measure_values.sensorValue=measure_settings.smoothingFactor*fluorescein+(1-measure_settings.smoothingFactor)*measure_values.sensorValue;
	measure_values.sensorValue_mA=fluorescein/sensor_param.mARange*16+4;
}

u8 slopeCalib(float solution)
{
	float S365,T420,slope,intercept;
	//measure();
	S365=(float)sensor_param.s365/(float)sensor_param.s365di;
	T420=(float)sensor_param.s420/(float)sensor_param.s420di;
	slope=solution /(S365-1.0)*T420;
	sensor_param.slope=slope;
	return 0;
}
