
#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

// #include "stm32f10x_lib.h"
#include "stm32f10x.h"
//#include "sys.h"
#include "delay.h"

#define SCLOCK1   	GPIO_SetBits(GPIOB,GPIO_Pin_0);
#define SCLOCK0   	GPIO_ResetBits(GPIOB,GPIO_Pin_0);

#define Temp_CS1  	GPIO_SetBits(GPIOA,GPIO_Pin_0);
#define Temp_CS0  	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

#define DIN1      	GPIO_SetBits(GPIOB,GPIO_Pin_11);
#define DIN0      	GPIO_ResetBits(GPIOB,GPIO_Pin_11);

//#define DOUT1     	GPIO_SetBits(GPIOB,GPIO_Pin_6);
//#define DOUT0     	GPIO_ResetBits(GPIOB,GPIO_Pin_6);

#define	R0			(100.0)
#define	RTD_A		(3.908E-3)
#define	RTD_B		(-5.8019E-7)
#define	RTD_C		(-4.2735E-12)

// Celsius [0-70] => Fahrenheit [32-160]
#define	TEMP_MIN	(0)
#define	TEMP_MAX	(70)

typedef struct 
{
	float temp_k;
	float temp_b;
}TemperatureParameter;

extern float TemperC;

// void delayLoop(unsigned int Time);//暂时定义，延时函数

 
void  temperatureInit(void);
float temperatureRead(void);
void AD7792Init(void);
// extern void  temperatureInit(void);
// extern float temperatureRead(void);

#endif
