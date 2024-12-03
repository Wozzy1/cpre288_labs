///*
// * ping.c
// *
// *  Created on: Nov 1, 2024
// *      Author: diepw50
// */
//
//
//#include"ping.h"
//#include "driverlib/interrupt.h"
//
//volatile unsigned int rising_time;
//volatile unsigned int falling_time;
//volatile unsigned int delta_time;
//volatile unsigned int overflow = 0;
//volatile enum {LOW, HIGH, DONE} state;
//
///**
// * Pages
// * 723 - input edge count
// * 656 - GPIO init
// * 688 - GPIO PCTL
// * 1351 - AFSEL functions
// */
//void ping_init(void) {
//
//    SYSCTL_RCGCGPIO_R |= 0x00000002;    // port b clock on
//    timer_waitMillis(1);
//    SYSCTL_RCGCTIMER_R |= 0x00000008;   // bit 3 - clock 3 ON
//    GPIO_PORTB_DEN_R |= 0x00000008;     // pin 3 EN
//    GPIO_PORTB_DIR_R |= 0x00000008;     // pin 3 output
//
//    GPIO_PORTB_AFSEL_R |= 0x00000008;   // pin 3 use alt function
//    GPIO_PORTB_PCTL_R |= 0x00007000;    // ping 3 bits 14:12 set
//    GPIO_PORTB_PCTL_R &= ~0x00008000;   // pin 3 bit 15 cleared
//
//    //init timer 3
//    TIMER3_CTL_R |= 0x00000D00;     // enables the timer 3B
//    TIMER3_CTL_R &= ~0x00000D00;    // disables the timer 3B
//    TIMER3_CFG_R |= 0x00000004;     // enables 16b or 32b timer
//    TIMER3_TBMR_R |= 0x00000007;    // sets to input edge time, capture mode
//    TIMER3_TBMR_R &= 0x000000EF;
//
//    TIMER3_TBILR_R |= 0x0000FFFF;   // initialize timer to max time
//    TIMER3_TBPR_R |= 0x000000FF;    // from lab doc
//    TIMER3_IMR_R |= 0x00000400;     // enable interrupts for capture event
//
//    TIMER3_CTL_R |= 0x00000D00;     // enables the timer 3B
//
//    NVIC_EN1_R |= 0x10;
//    IntMasterEnable();
//    IntRegister(INT_TIMER3B, TIMER3B_Handler);
//}
//
//void TIMER3B_Handler() {
//    if (TIMER3_MIS_R & 0x400) {
//        // clear ICR
//
//        if (state == LOW) {
//            state = HIGH;
//            rising_time = TIMER3_TBR_R & 0xFFFF;
//        }
//        else if (state == HIGH) {
//            state = DONE;
//            falling_time = TIMER3_TBR_R & 0xFFFF;
//        }
//    }
//    TIMER3_ICR_R |= 0x400;
//}
//
//
////void send_pulse() {
////    TIMER3_CTL_R &= ~0x100;     // disable clock
////    GPIO_PORTB_AFSEL_R &= ~0x8; // disable afsel
////    TIMER3_IMR_R &= ~0x400;     // disable timer interrupt
////
////    GPIO_PORTB_DATA_R &= ~0x8;  // clear data to be safe
////    GPIO_PORTB_DIR_R |= 0x8;    // set dir of wire 3 to out
////    GPIO_PORTB_DATA_R |= 0x8;   // set wire 3 to high
////    timer_waitMicros(5);        // wait 5 micros
////    GPIO_PORTB_DATA_R &= ~0x8;  // set wire 3 to low
////    GPIO_PORTB_DIR_R &= ~0x8;   // set dir of wire 3 to in
////
////    GPIO_PORTB_AFSEL_R |= 0x8;  // enable AFSEL
////    TIMER3_ICR_R |= 0x400;      // clear interrupts to be safe
////    TIMER3_CTL_R |= 0x100;      // enable clock
////    TIMER3_IMR_R |= 0x400;      // enable interrupt
////
////}
//
//void send_pulse(void)
//{
//    TIMER3_CTL_R &= ~0x100;                 // Disable clock
//    TIMER3_IMR_R &= ~0x400;                 // Disable interrupt
//    GPIO_PORTB_AFSEL_R &= ~0x8;              // Disable AFSEL
//
//    GPIO_PORTB_DIR_R |= 0x08;               // Set PB3 as output
//    GPIO_PORTB_DATA_R &= ~0x08;              // Set PB3 to low
//    GPIO_PORTB_DATA_R |= 0x08;              // Set PB3 to high TRIGGERS
//    timer_waitMicros(5);
//    GPIO_PORTB_DATA_R &=  ~0x08;              // Set PB3 to low TRIGGER
//    GPIO_PORTB_DIR_R &=  ~0x08;               // Set PB3 as input
//
//    GPIO_PORTB_AFSEL_R |= 0x8;              // Re enable AFSEL
//    TIMER3_ICR_R |= 0x400;                  // Clear interrupts to be safe
//    TIMER3_CTL_R |= 0x100;                  // Enable clock
//    TIMER3_IMR_R |= 0x400;                  // Enable interrupt
//
//}
//
//
//
//
///**
// * To perform a read,
// * 1) mask timer interrupts
// * 2) config PB3 for output signal (GPIO DIR, AFSEL, PCTL, etc)
// * 4) transmit signal pulse
// * 5) config PB3 for input
// * 6) clear timer interrupt
// * 7) unmask timer interrupt
// * 8) store TIMER3_TBR_R in a variable
// * 9) wait & catch falling edge
// */
//float ping_read(void) {
//
//    state = LOW;
//    send_pulse();
//
//    while (state != DONE) {
//
//    }
//
//    if (rising_time > falling_time) {
//        delta_time = rising_time - falling_time;       // Clocks per cycle
//    } else { // overflow
//        //delta_time = (65535 - fall_time) + rise_time; // Max val - fall, + rise
//        delta_time = rising_time - falling_time + (1 << 16);
//        overflow++;
//    }
//
//    float period = .0000000625;
//    float time = delta_time * period;       // calculate time elapsed in seconds (for demo 3 would just need to divide by 1000 and print to LCD)
//    float cm = (time / 2) * 34000.0;        // divide by 2 cuz sound goes there and back, then *34000 because speed in cm/s (time/2 * speed)
//
//    return cm;
//
//    // wait for pulse to come back
//
//}
//
//
//
//
//
/*
 * ping.c
 *
 *  Created on: Oct 29, 2024
 *      Author: lstuckly
 */

#include "ping.h"
#include "driverlib/interrupt.h"


volatile enum {LOW, HIGH, DONE} state;
volatile char interrupt_flag;
volatile unsigned int rise_time;
volatile unsigned int fall_time;
volatile unsigned int dif_time;
volatile unsigned int overflow = 0;

void ping_init(void)
{
    // GPIO stuff
    SYSCTL_RCGCGPIO_R |= 0x02;      // Enable port B
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    SYSCTL_RCGCTIMER_R |= 0x8;      // Enable timer 3
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_DIR_R |= 0x08;       // set wire 3 as output
    GPIO_PORTB_AFSEL_R |= 0x8;      // use Timer for wire 3
    GPIO_PORTB_PCTL_R |= 0x7000;       // Use T3CCP1
    GPIO_PORTB_DEN_R |= 0x08;       // enable digital for wire 3

    // Timer stuff
    TIMER3_CTL_R |= 0xD00;          // Enable Timer3B and select both edges was 7
    TIMER3_CFG_R |= 0x4;            // Set bits for 16 bit mode
    TIMER3_CFG_R &= 0x4;            // Clear bits for 16 bit mode
    TIMER3_TBMR_R |= 0x7;           // Sets Capture mode and Edge-Time mode
    TIMER3_TBMR_R &= 0xEF;          // Timer counts down
    TIMER3_TBILR_R |= 0xFFFF;       // From lab doc set upper bound
    TIMER3_TBPR_R |= 0xFF;          // From lab doc
    TIMER3_IMR_R |= 0x400;          // Enable capture interrupt

    //Interrupts
    NVIC_EN1_R |= 0x10;             // Enable interrupt for Timer 3B
    IntMasterEnable();
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
}

void TIMER3B_Handler(void)
{
    if (TIMER3_MIS_R & 0x400)
    {
        if (state == LOW) // if state was low, flip high (beginning)
        {
            state = HIGH;
            rise_time = TIMER3_TBR_R;
        }
        else if (state == HIGH) // If state was high, flip low
        {
            state = DONE;
            fall_time = TIMER3_TBR_R;
        }
    }
    TIMER3_ICR_R |= 0x400;      //clear interrupt

}
/* TODO
 * TIMER3_TBR_R is clock cycles
 * 16 MHz is frequency
 * Find time from clock cycles * period
 * period = 1/f  = 1/16e6 = 6.25e-8 = .0000000625
 * time = dif_time * period
 */




float ping_read(void)
{
    float cm = 0;
    float period = .0000000625;                // 1/f = 1/16e6
    float time = 0;
    state = LOW;
    send_pulse();                               // Sends pulse
    while (state != DONE)
    {
                                                // Wait for ISR to capture rising and falling edge times
    }

    if (rise_time > fall_time)
    {
        dif_time = rise_time - fall_time;       // Clocks per cycle
    }
    else                                        // Overflow
    {
        //dif_time = (65535 - fall_time) + rise_time; // Max val - fall, + rise
        dif_time = rise_time - fall_time + (1 << 16);
        overflow++;
    }

    time = dif_time * period;                   // Calculate time elapsed in seconds (for demo 3 would just need to divide by 1000 and print to LCD)
    cm = (time / 2) * 34000.0;                  // Divide by 2 bc sound goes there and back, then *34000 because speed in cm/s (time/2 * speed)

    return cm;
}

void send_pulse(void)
{
    TIMER3_CTL_R &= ~0x100;                 // Disable clock
    TIMER3_IMR_R &= ~0x400;                 // Disable interrupt
    GPIO_PORTB_AFSEL_R &= ~0x8;              // Disable AFSEL

    GPIO_PORTB_DIR_R |= 0x08;               // Set PB3 as output
    GPIO_PORTB_DATA_R &= ~0x08;              // Set PB3 to low
    GPIO_PORTB_DATA_R |= 0x08;              // Set PB3 to high TRIGGERS
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &=  ~0x08;              // Set PB3 to low TRIGGER
    GPIO_PORTB_DIR_R &=  ~0x08;               // Set PB3 as input

    GPIO_PORTB_AFSEL_R |= 0x8;              // Re enable AFSEL
    TIMER3_ICR_R |= 0x400;                  // Clear interrupts to be safe
    TIMER3_CTL_R |= 0x100;                  // Enable clock
    TIMER3_IMR_R |= 0x400;                  // Enable interrupt

}











