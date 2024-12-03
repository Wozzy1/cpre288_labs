/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"

extern volatile  char uart_data;
extern volatile  char flag;




void uart_init(void){

    // Enable uart for module 1
    SYSCTL_RCGCUART_R |= 0x00000002;

    // Enable clock for module 1 / port b
    SYSCTL_RCGCGPIO_R |= 0x00000002;

    // use the peripherals for wires 0, 1
    GPIO_PORTB_AFSEL_R |= 0x00000003;
    GPIO_PORTB_PCTL_R |= 0x00000011;
    GPIO_PORTB_DEN_R |= 0x0000003;
    GPIO_PORTB_DIR_R = 0x00000001;

//    GPIO_PORTB_AHB_DR2R_R |= 0x00000003;


    UART1_CTL_R &= 0xFFFFFCFE;
    // want to be 115200

    UART1_IBRD_R = 0x00000008;
    UART1_FBRD_R = 0x00000034; // should be 0x2c

    // 0b0110000
    UART1_LCRH_R = 0x00000060;

    UART1_CC_R = 0x00000000;

    UART1_CTL_R |= 0x00000301;




}

void uart_sendChar(char data){

    //wait here as long as the FIFO is Full
    while(UART1_FR_R & 0x00000020) {
        // wait
    }
    //send data
    UART1_DR_R = data;

}

char uart_receive(void){
    char data = 0;
    // keep waiting as long as FIFO is empty
    while(UART1_FR_R & 0x00000010)
    {
    }
    //mask the 4 error bits and grab only 8 data bits
    // See datasheet
    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendStr(const char *data){
    int i = 0;
    while (data[i]) {
        uart_sendChar(data[i++]);
    }
}

void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x00000010; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    // 0b0100.0000
    NVIC_EN0_R |= 0x00000040; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
    printf("INSIDE INTERRUPT\n");

    if (UART1_MIS_R & 0x00000010) {

        // copy data to volatile variable
        uart_data = (char) (UART1_DR_R & 0xFF);

        // set flag true, so outside code can execute
        flag = 1;

        // clear interrupt reg
        UART1_ICR_R &= 0x00000010;
    }

// STEP1: Check the Masked Interrup Status

//STEP2:  Copy the data

//STEP3:  Clear the interrup

}

