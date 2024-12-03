/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Mearged Spring 2019 version with Fall 2018
 */
 


//The buttons are on PORTE 3:0
// GPIO_PORTE_DATA_R -- Name of the memory mapped register for GPIO Port E, 
// which is connected to the push buttons
#include "button.h"
#define BUTTON_PORT GPIO_PORTE_DATA_R

// Global varibles
volatile int button_event;
volatile int button_num;

void gpioe_handler();

/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

    // 1) Turn on PORTE system clock, do not modify other clock enables
    SYSCTL_RCGCGPIO_R |= 0x0000010;

    // 2) Set the buttons as inputs, do not modify other PORTE wires
    GPIO_PORTE_DIR_R &= 0x11111110;
    GPIO_PORTE_AFSEL_R &= 0x11111110;

    // 3) Enable digital functionality for button inputs

    GPIO_PORTE_DEN_R |= 0x0000000F;
//    GPIO_PORTE_DR2R_R &= 0x11111110; // maybe need?

    // configure gpio to read interrupts


//    GPIO_PORTE_IS_R

//    GPIO_PORTE_IBE_R

//    GPIO_PORTE_EV_R_ = 0;

    // set the masks to 1
//    GPIO_PORTE_IM_R |= 0x000000FF;
    initialized = 1;
}



/**
 * Initialize and configure PORTE interupts
 */
void init_button_interrupts() {

//    #warning: "Unimplemented function: void init_button_interrupts() -- You must configure GPIO to detect interrupts" // delete warning after implementing
    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // 1) Mask the bits for pins 0-3
    GPIO_PORTE_IM_R |= 0x0000000F;

    // 2) Set pins 0-3 to use edge sensing
    GPIO_PORTE_IS_R &= 0xFFFFFFF0;

    // 3) Set pins 0-3 to use both edges. We want to update the LCD
    //    when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0x0000000F;

    // 4) Clear the interrupts
    GPIO_PORTE_ICR_R = 0xFFFFFFFF;

//    // 5) Unmask the bits for pins 0-3
//    GPIO_PORTE_IM_R |= 0x0000000F;

//    #warning: "Unimplemented function: void init_button_interrupts() -- You must configure interrupts" // delete warning after implementing
    // TODO: Complete code below
    // 6) Enable GPIO port E interrupt
    NVIC_EN0_R |= 0x00000010;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
    printf("INSIDE INIT INTERRUPT");
}

/**
 * Helper method to return which button is being pressed.
 * The pressed button(s) will be returned in the least significant nibble,
 * represented by the active low.
 */
uint8_t button_checkButton(){
    char val = (BUTTON_PORT) & (0x0F);
    return val; //return if button is pressed or not
}

/**
 * Interrupt handler -- executes when a GPIO PortE hardware event occurs (i.e., for this lab a button is pressed)
 */
void gpioe_handler() {

//#warning: "Unimplemented function: void gpioe_handler() -- You must configure interrupts" // delete warning after implementing
    // Clear interrupt status register, meaning it got handled
//     update button event
    button_event = 1;
    printf("INSIDE HANDLER\n");
    button_num = button_getButton();
     GPIO_PORTE_ICR_R = 0x000000FF;
}




/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the rightmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {

    if (!((BUTTON_PORT >> 3) & 1 )) {
        return 0x04;
    }
    if (!((BUTTON_PORT >> 2) & 1)) {
        return 0x03;
    }
    if (!((BUTTON_PORT >> 1) & 1)) {
        return 0x02;
    }
    if (!(BUTTON_PORT & 0b1)) {
        return 0x01;
    }
    return 0x00;
	
}

/**
 * Helper method, extracted for debugging.
 * Used for polling.
 */
void waitForPress() {
    while (button_checkButton() != 0x0F) {
        // do nothing;
    }
    // leave the loop if button is pressed
    return;
}



