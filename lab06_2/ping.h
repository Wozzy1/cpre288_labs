/*
 * ping.h
 *
 *  Created on: Nov 1, 2024
 *      Author: diepw50
 */

#ifndef PING_H_
#define PING_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void ping_init(void);
void send_pulse(void);
void TIMER3B_Handler();
float ping_read(void);



#endif /* PING_H_ */
