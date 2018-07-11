#include "AD5410.h"

void DA_SendByte(unsigned char byte);
void DA_control(unsigned char addr,unsigned int data) ;

void AD5410_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=AD5410_CLEAR_PIN;   //5410_CLEAR
	GPIO_Init(AD5410_CLEAR_PORT,&g);
	
	g.GPIO_Pin=AD5410_LATCH_PIN;   //5410_LATCH
	GPIO_Init(AD5410_LATCH_PORT,&g);
	
	g.GPIO_Pin=AD5410_CLOCK_PIN;	 //5410_CLOCK
	GPIO_Init(AD5410_CLOCK_PORT,&g);
	
	g.GPIO_Pin=AD5410_SLAVE_IN_PIN;		//5410_SLAVE_IN
	GPIO_Init(AD5410_SLAVE_IN_PORT,&g);	
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=AD5410_FAULT_PIN;    //5410_FAULT
	GPIO_Init(AD5410_FAULT_PORT,&g);
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=AD5410_SLAVE_OUT_PIN;    //5410_SLAVE_OUT
	GPIO_Init(AD5410_SLAVE_OUT_PORT,&g);	
 } 
 
void SIMO_H(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_SetBits(AD5410_SLAVE_IN_PORT,AD5410_SLAVE_IN_PIN);
}

void SIMO_L(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_ResetBits(AD5410_SLAVE_IN_PORT,AD5410_SLAVE_IN_PIN);
} 

 /******************************/
 /*   往AD5410中写入一个字节   */
 /******************************/
void DA_SendByte(unsigned char byte)  
{		
		
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        AD5410_CLOCK_L; 
        if ( byte & 0x80 )
				{SIMO_H();
					delay_us(1);}
        else
				{SIMO_L();
					delay_us(1);}
        AD5410_CLOCK_H; 
        byte <<= 1;     
        AD5410_CLOCK_L;
    }
}
/***********************************/
/*   往AD5410中写入完整的命令      */
/*   总长3个字节 addr寄存器地址    */
/*            data操作指令         */
void DA_control(unsigned char addr,unsigned int data)    
{
	u8 Hdata,Ldata;
	Hdata=data>>8;
	Ldata=data&255;
	AD5410_LATCH_L;
	DA_SendByte(addr);
	DA_SendByte(Hdata);
	DA_SendByte(Ldata);
	AD5410_LATCH_H;
	delay_us(2);
}
 
void LATCH(void)
{
	 AD5410_LATCH_L;
	 delay_us(5);
	 AD5410_LATCH_H;
	 delay_us(5);
	 AD5410_LATCH_L;
}
/**********************/
/*    初始化AD5410    */
void AD5410_Init(void)
{
	AD5410_GPIOInit();
	DA_control(0x56,0x0001);   //复位  0x56复位寄存器地址   0x0001复位指令
	AD5410_CLEAR_L;
	LATCH();
	DA_control(0x55,0x1016);   //0-20ma  257730更新频率   1/16步进大小
	LATCH();
}
/********************/
/* IOUT  0<=DATA<=20*/
void AD5410_IOUT(float DATA)
{
	u16 I_OUT;
	if(DATA>=20)
	{
		I_OUT=4095;
	}
	else if(DATA<=4)
	{
		DATA=4;
		I_OUT=DATA*4096/20;
	}
	else
	{
		I_OUT=DATA*4096/20;
	}
	I_OUT=((int)I_OUT)<<4;
	DA_control(0x01,I_OUT);///往AD5410中写入完整的命令
	LATCH();
}
/************************************/
/*    往AD5410中写入回读指令        */
/*    addr为回读的寄存器控制字      */ 
/*    0x00读取状态寄存器            */ 
/*    0x01读取数据寄存器            */
/*    0x02读取控制寄存器            */  
// void DA_Read_Register(unsigned char addr)
// {
//   AD5410_LATCH_L; 
//   DA_SendByte(0x02);               //回读寄存器   
//   DA_SendByte(0x00);
//   DA_SendByte(addr);
//   AD5410_LATCH_H; 
// }

/***********************************************/
/*            从Register中回读数据             */
/*将回读的数据存储在data_ReadFormRegister[x] 中*/
// void ReadDataFormRegister()    //将回读的数据存储在data_ReadFormRegister[x] 中
// {
// 	u8 temp;
// 	u8 i,j;
// 	//DA_CLK_HIGH;
// 	AD5410_LATCH_L;
// 	SIMO_L();
// 	delay_us(1);
// 	for(i=0; i<3; i++)
// 	{
// 		for(j=0; j<8; j++)
// 		{

// 			AD5410_CLOCK_L;

// 			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==0)
// 			{
// 				temp=temp<<1;
// 			}
// 			else
// 			{
// 				temp=temp<<1;
// 				temp=temp+0x01;
// 			}
// 	    data_ReadFormRegister[i]=temp;
// 			temp=0;
// 			AD5410_CLOCK_H; 
// 		}
//   }
// 	SIMO_H();
// 	AD5410_LATCH_H;
// }
/************************************/
/*    ReadDatafromAD5410 Only       */
/*    addr为回读的寄存器控制字      */  
/*    0x00读取状态寄存器            */ 
/*    0x01读取数据寄存器            */
/*    0x02读取控制寄存器            */ 
// void ReadDataFromAD5410(unsigned char addr)
// {
// 	DA_Read_Register(addr);
// 	ReadDataFormRegister();
// }
