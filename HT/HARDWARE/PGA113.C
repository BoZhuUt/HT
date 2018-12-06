#include "PGA113.h"

void PGA113_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=PGA113_CS_PIN | PGA113_DIO_PIN;   
	GPIO_Init(PGA113_DIO_PORT,&g);
    
	g.GPIO_Pin=PGA113_CLK_PIN;   
	GPIO_Init(PGA113_CLK_PORT,&g);     
}

void WriteByte(u8 command)
{
    unsigned char i;
    for(i = 0; i < 8;i++)
    {
        if(command&0x80)
          PGA113_DIO_H;
        else
          PGA113_DIO_L;
          command <<= 1;
          PGA113_CLK_H;
          PGA113_CLK_L;				
    }
}

void configPGA113(u8 ch,u8 gain)
{
    unsigned char i;
    switch(gain)
    {
        case 1: gain=pgaGain1;
                break;
        case 2: gain=pgaGain2;
                break;
        case 4: gain=pgaGain4;
                break;
        case 8: gain=pgaGain8;
                break;
        case 16: gain=pgaGain16;
                break;
        case 32: gain=pgaGain32;
                break;
        case 64: gain=pgaGain64;
                break;
        case 128: gain=pgaGain128;
                break;
        default: gain=pgaGain1;
                break;
    }
    i=ch+gain;
    PGA113_CS_L;
    WriteByte(0x3A);
    WriteByte(i);
    PGA113_CS_H;
}