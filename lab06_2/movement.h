/*
 * movement.h
 *
 *  Created on: Sep 9, 2024
 *      Author: diepw50
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

double move_forward_with_bump(oi_t *sensor, int centimeters, int *bumpStatus);
double move_straight_forward(oi_t *sensor, int centimeters);
void turn_clockwise(oi_t *sensor, int degrees);
int traverse(oi_t *sensor, unsigned int distanceToTravel);

#endif /* MOVEMENT_H_ */
