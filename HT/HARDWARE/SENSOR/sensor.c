#include "sensor.h"
#include "app.h"
#include "PGA113.h"
#include "ad5060.h"
u16 dark[4]={1};
u8	GAIN=0xA0;
 void measure(void)
{
	u8 j=0;
	u16 CS365;
	float fluorescein;
  /*DARK*/
	CofigAD(S365);
	configPGA113(ch1,sensor_param.ct365);
	delay_ms(TIME_DELAY/2);
	ADS1120_ReadChannel(S365,10);
	/*S365*/
	sensor_param.darks365=adcResults[S365];
	write_to_LTC2630ISC6(0X30,sensor_param.cs365);
	turnOnLed2();
	delay_ms(TIME_DELAY/2);
	 ADS1120_ReadChannel(S365,50);
	turnOffLeds();
	if(adcResults[S365]>dark[S365])
	{
		sensor_param.s365=adcResults[S365]-dark[S365];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
     sensor_param.s365=adcResults[S365]-dark[S365];
	}
	turnOffLeds();
	fluorescein=sensor_param.slope*((float)sensor_param.s365/(float)sensor_param.s365di-1.0);
	if(fluorescein<0) fluorescein=0;
	measure_values.sensorValue=measure_settings.smoothingFactor*fluorescein+(1-measure_settings.smoothingFactor)*measure_values.sensorValue;
	measure_values.sensorValue_mA=measure_values.sensorValue/sensor_param.mARange*16+4;
}
u8 slopeCalib(float solution)
{
	float S365,slope;
	measure();
	S365=(float)sensor_param.s365/(float)sensor_param.s365di;
	slope=solution /(S365-1.0);
	sensor_param.slope=slope;
	return 0;
}
