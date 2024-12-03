/*
 * servo.h
 *
 *  Created on: Nov 23, 2024
 *      Author: diepw50
 */

#ifndef SERVO_H_
#define SERVO_H_
#include <inc/tm4c123gh6pm.h>


void servo_init(void);
int servo_move(float degrees);
void servo_cal(void);



#endif /* SERVO_H_ */
