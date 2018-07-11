/**
  ******************************************************************************
  * @file    IAP/src/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/** @addtogroup IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* Private function prototypes -----------------------------------------------*/
static void IAP_Init(void);
void USART2_ReInit(void);
/* Private functions ---------------------------------------------------------*/

uint8_t Index=0;
uint8_t StrBuff[CMD_STRING_SIZE];

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{	
		StrBuff[Index]=USART_ReceiveData(USART2);		
		Index++;	
	}
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  //SystemInit();
	char isGotoBootloader=0;
	char i=3; 
	/* Flash unlock */
  FLASH_Unlock();
	IAP_Init();//初始化串口，串口等待更新指令
	Serial_PutString("Sensor Startup\r\n\n");
//	Delay_ms(1000);
	Serial_PutString("Boot to App in 3 sec later\r\n\n");
//	Delay_ms(1000);
	Serial_PutString("Press abc to interrupt ......\r\n\n");
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	SET485_RX;
	while(i)
	{
		if((StrBuff[0]=='a')&&((StrBuff[1]=='b'))&&((StrBuff[2]=='c')))
		{
			isGotoBootloader=1;
			Serial_PutString("going \r\n\n");
			break;
		}
		delay_ms(1000);
		i--;
	}
	
	USART_DeInit(USART2);
	USART2_ReInit();
	
  if (isGotoBootloader)
  { 
    /* Display main menu */
    Main_Menu ();
  }  
  /* Keep the user application running */
  else
  {
		Serial_PutString("Jump To APP \r\n\n");
    /* Test if user code is programmed starting from address "ApplicationAddress" */
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    { 
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) ApplicationAddress);
      Jump_To_Application();
    }
  }

  while (1)
  {}
}

/**
  * @brief  Initialize the IAP: Configure RCC, USART and GPIOs.
  * @param  None
  * @retval None
  */
void IAP_Init(void)
{
//  USART_InitTypeDef USART_InitStructure;

//   /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
//   /* USART configured as follow:
//         - BaudRate = 115200 baud  
//         - Word Length = 8 Bits
//         - One Stop Bit
//         - No parity
//         - Hardware flow control disabled (RTS and CTS signals)
//         - Receive and transmit enabled
//   */
//   USART_InitStructure.USART_BaudRate = 115200;
//   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//   USART_InitStructure.USART_StopBits = USART_StopBits_1;
//   USART_InitStructure.USART_Parity = USART_Parity_No;
//   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//   STM_EVAL_COMInit(COM1, &USART_InitStructure);  
	 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

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

  USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 	
}

/** @LX
  * @brief  Initialize the IAP.
  * @param  None
  * @retval None
  */
void USART2_ReInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

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

  USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  //串口初始化
  USART_Init(USART2, &USART_InitStructure);
  //使能USART1
  USART_Cmd(USART2, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
