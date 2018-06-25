#include "conduct.h"
#include "ltc2440.h"
#include "ad5410.h"
#include "app.h"
#include "stm32f10x_iwdg.h"

float Vva1,Via1;
u8 ResIdx;
float conduct;
float lastConduct=0;
u8 isFirstValue=1;
float measure_G(void)
{
	float I,G,T,V,R;
	
	USART_Cmd(USART2, DISABLE);
	
	if((measure_settings.smoothingFactor<0.00001)||(measure_settings.smoothingFactor>1))
	{
		measure_settings.smoothingFactor=0.3;
	}
	
	T=temperatureRead();
	T=T;	
	getCond();
	if(ResIdx==0)
		R=calib_settings.Ra;	
	else if(ResIdx==1)
		R=calib_settings.Rb;
	else if(ResIdx==2)
		R=calib_settings.Rc;	
	else if(ResIdx==3)
		R=calib_settings.Rd;	
	I=Via1/R;
	V=(Vva1-0.020)/6;
	conduct=I/V*1000000/(1+0.02*(T-25));
	if(!isFirstValue)
	{
		conduct=conduct*measure_settings.smoothingFactor+lastConduct*(1-measure_settings.smoothingFactor);
	}
	else
	{
		isFirstValue=0;
	}
	lastConduct=conduct;
 	G=conduct*calib_settings.K;	
  if(G<0)G=0;
	if(filter_settings.filterCoefficient_1==filter_settings.filterCoefficient_2)
	{
		filter_settings.filterCoefficient_1=0;
		filter_settings.filterCoefficient_2=10000;
	}
  measure_values.sensorValue_mA=((G-filter_settings.filterCoefficient_1)/(filter_settings.filterCoefficient_2-filter_settings.filterCoefficient_1)*16+4);    //modbus address 16003
  AD5410_IOUT(measure_values.sensorValue_mA); //4-20ma output
	measure_values.sensorValue=G;       //modbus address 46001
	measure_values.temperatureValue=T;    //modbus address 46005
// 	measure_values.sensorValue_mA=T;
// 	measure_values.temperatureValue=ResIdx;
	measure_values.i=Via1;
	measure_values.v=Vva1;
	
	USART_Cmd(USART2, ENABLE);
	
	return G;
}

void Tim3_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  //
  uint16_t PrescalerValue = 0;
  
  //使能定时器3时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //定时器时间基配置说明
  //HCLK为72MHz，APB1经过2分频为36MHz
  //TIM3的时钟倍频后为72MHz（硬件自动倍频,达到最大）
  //TIM3的分频系数为3599，时间基频率为72M/ (1 + Prescaler) = 10KHz,基准为0.1ms
  //TIM最大计数值为usTim1Timerout50u
  PrescalerValue = (uint16_t) (SystemCoreClock / 10000) - 1;// 
  //定时器3初始化
  TIM_TimeBaseStructure.TIM_Period =39999;     //    timing cycle = 4S
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;  //预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;      //设置分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  //预装载使能
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  
  //
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //定时器3中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
//   //清除溢出中断标志位
//   TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//   //定时器3溢出中断关闭
//   TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

  //清除更新中断标志
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
	//使能TIM3更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
  
//   //定时器3禁能
//   TIM_Cmd(TIM3,  DISABLE);

     TIM_Cmd(TIM3,ENABLE);

}

/* use TIM to prevent modbus time out*/
u8 isMeasureFlg=0;
void TIM3_IRQHandler(void)
{
  if( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET )
  {
    IWDG_ReloadCounter();//喂狗
	  
		isMeasureFlg=1; 
		
	  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
  }
}
