/*
 * servo.c
 *
 *  Created on: Nov 23, 2024
 *      Author: diepw50
 */

/**
 * Initial TIMER1B to be 24b countdown in PWM periodic mode
 */
#include "servo.h"
#include "Timer.h"
#include "button.h"
#include "lcd.h"

int match_value;
//int right_value = 0;
//int left_value = 0;

int right_value = 312000;
int left_value = 285067;


void servo_init(void)
{
    // GPIO
    SYSCTL_RCGCGPIO_R |= 0x02;      // enable port B
    timer_waitMillis(1);            // short pause
    SYSCTL_RCGCTIMER_R |= 0x2;      // enable timer 1
    timer_waitMillis(1);
    GPIO_PORTB_DIR_R &= ~0x20;      // set wire 5 as input
    GPIO_PORTB_AFSEL_R |= 0x20;     // use Timer for wire 5
    GPIO_PORTB_PCTL_R |= 0x700000;  // using T1CCP1
    GPIO_PORTB_DEN_R |= 0x20;       // enable wire 5

    // Timer
    TIMER1_CTL_R &= ~0xD00;         // disable Timer1B and disable both edge
    TIMER1_CFG_R |= 0x4;            // set bits for 16 bit mode
    TIMER1_CFG_R &= 0x4;            // clear bits for 16 bit mode
    TIMER1_TBMR_R |= 0xA;           // set PWM and periodic mode
    TIMER1_TBMR_R &= 0xEF;          // timer counts down
    TIMER1_TBILR_R = 0xE200;        //
    TIMER1_TBPR_R = 0x4;            // 320,000 = 0x4E200

    TIMER1_CTL_R |= 0xD00;          // enable Timer1B and select both edges
}

int servo_move(float degrees)
{
    TIMER1_CTL_R &= ~0xD00;         // Disabled Timer1B and disable both edge
    if (left_value == 0 || right_value == 0)
    {
        float ms = (degrees / 180) + 1;
        int pulse_width = ms / 1000 * 16000000;
        match_value = 320000 - pulse_width;
    }
    else
    {
        //right val is highest value, right - left = total dist, / by 180 to find distance for each deg, * by degree to get value for specific degree
        match_value = right_value - ((right_value - left_value) / 180 * degrees);
    }
    TIMER1_TBMATCHR_R = match_value;
    TIMER1_TBPMR_R = match_value >> 16;

    TIMER1_CTL_R |= 0xD00;          // Enable Timer1B and select both edges
    return 0;
}


void servo_cal(void)
{
    int degree = 90;
    servo_move(degree);
    char message[10];

    char mode = 0; // 0 = CW, 1 = CCW
    while (1)
    {
        int button = button_getButton();
        if (mode == 0) {
            sprintf(message, "CW");
        } else {
            sprintf(message, "CCW");
        }
        lcd_printf("Direction: %s\nMV: %d", message, match_value);
        if (button == 1)
        {
            if (mode == 0) // clockwise
            {
                degree -= 1;
            }
            else // countclockwise
            {
                degree += 1;
            }
            servo_move(degree);
        }
        else if (button == 2)
        {
            if (mode == 0) // CW
            {
                degree -= 5;
            }
            else // CCW
            {
                degree += 5;
            }
            servo_move(degree);
        }
        else if (button == 3)
        {
            mode = 1 - mode;
        }
        else if (button == 4)
        {
            break;
        }
        timer_waitMillis(200);
    }
}
