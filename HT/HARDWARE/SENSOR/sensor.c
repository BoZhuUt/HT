#include "sensor.h"
#include "app.h"
#include "PGA113.h"
#include "ad5060.h"
u16 dark[4]={1};
u8	GAIN=0x40;
// void measure(void)
//{
//	u8 j=0;
//	u16 CS365;
//	float fluorescein;
//	GAIN=0x94;
//	turnOnLed2();
////	write_to_LTC2630ISC6(0X30,sensor_param.cs365);
////	delay_ms(TIME_DELAY*2);
////	ADS1120_ReadChannel(S365,10);
////	sensor_param.t365=adcResults[S365];
////	if(adcResults[S365]>=33000)
////	{
////		GAIN=0x90;
////		CS365=sensor_param.ct365;
////	} 
////	else
////	{
////		CS365=sensor_param.cs365;
////	}
////	turnOffLeds();
//	delay_ms(TIME_DELAY);
//	for(j=0;j<4;j++)
//	{
//		ADS1120_ReadChannel(j,20);
//		//ads1120ReadChannel(j,20);
//		dark[j]=adcResults[j];
//	}
//	turnOnLed2();
//	sensor_param.darks365=dark[S365];
//	write_to_LTC2630ISC6(0X30,CS365);
//	delay_ms(TIME_DELAY);
//	ADS1120_ReadChannel(S365,50);
//	if(adcResults[S365]>dark[S365])
//	{
//		sensor_param.s365=adcResults[S365]-dark[S365];
//	}
//	else
//	{
//		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
//	}
////	write_to_LTC2630ISC6(0X30,sensor_param.cs420);
////	delay_ms(10);
//    write_to_LTC2630ISC6(0x30,sensor_param.ct420);
//	ADS1120_ReadChannel(T420,50);
//	if(adcResults[S420]>dark[S420])
//	{
//		sensor_param.s420=adcResults[S420]-dark[S420];
//	}
//	else
//	{
//		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
//	}
//    turnOnLed1();
//    delay_ms(500);
//	turnOffLeds();
//	fluorescein=sensor_param.slope*((float)sensor_param.s365/(float)sensor_param.s365di-1.0)/((float)sensor_param.s420/(float)sensor_param.s420di);
//	if(fluorescein<0) fluorescein=0;
//	measure_values.sensorValue=measure_settings.smoothingFactor*fluorescein+(1-measure_settings.smoothingFactor)*measure_values.sensorValue;
//	measure_values.sensorValue_mA=fluorescein/sensor_param.mARange*16+4;
//}
 void measure(void)
{
	u8 j=0;
	u16 CS365;
	float fluorescein;
    if(sensor_param.cs365Factory==1)
        GAIN=0X91;
    else if(sensor_param.cs365Factory==2)
        GAIN=0X93;
    else if(sensor_param.cs365Factory==4)
        GAIN=0X95;
    else if(sensor_param.cs365Factory==8)
        GAIN=0X96;
    else if(sensor_param.cs365Factory==16)
        GAIN=0X98;
    else if(sensor_param.cs365Factory==32)
        GAIN=0X9A;
    else if(sensor_param.cs365Factory==64)
        GAIN=0X9C;
    else if(sensor_param.cs365Factory==128)
        GAIN=0X9E;
    else
        GAIN=0X90;
    
	turnOffLeds();
	delay_ms(TIME_DELAY);
    /*DARK*/
    ad5060Out(sensor_param.t365di);
    configPGA113(ch0,sensor_param.ct365);
    delay_ms(TIME_DELAY/2);
    ADS1120_ReadChannel(S365,20);
    dark[S365]=adcResults[S365];
    sensor_param.t365=dark[S365];
//    ad5060Out(sensor_param.s420di);
//    configPGA113(ch1,sensor_param.cs420);
//    delay_ms(TIME_DELAY/2);
//    ADS1120_ReadChannel(T420,20);
//    dark[T420]=adcResults[T420];
//    sensor_param.s420=dark[T420];
    /*S365*/
	sensor_param.darks365=dark[S365];
	write_to_LTC2630ISC6(0X30,sensor_param.cs365);
    turnOnLed2();
    ad5060Out(sensor_param.t365di);
    configPGA113(ch0,sensor_param.ct365);
    delay_ms(TIME_DELAY/2);
	ADS1120_ReadChannel(S365,50);
	if(adcResults[S365]>dark[S365])
	{
		sensor_param.s365=adcResults[S365]-dark[S365];
	}
	else
	{
		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
        sensor_param.s365=adcResults[S365]-dark[S365];
	}
    /*T420*/
//    dark[T420]=0;
//    write_to_LTC2630ISC6(0x30,sensor_param.ct420);
//    turnOnLed1();
//    ad5060Out(sensor_param.s420di);
//    configPGA113(ch1,sensor_param.cs420);
//    delay_ms(TIME_DELAY/2);
//	ADS1120_ReadChannel(T420,50);
//	if(adcResults[T420]>dark[T420])
//	{
//		sensor_param.t420=adcResults[T420]-dark[T420];
//	}
//	else
//	{
//        sensor_param.t420=adcResults[T420]-dark[T420];
//		sensor_param.errorCode|=ERR_SINGNAL_TOO_LOW;
//	}
	turnOffLeds();
//	fluorescein=sensor_param.slope*((float)sensor_param.s365/(float)sensor_param.s365di-1.0)/((float)sensor_param.s420/(float)sensor_param.s420di);
//	if(fluorescein<0) fluorescein=0;
//	measure_values.sensorValue=measure_settings.smoothingFactor*fluorescein+(1-measure_settings.smoothingFactor)*measure_values.sensorValue;
//	measure_values.sensorValue_mA=fluorescein/sensor_param.mARange*16+4;
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
