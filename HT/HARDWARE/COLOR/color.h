/**
  ******************************************************************************
  * @file    color.h
  * @author  
  * @version V1.0
  * @date    2018-06-14
  * @brief   Colorimetric measurement
  ******************************************************************************
  * @attention
  *  
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __COLOR_H

#define __COLOR_H
#include "app.h"
#include "embreg.h"
#include "LTC2630ISC6.h"
#include "SWITCH.H"
#include "ADC.H"

void measureUrea(void);
void measureClF(void);

#endif
