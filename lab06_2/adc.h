/*
 * adc.h
 *
 *  Created on: Oct 22, 2024
 *      Author: lstuckly
 */


#ifndef ADC_H_
#define ADC_H_
#include <inc/tm4c123gh6pm.h>
// Initialize ADC

// Arrays for bot 4
static int raw[] = {
    1004,
    3153,
    3368,
    2211,
    1727,
    1479,
    1306,
    1190,
    1087,
    1047,
    953,
    893,
    865
};
static float ping[] = {
    3.27,
    8.82,
    14.56,
    19.96,
    25.46,
    30.00,
    35.33,
    40.55,
    45.61,
    50.59,
    56.88,
    61.96,
    66.83
};
void adc_init(void);


// Read in values to with the ADC
int adc_read(void);

float adc_to_ir(int x);


#endif /* ADC_H_ */
