/**
  ******************************************************************************
  * @file    adc.c
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
#include "ADC.H"

/***********************************DMA采样配置**************************************/

//#define AverageConstant 30
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
__IO uint16_t ADC_RegularConvertedValueTab[1000];

void TIM3_TRGO_Config(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	 /* Time Base configuration */
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1; //1M, 1us
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 10;   //每10us触发一次ADC采样     
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
}

void ADC_Config(u8 channel,u16 times)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

  /* Configure PA6 as anolog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA1 Channel1 Configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RegularConvertedValueTab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //由外设搬到内存
  DMA_InitStructure.DMA_BufferSize = times;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
// 	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel6 configuration */ 
  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_13Cycles5);  
	
	ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 external trigger */ 
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

//times的大小不可以超过DMA接收数组ADC_RegularConvertedValueTab[1000]的长度
float ADC_ReadChannel(u8 channel,u16 times)
{
	float sum=0.0;
	float average=0.0;
	u16 count=0;
	
	if(times>0)
	{
		TIM3_TRGO_Config();
		ADC_Config(channel,times);
		TIM_Cmd(TIM3, ENABLE); 

		while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
		DMA_ClearFlag(DMA1_FLAG_TC1); //清除通道1发送完成标志
		TIM_Cmd(TIM3, DISABLE);  //关闭TIM3，停止驱动ADC
		
		for(count=0;count<times;count++)
		{
			sum+=ADC_RegularConvertedValueTab[count];
		}
		average=sum/times;
	}
	else
	{
		average=1.0;
	}
	
	return average;	
}


/***********************************原来的配置**************************************/

	void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA6 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	//ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_13Cycles5 );	
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

float Get_Adc_Average(u8 ch,u8 times)
{
	float sum=0;
	float adcValue=1.0;
	u8 t;
	
	for(t=0;t<times;t++)
	{
		sum+=Get_Adc(ch);
		//delay_ms(5);
	}
	if(times>0)
	{
	  adcValue=sum/times;
	}
	
	return adcValue;	
} 	

