/******************************************************************************
 * Copyright (C) 2013-2014 Pyxis-lab LLC
 *
 *-----------------------------------------------------------------------------
 * File: 			filter.h
 * Description:		header file for digital filter
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Mar 27, 2014
 *****************************************************************************/

#ifndef __FILTER_H__
#define __FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_SAMPLE    9999.0

extern void filter_setup(void);
extern float filter_get_output(float rawData);
extern void filter_set_initial(float raw_2, float raw_1, float filter_2, float filter_1);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
