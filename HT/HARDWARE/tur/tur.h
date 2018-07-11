#ifndef __TUR_H

#define __TUR_H

#include "app.h"
#include "embreg.h"
#include "spi.h"
#include "SWITCH.H"
#include "ADC.H"

#define GROUND_ERR	0x01

#define S365CHANNEL ((uint8_t)0x04)
#define S420CHANNEL ((uint8_t)0x05)
#define T365CHANNEL ((uint8_t)0x06)
#define T420CHANNEL ((uint8_t)0x07)

extern float s365F;

//float getS365(u16 aveTimes);
void measureTurb(void);
void TIM2_MeasureInit(void);
void TIM1_ModpollInit(void);
float Calib_S365(void);

#endif
