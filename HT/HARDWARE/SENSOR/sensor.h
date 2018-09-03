/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __SENSOR_H

#define __SENSOR_H
#include "app.h"
#include "embreg.h"
#include "spi.h"
#include "switch.h"

#define TIME_DELAY 200

#define ERR_SINGNAL_TOO_LOW 0x01

void measure(void);
u8 slopeCalib(float solution);
#endif
