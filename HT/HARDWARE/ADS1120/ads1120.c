#include "spi.h"
#include "delay.h"

u16 adcResults[4]={1};
u8 Init_Config[4],channel0[8],channel1[8],channel2[8],channel3[8];
void ADReset(void);
/***************************************************************************
* Function Name:  ADS1120_Init()	
* Purpose: 	  ADS120 initialize
* Params :        
*
*************************************************************************/
void ADS1120_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	/******************* SLAVE OUT , DRDY ******************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_OUT_PIN;    
	GPIO_Init(SPI_SLAVE_OUT_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ADS1120_DRDY_PIN;                               
	GPIO_Init(ADS1120_DRDY_PORT,&GPIO_InitStructure);
	
	/******************* CS , CLOCK , SLAVE IN ******************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ADS1120_CS_PIN;                               
	GPIO_Init(ADC1120_CS_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_CLOCK_PIN;                               
	GPIO_Init(SPI_CLOCK_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=SPI_SLAVE_IN_PIN;   
	GPIO_Init(SPI_SLAVE_IN_PORT,&GPIO_InitStructure);

  ADS1120_CS_H;
  SPI_CLOCK_L;
  ADReset();
}

/***************************************************************************
* Function Name:  WriteOneByte(unsigned char command)	
* Purpose: 	  write one byte to ADS1120
* Params :        
*
*************************************************************************/
void WriteOneByte(u8 command)
{
    unsigned char i;
    for(i = 0; i < 8;i++)
    {
        if(command&0x80)
          SPI_SLAVE_IN_H;
        else
          SPI_SLAVE_IN_L;
          command <<= 1;
          SPI_CLOCK_H;
          SPI_CLOCK_L;				
    }
}

/***************************************************************************
* Function Name:  ReadOneByte()	
* Purpose: 	  read one byte from ADS1120
* Params :        result
*
*************************************************************************/
unsigned char ReadOneByte(void)
{
    unsigned char result,i;
    SPI_CLOCK_L;
    for(i = 0;i<8; i++)
    {
        SPI_CLOCK_H;
        result <<= 0x01;
        if(READ_SPI_SLAVE_OUT)
          result |= 0X01;
        SPI_CLOCK_L;
    }
    return result;
}

/***************************************************************************
* Function Name:  ReadData()	
* Purpose: 	  read data from ADS1120
* Params :        result 0~65535
*
*************************************************************************/
int16_t ReadData(void)
{
    int16_t result;
    ADS1120_CS_L;
    WriteOneByte(RDATA);
    result = ReadOneByte();
    result = (result<<8) | ReadOneByte();
    ADS1120_CS_H;
    return result;
}

/***************************************************************************
* Function Name:  ADReset()	
* Purpose: 	  reset ADS1120
* Params :        
*
*************************************************************************/
void ADReset(void)
{
    ADS1120_CS_L;
    WriteOneByte(RESET);
    ADS1120_CS_H;
}

/***************************************************************************
* Function Name:  ADPowerDown()	
* Purpose: 	  
* Params :        
*
*************************************************************************/
void ADPowerDown(void)
{
    ADS1120_CS_L;
    WriteOneByte(POWERDOWN);
    ADS1120_CS_H;
}

/***************************************************************************
* Function Name:  ADStartConversion()	
* Purpose: 	  start conversion
* Params :        
*
*************************************************************************/
void ADStartConversion(void)
{
    ADS1120_CS_L;
    WriteOneByte(START);
    ADS1120_CS_H;
}

/***************************************************************************
* Function Name:  ReadRegister()	
* Purpose: 	  read data from register
* Params :        data 
*
*************************************************************************/
void ReadRegister(void)
{
    unsigned char i;
    unsigned long Data;
    ADS1120_CS_L;
    WriteOneByte(RREG|0x03);
	// get the register content
	for (i=0; i< 4; i++)
	{
		 Data += ReadOneByte();
	}
	ADS1120_CS_H;
	return;
}

/***************************************************************************
* Function Name:  WriteRegister(unsigned char StartAddress, unsigned char NumRegs, unsigned char * pData)	
* Purpose: 	  write data to register
* steps:          1.write address  2.write data  
* Params          NumRegs-----the numbers of register
*************************************************************************/
void WriteRegister(unsigned char StartAddress, unsigned char NumRegs, unsigned char * pData)
{
    unsigned char i;
    ADS1120_CS_L;
    // send the command byte
    WriteOneByte(WREG | (((StartAddress<<2) & 0x0c) |((NumRegs-1)&0x03)));
    // send the data bytes
    for (i=0; i< NumRegs; i++)
    {
      WriteOneByte(*pData);
      pData++;
    }
    ADS1120_CS_H;
    return;
}

/***************************************************************************
* Function Name:  CofigAD(unsigned char channel)	
* Purpose: 	  set the sampling channel
* Params :        
*
*************************************************************************/
void CofigAD(unsigned char channel)
{
    switch(channel)
    {
    case T420: Init_Config[0] = 0x81;//AIN0 , GAIN=1 , PGA disabled
            break;
    case S365: Init_Config[0] = GAIN;//AIN1 , GAIN=4 , PGA enabled
            break;
    case T365: Init_Config[0] = 0xA1;//AIN2 , GAIN=1 , PGA disabled
            break;
    case S420: Init_Config[0] = 0xB2;//AIN3 , GAIN=2 , PGA enabled
            break;
    }
    Init_Config[1] = 0X00;//default
    Init_Config[2] = 0X40;//Init_Config[2] = 0X56;
    Init_Config[3] = 0X00; //0x80
    WriteRegister(0x00,0x04,Init_Config);
    ReadRegister();
}

/***************************************************************************
* Function Name:  GetAD(unsigned char channel)	
* Purpose: 	  get the result 
* Params :        Result 0~65535
*
*************************************************************************/
u16 GetAD(unsigned char channel)
{
    int16_t Result;
    CofigAD(channel);
    ADStartConversion();
    while(READ_ADS1120_DRDY);
    Result = ReadData();
    ADPowerDown();
    return Result;
}

void ADS1120_ReadChannel(u8 channel,u16 times)
{
	u16 j=0;
	float sum=0.0;
	
	for(j=0;j<times;j++)
	{
		sum+=GetAD(channel);
	}
	sum=sum/(float)times;
	adcResults[channel]=sum;
}

unsigned int ADC_Res;
void ADS1120_Init(void)
{
	ADS1120_GPIOInit();
}
/***************************************************************************
* Function Name:  CofigAD(unsigned char channel)	
* Purpose: 	  set the sampling channel
* Params :        
*
*************************************************************************/
void cofigAD(unsigned char channel)
{
    switch(channel)
    {
    case T420: Init_Config[0] = 0x81;//AIN0 , GAIN=1 , PGA disabled
            break;
    case S365: Init_Config[0] = GAIN;//AIN1 , GAIN=4 , PGA enabled
            break;
    case T365: Init_Config[0] = 0xA1;//AIN2 , GAIN=1 , PGA disabled
            break;
    case S420: Init_Config[0] = 0xB2;//AIN3 , GAIN=2 , PGA enabled
            break;
    }
    Init_Config[1] = 0X04;//continuous conversion mode
    Init_Config[2] = 0X40;//Init_Config[2] = 0X56;
    Init_Config[3] = 0X00; //0x80
    WriteRegister(0x00,0x04,Init_Config);
    ReadRegister();
}
/***************************************************************************
* Function Name:  ads1120ReadChannel(u8 channel,u16 times)	
* Purpose: 	  get the result 
* Params :    Result 0~65535
*
*************************************************************************/
void ads1120ReadChannel(u8 channel,u16 times)
{
	u16 j=0;
	float sum=0.0;
	cofigAD(channel);
	ADStartConversion();
	for(j=0;j<times;j++)
	{
		while(READ_ADS1120_DRDY);
		sum+=ReadData();
	}
	sum=sum/(float)times;
	ADPowerDown();
	adcResults[channel]=sum;
}  



