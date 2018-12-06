#include "ad5060.h"

void ad5060GpioInit(void)
{
    GPIO_InitTypeDef g;
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=AD5060_CS_PIN | AD5060_DIO_PIN;   
	GPIO_Init(AD5060_DIO_PORT,&g);
    
	g.GPIO_Pin=AD5060_CLK_PIN;   
	GPIO_Init(AD5060_CLK_PORT,&g);
    
}

void WriteByte_ad5060(u8 command)
{
    unsigned char i;
    for(i = 0; i < 8;i++)
    {
        if(command&0x80)
            AD5060_DIO_H;
        else
            AD5060_DIO_L;
            command <<= 1;
            AD5060_CLK_H;
            delay_us(1);
            AD5060_CLK_L;
            delay_us(1);
    }
}

void Write2Byte_ad5060(u16 command)
{
    unsigned char i;
    for(i = 0; i < 16;i++)
    {
        if(command&0x8000)
            AD5060_DIO_H;
        else
            AD5060_DIO_L;
            command <<= 1;
            AD5060_CLK_H;
            delay_us(1);
            AD5060_CLK_L;
            delay_us(1);
    }
}

void ad5060Out(u16 DATA)
{
    if(DATA>65535)
        DATA=0;
    AD5060_CS_L;
    WriteByte_ad5060(0x00);
    Write2Byte_ad5060(DATA);
    AD5060_CS_H;
}