/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Describtion: Added timer_init call, and including Timer.h
 */
#include <string.h>
#include<stdio.h>
#include "Timer.h"
#include "lcd.h"



void printSpaces(int num, int length);



int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the the LCD screen.  This also clears the screen.

	// Clear the LCD screen and print "Hello, world" on the LCD
//	lcd_printf("Hello, world");

//	 lcd_puts("Hello, world");// Replace lcd_printf with lcd_puts
                                 // step through in debug mode
                                 // and explain to TA how it
                                 // works

	 char phrase[100] = "Microcontrollers are lots of fun!";
	 int phraseLength = strlen(phrase);


     int firstIndex = 0;
     while(1) {
         // after all the letters, return the pointer to beginning of phrase
         if (firstIndex >= phraseLength) {
             firstIndex = 0;
         }



//       printSpaces(firstIndex, length);


         char spaces[100];;
         int x = 0;
         for (; x < 19 - firstIndex; x++) {
             spaces[x] = ' ';
         }
         spaces[x] = '\0';


         strcat(spaces, phrase);
         lcd_printf("%.20s", spaces);
         firstIndex++;
         if (firstIndex == 20) {
//             printf(spaces);
             while (firstIndex < phraseLength) {
                 int fuck = 0;
                 while (spaces[fuck] != '\0') {
                     spaces[fuck] = spaces[fuck + 1];
                     fuck++;
                 }
                 lcd_printf("%.20s", spaces);
                  firstIndex++;
                  timer_waitMillis(300);

             }

             while (spaces[0] != '\0') {
                 int fuck = 0;
                 while (spaces[fuck] != '\0') {
                     spaces[fuck] = spaces[fuck + 1];
                     fuck++;
                 }
                 lcd_printf("%.20s", spaces);
                 timer_waitMillis(300);
             }
         }

         timer_waitMillis(300);


     }




	 // for i until 20 - index print spaces then start printing letters



    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
    // NOTE: For time fuctions see Timer.h

	return 0;
}


void printSpaces(int num, int length) {
    int i = 0;
    for(; i < length - num; i++) {
        lcd_putc(' ');
    }

}
