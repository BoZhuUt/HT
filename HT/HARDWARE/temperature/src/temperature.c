#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "temperature.h"
// #include "conductivity.h"
// #include "Color.h"
// #include "flash.h"
#include "delay.h"
#include "app.h"
// #include "utils.h"

#define	CUBIC_CURVE_FIT
#define	TEMP_DEBUG

#define	TEMP_JITTER_MIN_THRESHOLD	(0.3)
#define	TEMP_JITTER_MAX_THRESHOLD	(25.0)

float gTempF=-4.0;
float gSavedTempF=-4.0;
//u8    gFirstTemp=1
// bool  gFirstTemp=true;
u8    DataRead[3];

float gRSample=1000;
float gRSampleDefault=969.8831;
//float gRSampleDefault=3198.6;

//const float gRSample=980.75;
//const float gRSample=3198.6;
//const float gTempK=1.0554;
//const float gTempB=-2.236;

void delayLoop(unsigned int Time)
{
		Time*=3;
		while(Time--)
		{
			__nop();
		}
}

void temperatureGpioInit()
{
//	SPI_DCLK 	PB4; outpp
//	SPI_DOUT 	PB5; 	inipu
//	SPI_DIN 	PC15;	outpp
//	ADC_CS 		PA0		outpp
	
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_5;//DCLK(SPI CLOCK_BB),pb5
	GPIO_Init(GPIOB,&g);
	g.GPIO_Pin=GPIO_Pin_0;//CS(ADC_CSB),pa0
	GPIO_Init(GPIOA,&g);
	g.GPIO_Pin=GPIO_Pin_15;//(SPI SLAVE_IN_BB),pc15
	GPIO_Init(GPIOC,&g);
	
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=GPIO_Pin_6;//DOUT(SPI SLAVE_OUT_BB) pb6
	GPIO_Init(GPIOB,&g);
	

	

 }

void WriteToReg(unsigned char ByteData) // write ByteData to the register
{
	int i;
	unsigned char temp;

	Temp_CS0;
	temp=0x80;
	for(i=0;i<8;i++)
	{
		if((temp & ByteData)==0)
		{        
			DIN0;
		}    
		else
		{
			DIN1;
		}
		SCLOCK0;
		delayLoop(10);
		SCLOCK1;
		delayLoop(10);
		temp=temp>>1;
	}
	Temp_CS1;
}
/**********************/
void Writebyte(u8 data)
{
	int i=0;

	Temp_CS0;
	for(i=0; i<8; i++)
	{
		if(data&0x80)
		{
			DIN1;
		}
		else
		{
			DIN0;
		}

		data<<=1;
		SCLOCK0;
		delayLoop(10);
		SCLOCK1;
		delayLoop(10);
	}
	Temp_CS1;
}
/**********************/
void Write2bytes(u16 data)
{
	int i=0;

	Temp_CS0;
	for(i=0; i<16; i++)
	{
		if(data&0x8000)
		{
			DIN1;
		}
		else
		{
			DIN0;
		}

		data<<=1;
		SCLOCK0;
		delayLoop(10);
		SCLOCK1;
		delayLoop(10);
	}
	Temp_CS1;
}

void ReadFromReg(unsigned char nByte) // nByte is the number of bytes which need to be read
{
	int i,j;
	unsigned char temp;
	DIN1;
	Temp_CS0;
	temp=0;

	DataRead[0]=0;
	DataRead[1]=0;
	DataRead[2]=0;
	for(i=0; i<nByte; i++)
	{
		for(j=0; j<8; j++)
		{
			SCLOCK0;
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
			{
				temp=temp<<1;
			}else
			{
				temp=temp<<1;
				temp=temp+0x01;
			}
			delayLoop(10);
			SCLOCK1;
			delayLoop(10);
		}
		DataRead[i]=temp;
		temp=0;
	}
	Temp_CS1;
}

void AD7792Init(void)
{
	int ResetTime;

	temperatureGpioInit();

	/* PRECONFIGURE...*/
	ResetTime=32;
	SCLOCK1; 
	Temp_CS0;          //to keep DIN=1 for 32 sclock to reset the part
	DIN1;
	while(ResetTime--)
	{
		delayLoop(10);
		SCLOCK0;
		delayLoop(10);
 		SCLOCK1;
	}
	Temp_CS1; 

	WriteToReg(0x00|(1<<3));// config R1
	Write2bytes(0x400a);	// idle mode, 120ms refresh rate

	WriteToReg(0x00|(2<<3));// config R2
	Write2bytes(0x1210);	// uni-polar, 4-times gain, enable input buffer

	WriteToReg(0x00|(5<<3));// config R5
	Writebyte(0x03);		// enable 1mA output
	
	WriteToReg(0x00|(1<<3));// config R1
	Write2bytes(0x800a);	// internal calibration
	delay_ms(40);
	
	WriteToReg(0x00|(1<<3));// config R1
	Write2bytes(0xa00a);	// internal full scale calibration
	delay_ms(40);

	WriteToReg(0x00|(1<<3));// config R1
	Write2bytes(0x000a);	// start contiguous conversion
	delay_ms(40);
}


#define tempBufLenth 3
u16 tempBuf[tempBufLenth]={0};
 void temperatureInit(void)
{     
    int i=0;

    for(;i<tempBufLenth;i++)
    {
    	tempBuf[i]=2268;
    }
	AD7792Init();
}

float tempreadRt(void)
{
    int i;
	u16 temp;
	u16 tempMax=0;
	u16 tempMin=65535;
	u32 tempAverage=0;

	for (i=0; i<10; i++)
	{
		temp = 0;

	//	__disable_irq();
		WriteToReg(0x00|(1<<3));//config R1
		Write2bytes(0x200a);

		WriteToReg(0x40);//write to Communication register.The next step is to read from Status register.
		ReadFromReg(1);
		while((DataRead[0]&0x80)==0x80)//wait for the end of conversion by polling the status register RDY bit
		{
			WriteToReg(0x40);
			ReadFromReg(1);
		}

		WriteToReg(0x58);//write to Communication register.The next step is to read from Data register.
		ReadFromReg(2);
		//__enable_irq();

		temp=DataRead[0];
		temp<<=8;
		temp+=DataRead[1];

		if (temp > tempMax)
			tempMax = temp;
		if (temp < tempMin)
			tempMin = temp;

		tempAverage += temp;
	}

	tempAverage -= (tempMax+tempMin);
	tempAverage /= 8;
	temp = (u16)tempAverage;
	return temp*gRSample/65535;
}

float TemperC=0;
float Rtemp=0;

float temp_R0=0;
float temp_RTDA=0;

 float temperatureRead(void)
{
	/************************************************************************
	 Now after delaying 1000 mS in the tskCond.c, we measure temperature
	 once a second. Instead of averaging a low data rate of both temperature
	 and conductivity, we average the "raw" readings. For each display
	 update in every second, we measure 40 conductivity readings and 10
	 temperature readings. We could take more temperature readings.
	*********************************************************************/
	int i;
	u32 temp;	
	float tempF=0;
	u16 tempMax=0;
	u16 tempMin=65535;
	u32 tempAverage=0;

	
// 	gRSample = gSysParam.temp_Rx;//暂时注释，没找到gSysParam的定义-zb

	// for backward compatibility
// 	if ( (*ptr==0x00) && (*(ptr+1)==0x00) && (*(ptr+2)==0x00) && (*(ptr+3)==0x00) )
// 	{
// 		gRSample = gRSampleDefault;
// 	}

	for (i=0; i<10; i++)
	{
		temp = 0;

		__disable_irq();


		WriteToReg(0x40);//write to Communication register.The next step is to read from Status register.
		ReadFromReg(1);//读取状态寄存器

		WriteToReg(0x58);//write to Communication register.The next step is to read from Data register.
		ReadFromReg(2);//读取结果寄存器
		__enable_irq();

		temp=DataRead[0];
		temp<<=8;
		temp+=DataRead[1];
            
		if (temp > tempMax)
			tempMax = temp;
		if (temp < tempMin)
			tempMin = temp;

		tempAverage += temp;
	}

	tempAverage -= (tempMax+tempMin);
	tempAverage /= 8;
	temp = (u16)tempAverage;
      
	Rtemp=((float)temp/4.0)*(float)1000/(float)65535;

	/***************************************************
	 * A cubic fit over the range of -100癈 to +600癈
	 * provides an RMS error of only 0.038癈 over the
	 * entire range, and 0.026癈 in the range of
	 * 0癈 to 400癈
	 **************************************************/
	//TemperC = -247.29 + 2.3992*Rtemp + (6.3962E-4)*Rtemp*Rtemp + (1.0241E-6)*Rtemp*Rtemp*Rtemp;

	/*******************************************************************
	 * reference http://sensing.honeywell.com/index.php?ci_id=50025
	 ******************************************************************/
      
	if(temp_R0==0||temp_RTDA==0)	// if temp_R0 and temp_RTDA don't exist
	{
		TemperC = ( (-R0*RTD_A) + sqrt(R0*R0*RTD_A*RTD_A-4*R0*RTD_B*(R0-Rtemp)) )/(2*R0*RTD_B);
	}
	else
	{
		TemperC = ( (-temp_R0*temp_RTDA) + sqrt(temp_R0*temp_R0*temp_RTDA*temp_RTDA-4*temp_R0*RTD_B*(temp_R0-Rtemp)) )/(2*temp_R0*RTD_B);
	}

	//TemperC = TemperC*gTempK + gTempB;
	tempF = TemperC*1.8+32;  //华氏度-zb
	tempF=tempF;
	/************************************************************************
	 CX copy this portion of the code from temperatureRead()
	*********************************************************************/
//#ifndef TEMP_DEBUG
//	if ( (tempF < 150.0) && (tempF > -4.0) )
//    {
//		if (gFirstTemp == true)
//		{
//			gTempF = tempF;
//			gFirstTemp = false;
//		}
//		else
//		{
//			if (gTempF > tempF)
//			{
//				tempDiff = gTempF - tempF;
//			}
//			else
//			{
//				tempDiff = tempF - gTempF;
//			}

//			if ( (tempDiff > TEMP_JITTER_MIN_THRESHOLD) && (tempDiff < TEMP_JITTER_MAX_THRESHOLD) )
//			{
//				gTempF = tempF;
//			}
//		}
//    }
//#else
//	gTempF = tempF;
//#endif

    //return gTempF;
	if((TemperC<-10)||(TemperC>90))   //当温度超出限制，在运行状态寄存器中给出提示
	{
		system_status.runStatus=TMP_OVERRANGE;
		TemperC=25;
	}
	
	return TemperC;
}
