/*
 * adc.c
 *
 *  Created on: Oct 22, 2024
 *      Author: lstuckly
 */


#include "adc.h"
#include "timer.h"

//extern int raw[];
//extern float ping[];

void adc_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x2A;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    SYSCTL_RCGCADC_R |= 0x1;        // ADC0
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_DIR_R &= 0xEF;       // set as input
    GPIO_PORTB_AFSEL_R |= 0x10;     // use peripheral for wire 4
    GPIO_PORTB_AMSEL_R |= 0x10;     // Set wire 4 for AIN10
    GPIO_PORTB_DEN_R &= 0xEF;       // clear wire 4 for AIN10
    GPIO_PORTB_ADCCTL_R &= 0x00;    // Clear
    ADC0_ACTSS_R |= 0x1;            // use SS0
    ADC0_EMUX_R &= 0x0;             // Clear EM0
    ADC0_SSPRI_R |= 0x3330;         // Sets SS1-3 to low priority
    ADC0_SSPRI_R &= 0xC;            // Sets SS0 to high priority
    ADC0_SSMUX0_R |= 0xA;           // Set MUX0 to use AIN10
    ADC0_SSMUX0_R &= 0xA;
    ADC0_SSCTL0_R |= 0x6;           // Set CTL0 to use raw interrupt and first sample is end of sequence
    ADC0_SSCTL0_R &= 0x6;
    ADC0_IM_R &= 0xE;               // Clear interrupt mask (set bit 0 to use interrupt status) slide 120
}


int adc_read(void)
{
    int info_read = 0;
    ADC0_PSSI_R |= 0x1;                     // Start sample conversion
    info_read = ADC0_SSFIFO0_R & 0xFFF;     // Grabs 12 bits, ignores register
    while(ADC0_RIS_R & !0x1)                // Wait for sample conversion complete
    {
           // waits
    }
    //ADC0_SAC_R // lecture slide 123

    return info_read;
}




/**

 * Configure my look up tables for IR values

 * to init: populate arrays of IR values and ping values

 * by scanning and moving backwards increments

 * The arrays are filled in where the lower the index,

 * the shorter the distance.

 *

 */

float adc_to_ir(int x) {

//    return 2e6 * pow(adc, -1.497);

    float slope;

    float dist = -1;

    if (raw[0] >= x && x >= raw[1]) {

        slope = (ping[1] - ping[0]) / (raw[1] - raw[0]);

        dist = slope * (x - raw[0]) + ping[0];

    }

    if (raw[1] >= x && x >= raw[2]) {

        slope = (ping[2] - ping[1]) / (raw[2] - raw[1]);

        dist = slope * (x - raw[1]) + ping[1];

    }

    if (raw[2] >= x && x >= raw[3]) {

        slope = (ping[3] - ping[2]) / (raw[3] - raw[2]);

        dist = slope * (x - raw[2]) + ping[2];

    }

    if (raw[3] >= x && x >= raw[4]) {

        slope = (ping[4] - ping[3]) / (raw[4] - raw[3]);

        dist = slope * (x - raw[3]) + ping[3];

    }

    if (raw[4] >= x && x >= raw[5]) {

        slope = (ping[5] - ping[4]) / (raw[5] - raw[4]);

        dist = slope * (x - raw[4]) + ping[4];

    }

    if (raw[5] >= x && x >= raw[6]) {

        slope = (ping[6] - ping[5]) / (raw[6] - raw[5]);

        dist = slope * (x - raw[5]) + ping[5];

    }

    if (raw[6] >= x && x >= raw[7]) {

        slope = (ping[7] - ping[6]) / (raw[7] - raw[6]);

        dist = slope * (x - raw[6]) + ping[6];

    }

    if (raw[7] >= x >= raw[8]) {

        slope = (ping[8] - ping[7]) / (raw[8] - raw[7]);

        dist = slope * (x - raw[7]) + ping[7];
    }

    if (raw[8] >= x && x >= raw[9]) {

        slope = (ping[9] - ping[8]) / (raw[9] - raw[8]);

        dist = slope * (x - raw[8]) + ping[8];

    }

    if (raw[9] >= x && x >= raw[10]) {

        slope = (ping[10] - ping[9]) / (raw[10] - raw[9]);

        dist = slope * (x - raw[9]) + ping[9];

    }

    if (raw[10] >= x && x >= raw[11]) {

        slope = (ping[11] - ping[10]) / (raw[11] - raw[10]);

        dist = slope * (x - raw[10]) + ping[10];

    }

    // last one, if the value is larger than the largest recorded,

    // extrapolate with the last linear line

//    if (raw[11] >= x && x >= raw[12]) {
    if (raw[11] >= x) {

        slope = (ping[12] - ping[11]) / (raw[12] - raw[11]);

        dist = slope * (x - raw[11]) + ping[11];

    }

    return dist;

}



