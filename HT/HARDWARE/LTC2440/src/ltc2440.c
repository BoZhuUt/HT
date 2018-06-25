#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ltc2440.h"
#include "stdint.h"
void  WriteToReg_LTC2440(unsigned char ByteData);
void Write1bytetoltc2440(u8 data);
u8    DataRead1[4];
u32  adc_value_32;

/*********************/
//2440GPIO口初始化
/*********************/
void ltc2440Gpio_Init(void)
{
//	SPI_DCLK 	PB4
//	SPI_SDO 	PB5 	
//	SPI_SDI 	PC15
//	ADC_CS 		PB6		
	
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_5;//DCLK(SPI CLOCK_BB),pb5
	GPIO_Init(GPIOB,&g);
	g.GPIO_Pin=GPIO_Pin_7;//CS(ADC_CS),pB6
	GPIO_Init(GPIOB,&g);
	g.GPIO_Pin=GPIO_Pin_15;//(SPI SLAVE_IN_BB),pc15
	GPIO_Init(GPIOC,&g);
	
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=GPIO_Pin_6;//DOUT(SPI SLAVE_OUT_BB)
	GPIO_Init(GPIOB,&g);
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=GPIO_Pin_4;// ADC_BUSY ,PB4(需要释放第一功能)
	GPIO_Init(GPIOB,&g);
 }


#define SW0_A_1   	GPIO_SetBits(GPIOB,GPIO_Pin_8);
#define SW0_A_0   	GPIO_ResetBits(GPIOB,GPIO_Pin_8);

#define SW0_B_1   	GPIO_SetBits(GPIOB,GPIO_Pin_9);
#define SW0_B_0   	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

#define COND_A0_1   	GPIO_SetBits(GPIOC,GPIO_Pin_13);
#define COND_A0_0   	GPIO_ResetBits(GPIOC,GPIO_Pin_13);

#define COND_A1_1   	GPIO_SetBits(GPIOC,GPIO_Pin_14);
#define COND_A1_0   	GPIO_ResetBits(GPIOC,GPIO_Pin_14);

#define S1B_1   	GPIO_SetBits(GPIOA,GPIO_Pin_9);
#define S1B_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_9);

#define S2B_1   	GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define S2B_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_8);

#define S3B_1   	GPIO_SetBits(GPIOB,GPIO_Pin_14);
#define S3B_0   	GPIO_ResetBits(GPIOB,GPIO_Pin_14);

#define S4B_1   	GPIO_SetBits(GPIOB,GPIO_Pin_15);
#define S4B_0   	GPIO_ResetBits(GPIOB,GPIO_Pin_15);

#define DA0_1   	GPIO_SetBits(GPIOA,GPIO_Pin_12);
#define DA0_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_12);

#define DA1_1   	GPIO_SetBits(GPIOA,GPIO_Pin_11);
#define DA1_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_11);

#define DA2_1   	GPIO_SetBits(GPIOA,GPIO_Pin_10);
#define DA2_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_10);

// #define positive_pulse_time  1000     //正脉冲时间
// #define negative_pulse_time  1000			//负脉冲时间
// #define sampling_time        300     //采样时间

u8 data_ReadFormRegister[3];

u16 SendDataBuffer[3]={0} ;
// int number1;
// float V_positive_pulse=0;
// float V_negative_pulse_time=0;
// float  V_positive_1=0;    //VS1
// float  V_positive_2=0;		//VS2

// float  V_negative_1=0;		//VS3
// float  V_negative_2=0;    //VS4

void temperatureInit(void);
float temperatureRead(void);
void TIM3_IRQHandler(void) ;  //TIM3中断
/*******************************************************/
/*脉冲对应端口初始化函数*/
void Da_init(void)
{
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_12;//DA0为PA12--+1.8V
	GPIO_Init(GPIOA,&g);
	g.GPIO_Pin=GPIO_Pin_11;//DA1为PA11--GND
	GPIO_Init(GPIOA,&g);
	g.GPIO_Pin=GPIO_Pin_10;//DA2为PA12---1.8V
	GPIO_Init(GPIOA,&g);
}


/*******************************************************/
void smp04_init(void)          //采样保持器对应端口初始化
{
	GPIO_InitTypeDef x;
	x.GPIO_Mode=GPIO_Mode_Out_PP;
	x.GPIO_Speed=GPIO_Speed_50MHz;
	
	x.GPIO_Pin=GPIO_Pin_8;    //SW0-A  PB8
	GPIO_Init(GPIOB,&x);
	
	x.GPIO_Pin=GPIO_Pin_9;    //SW0-B  PB9
	GPIO_Init(GPIOB,&x);
}
/*******************************************************/
void adg1604_init(void)        //2440前的模拟开关对应的端口初始化  
{
		GPIO_InitTypeDef x;
	x.GPIO_Mode=GPIO_Mode_Out_PP;
	x.GPIO_Speed=GPIO_Speed_50MHz;

	
	x.GPIO_Pin=GPIO_Pin_13;     //COND_A0  PC13
	GPIO_Init(GPIOC,&x);
	
	x.GPIO_Pin=GPIO_Pin_14;    // COND_A1  PC14
	GPIO_Init(GPIOC,&x);
}
/*********************************************************/
void adg1612_init(void)       //换挡对应的端口初始化
{
			GPIO_InitTypeDef x;
	x.GPIO_Mode=GPIO_Mode_Out_PP;
	x.GPIO_Speed=GPIO_Speed_50MHz;
	
	x.GPIO_Pin=GPIO_Pin_9;     //S1B           200
	GPIO_Init(GPIOA,&x);
	
	x.GPIO_Pin=GPIO_Pin_8;     //S2B           2K
	GPIO_Init(GPIOA,&x);
	
		x.GPIO_Pin=GPIO_Pin_14;  //S3B           20K
	GPIO_Init(GPIOB,&x);
	
	x.GPIO_Pin=GPIO_Pin_15;    //S4B           200K
	GPIO_Init(GPIOB,&x);
	
}
/******************************************/
void produce_pulse(int a)        //脉冲产生函数（a=1:+1.5,a=2:-1.5,a=3:0)
{
	if(a==1)                       //+1.5V
	{
		DA1_0;
		DA2_0;
		DA0_1;
	}
	else if(a==2)                 //-1.5V
	{
		DA0_0;
		DA1_0;
 		DA2_1;
	}
	else if(a==3)                 //0V
	{
		DA0_0; 				
		DA2_0;
 		DA1_1;
	}
	else                         //0V
	{
		DA0_0; 				
		DA2_0;
 		DA1_1;		
	}
}
/**********************************************************/
u8 R_Transconductance(u8 a)    // 换挡设置 
{
	if(a==0)                        //Rtransconductance=200000
	{
		S1B_1;
		S2B_0;
		S3B_0;
		S4B_0;
	}
	else if(a==1)                   //Rtransconductance=20000
	{
		S1B_0;
		S2B_1;
		S3B_0;
		S4B_0;
	}
	else if(a==2)                   //Rtransconductance=2000
	{
		S1B_0;
		S2B_0;
		S3B_1;
		S4B_0;
	}
	else if(a==3)                   //Rtransconductance=200
	{
		S1B_0;
		S2B_0;
		S3B_0;
		S4B_1;
	}

	return 1;
}



	

/**********************************************************/
s32 Get_24BitADC()
{
      int i=0;
      s32 adcResult=0;
      s32 ADCResult=0;
      SCLOCK_0;
      
      while(1)
      {
            ADC_CS_0;
            //delayCount(10);
            if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
            {
                  
            }
            else
            {
                   break;
            }
      }
      //test end of conversion      
      ADC_CS_0;
      delay_us(2);
      for(i=0;i<32;i++)
      {
            SCLOCK_1;
            delay_us(2);
            if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
            {
                  adcResult+=(1<<(31-i));
            }
            SCLOCK_0;
      }
      ADC_CS_1;
      //get 32bits data from device
      ADCResult=adcResult>>5;
      ADCResult&=0x00ffffff;
      //get the use bits
      if((adcResult>>29)&1)
      {
            
      }
      else 
      {
            //ADCResult*=-1;
            //ADCResult=(~ADCResult)+1;
            ADCResult=ADCResult|0xff000000;     
      }
      return ADCResult;
}

#define A_Vv 	0
#define A_Vi 	1
#define B_Vv	2
#define B_Vi	3

s32 getADCFromCh(u8 ch)
{
		if(ch&0x01)
		{
				COND_A0_1;
		}
		else 
		{
				COND_A0_0;
		}
		if(ch&0x02)
		{
				COND_A1_1;
		}
		else 
		{
				COND_A1_0;
		}
		Get_24BitADC();///?????????????
		return Get_24BitADC();
}

s32 AvvMid,AviMid,Avv,Avi,Res,Vve,Vie;
float Vm,Vd,Vs;
u32 Vva,via;
extern u8 ResIdx; 
extern float Vva1,Via1;
float VvAverage,ViAverage;
u16 t[4]={7,10,60,200};  //脉冲宽度

#define Vpositive 1   //produce positive pulse
#define Vnegative  2
#define Vzero  3



#define AVGCount 40
void getCond()
{
		u8 i=0;
		produce_pulse(Vzero);
		for(i=0;i<4;i++)
		{
			ResIdx=3-i;
			//ResIdx=0;
			R_Transconductance(ResIdx);///Transconductance换挡
			

			produce_pulse(Vzero);///???????????
			SW0_A_0;      //打开采样保持器
			SW0_B_0;
			delay_us(500);
			AvvMid=getADCFromCh(A_Vv);///????????????
			AviMid=getADCFromCh(A_Vi);
			
			produce_pulse(Vpositive);     //发正脉冲
			delay_us(t[ResIdx]);
			SW0_A_1;
			
			produce_pulse(Vnegative);     //发负脉冲
			delay_us(t[ResIdx]);///脉冲宽度
			SW0_B_1;///???????????
			
			produce_pulse(Vzero);
			Avv=getADCFromCh(A_Vv);
			Avi=getADCFromCh(A_Vi);
			Res=AviMid-Avi;
			delay_ms(1);
			if(Res*0.0000001966953277587890625<0.95)  ///     
			{
					break;
			}
		}
		
		for(i=0;i<AVGCount;i++)///#define AVGCount 40
		{

			SW0_A_0;      //打开采样保持器
			SW0_B_0; 
			delay_us(500);
			AvvMid=getADCFromCh(A_Vv);
			AviMid=getADCFromCh(A_Vi);
			
			produce_pulse(Vpositive);  
			delay_us(t[ResIdx]);
			SW0_A_1;
			
			produce_pulse(Vnegative);  
			delay_us(t[ResIdx]);
			SW0_B_1;
			
			produce_pulse(Vzero);
			Avv=getADCFromCh(A_Vv);
			Avi=getADCFromCh(A_Vi);
			
			SW0_A_0;      //打开采样保持器
			SW0_B_0; 

      Vve=AvvMid-Avv;
			Vie=AviMid-Avi;
			Vva+=Vve;///?????
			via+=Vie;
			delay_ms(1);
			
		}
		  Vva/=AVGCount;
			via/=AVGCount;
			Vva1=Vva*0.0000001966953277587890625;///3.3v/(2^24)
			Via1=via*0.0000001966953277587890625;
}
 /***********************/
 void LTC2440_INIT(void)           //初始化
 {
	 ltc2440Gpio_Init();
	  Da_init();
	  smp04_init();
	 adg1612_init();
	 adg1604_init();
}
/************************/
void test()
{
	R_Transconductance(2);
			produce_pulse(Vzero);
			SW0_A_0;      //打开采样保持器
			SW0_B_0;
			delay_us(500);
			AvvMid=getADCFromCh(A_Vv);
			AviMid=getADCFromCh(A_Vi);
    	Via1=AviMid;
}

