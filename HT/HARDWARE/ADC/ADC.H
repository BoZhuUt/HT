/**
  ******************************************************************************
  * @file    adc.h
  * @author  bo.zhu
  * @version V1.0
  * @date    2016-12-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  *  
  ******************************************************************************
 */ 
 #ifndef __ADC_H
#define __ADC_H	
#include "stm32f10x.h"
#include "delay.h"
#include "stm32f10x_adc.h"

float ADC_ReadChannel(u8 channel,u16 times);

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
float Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
