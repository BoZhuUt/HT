#include "sys.h"		    
#include "uart2.h"	 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


#ifdef EN_USART2_RX   	//如果使能了接收
 		  
  
void USART2_IRQHandler(void)
{
	u8 res;	
	(void)res;
 
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 			 
		res =USART_ReceiveData(USART2); 	//读取接收到的数据
	}									 
} 
#endif	
									 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void Uart2_Init(u32 bound)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
 		NVIC_InitTypeDef NVIC_InitStructure;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
 
	
		#ifdef EN_USART2_RX		  	//如果使能了接收
		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

		USART_Init(USART2, &USART_InitStructure);  //初始化串口
		
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
		NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART2, ENABLE);                    //使能串口 

		#endif
}
 






















