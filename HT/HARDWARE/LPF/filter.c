/******************************************************************************
 * Copyright (C) 2013-2014 Pyxis-lab Inc.
 *
 *-----------------------------------------------------------------------------
 * File: 			filter.c
 * Description:		digital filter to smooth sampling data
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Mar 27, 2014
 *****************************************************************************/
/* ----------------------- Platform includes --------------------------------*/
//#include "datatype.h"
#include "filter.h"
//#include "param.h"
//#include "system.h"
//#include "version.h"

/***********************************************
 * Default Butterworth filter parameters
 *  cutoff frequency = 1Hz
 *  sample frequency = 2.02Hz
 **********************************************/
float g_a0 = 0.005543;
float g_a1 = 0.011086;
float g_a2 = 0.005543;
float g_b1 = 1.7787;
float g_b2 = -0.8008;

float g_r2_sample = INVALID_SAMPLE;    // 2 sample before current raw data
float g_r1_sample = INVALID_SAMPLE;    // 1 sample before current raw data

float g_f2_sample = INVALID_SAMPLE;    // 2 sample before current filtered data
float g_f1_sample = INVALID_SAMPLE;    // 1 sample before current filtered data

/******************************************************
 * x'(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) +
 *         b1*x'(n-1) + b2*x'(n-2)
 *
 * x(n)    - current raw data
 * x(n-1)  - 1 sample before current
 * x(n-2)  - 2 sample before current
 * x'(n)   - current filtered data
 * x'(n-1) - 1 sample before current
 * x'(n-2) - 2 sample before current
 ****************************************************/
void filter_setup(void)
{
    float k1, k2, k3;
    float omg = 0.02;

    k1 = 1.414*omg;
    k2 = omg*omg;
    g_a0 = k2/(1.0+k1+k2);
    g_a1 = 2.0*g_a0;
    g_a2 = g_a0;

    k3 = 2.0*g_a0/k2;
    g_b1 = (-2.0)*g_a0+k3;
    g_b2 = 1.0-2.0*g_a0-k3;

    return ;
}

void filter_set_initial(float raw_2, float raw_1, float filter_2, float filter_1)
{
    g_r2_sample = raw_2;
    g_r1_sample = raw_1;

    g_f2_sample = filter_2;
    g_f1_sample = filter_1;

    return ;
}

float filter_get_output(float rawData)
{
    float filterData;

    if (g_r2_sample == INVALID_SAMPLE)
    {
        g_r2_sample = g_f2_sample = rawData;
        filterData = rawData;
    }
    else if (g_r1_sample == INVALID_SAMPLE)
    {
        g_r1_sample = g_f1_sample = rawData;
        filterData = rawData;
    }
    else
    {
        filterData = g_a0*rawData + g_a1*g_r1_sample + g_a2*g_r2_sample + \
                     g_b1*g_f1_sample + g_b2*g_f2_sample;

        g_r2_sample = g_r1_sample;
        g_f2_sample = g_f1_sample;
        g_r1_sample = rawData;
        g_f1_sample = filterData;
    }

    return filterData;
}

