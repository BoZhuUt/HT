/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

//STM32操作相关头文件
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_iwdg.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
  if(xRxEnable)
  {
    //使能接收和接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //MAX485操作 低电平为接收模式
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
  }
  else
  {
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
    //MAX485操作 高电平为发送模式
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
  }

  if(xTxEnable)
  {
    //使能发送完成中断
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
  }
  else
  {
    //禁止发送完成中断
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  }
  
}

/**
  * @brief  串口初始化
  * @param  ucPORT      串口号
  *         ulBaudRate  波特率
  *         ucDataBits  数据位
  *         eParity     校验位 
  * @retval None
  */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	  (void)ucPORT;     //不修改串口
  (void)ucDataBits; //不修改数据位长度
  //(void)eParity;    //不修改校验格式



		//打开USART2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	
	//开启PA、PB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

  //GPIOA9 USART1_Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIOA.10 USART1_Rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //浮动输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = ulBaudRate;            //只修改波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	if(eParity == MODBUS_PARITY_ODD)
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else if(eParity == MODBUS_PARITY_EVEN)
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	else 
	{
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
  USART_InitStructure.USART_Parity = USART_Parity_Even;

  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  //串口初始化
  USART_Init(USART2, &USART_InitStructure);
  //使能USART1
  USART_Cmd(USART2, ENABLE);
  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //设定USART1 中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //最后配置485发送和接收模式
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  //GPIOD.8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  return TRUE;
}

/**
  * @brief  通过串口发送数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  //发送数据
  USART_SendData(USART2, ucByte);
  return TRUE;
}

/**
  * @brief  从串口获得数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  //接收数据
  *pucByte = USART_ReceiveData(USART2);
  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
  //mb.c eMBInit函数中
  //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
  //发送状态机
  pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  //mb.c eMBInit函数中
  //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
  //接收状态机
  pxMBFrameCBByteReceived();
}

u8 flag_rx = 0;

/**
  * @brief  USART1中断服务函数
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	IWDG_ReloadCounter();//喂狗
  //发生接收中断
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
    prvvUARTRxISR(); 
    //清除中断标志位    
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   
  }
  
  //发生完成中断
  if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
  {
    prvvUARTTxReadyISR();
    //清除中断标志
    USART_ClearITPendingBit(USART2, USART_IT_TC);
  }
   
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART2,USART_FLAG_ORE); 
  }	
	if(USART_GetFlagStatus(USART2,USART_FLAG_NE))
	{
		USART_ClearFlag(USART2,USART_FLAG_NE); 
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_FE))
	{
		USART_ClearFlag(USART2,USART_FLAG_FE); 
	}
  if(USART_GetFlagStatus(USART2,USART_FLAG_PE))
	{
		USART_ClearFlag(USART2,USART_FLAG_PE); 
	}	
}

