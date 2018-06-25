#ifndef __CONDUCT_H__
#define __CONDUCT_H__

#include "stm32f10x.h"

extern u8 isMeasureFlg;

float measure_G(void);
void Tim3_init(void);

#endif
