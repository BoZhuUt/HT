#ifndef __ADS1120_H

#define __ADS1120_H

#include "stm32f10x.h"



void ADS1120_Init(void);
void WriteOneByte(unsigned char command);
unsigned char ReadOneByte(void);
int16_t ReadData(void);
void ADReset(void);
void ADPowerDown(void);
void ADStartConversion(void);
void ReadRegister(void);
void WriteRegister(unsigned char StartAddress, unsigned char NumRegs, unsigned char * pData);
void CofigAD(unsigned char channel);
void SetChannelPGA(unsigned char channel, unsigned char gain);
int16_t GetAD(unsigned char channel);
int16_t GetAD_ONLY_ORP(unsigned char channel);

#endif
