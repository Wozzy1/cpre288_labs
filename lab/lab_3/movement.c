#include "open_interface.h"

/**
 * if bumpLeft return -1
 * if bumpRight return 1
 * if no bump return 0
 *
 * bumpStatus pointer allows move_forward to record what happens during the method call
 * this allows traverse() to read what happens without having to return a different value
 * than the distance traveled.
 */
double move_forward_with_bump(oi_t *sensor, int centimeters, int *bumpStatus) {
    double sum = 0;

    if (centimeters > 0) {
        oi_setWheels(500, 500);
    }
    else {
        oi_setWheels(-500, -500);
    }
    while (sum < abs(centimeters) * 10) {
        oi_update(sensor);
        if (sensor->bumpLeft) {
            *bumpStatus = -1;
            break;
        }
        else if (sensor->bumpRight) {
            *bumpStatus = 1;
            break;
        }

        // distance measured in mm
        sum += abs(sensor->distance);
    }
    // since we break we didnt call it again
    sum += abs(sensor->distance);


    // divide by ten to convert back to cm from mm
    return sum / 10;
}

double move_straight_forward(oi_t *sensor, int centimeters) {
    double sum = 0;

    if (centimeters > 0) {
        oi_setWheels(500, 500);
    }
    else {
        oi_setWheels(-500, -500);
    }
    while (sum < abs(centimeters) * 10) {
        oi_update(sensor);

        // distance measured in mm
        sum += abs(sensor->distance);
    }

    // divide by ten to convert back to cm from mm
    return sum / 10;
}

void turn_clockwise(oi_t *sensor, int degrees) {
    // +, - wheels -> counter clockwise
    // -, + wheels -> clockwise

    if (degrees > 0) {
        oi_setWheels(-50, 50);
    }
    else {
        oi_setWheels(50, -50);
    }

    double deg = (double) abs(degrees);
    while (deg > 0) {
        oi_update(sensor);

        /* the higher the coefficient, the less weight a degree has */
        deg -= (1.55) * abs(sensor->angle);
        /* close enough to 90, give or take 5-10 degrees
         * value 1.55 for bot 2041-06
         */
    }
}

/*
 * distance to travel in cm
 */
int traverse(oi_t *sensor, unsigned int distanceToTravel) {

    /*
     travel incremental amounts until 2m or collision

     if bumpLeft, then turn 90deg clockwise
     else if bumpRigth then turn 90deg counterclockwise (-90deg)

     bumpLeft:
        move_forward -15cm
        turn_clockwise 90deg
        move_forward 25cm
        turn_clockwise -90deg
        resume traverse

    bumpRight:
        move_forward -15cm
        turn_clockwise -90deg
        move_forward 25cm
        turn_clockwise 90deg
        resume traverse

    bumpLeft && bumpRight:
        default bumpLeft
     */
    int *bumpStatus;

    while (distanceToTravel > 0) {
        distanceToTravel -= move_forward_with_bump(sensor, 25, bumpStatus);

//        oi_update(sensor);
        /* the first if statement catches if LEFT && RIGHT */
        if (*bumpStatus == -1) {
            move_straight_forward(sensor, -15);
            turn_clockwise(sensor, 90);
            move_straight_forward(sensor, 25);
            turn_clockwise(sensor, -90);
        }
        else if (*bumpStatus == 1) {
            move_straight_forward(sensor, -15);
            turn_clockwise(sensor, -90);
            move_straight_forward(sensor, 25);
            turn_clockwise(sensor, 90);
        }
        oi_update(sensor);

        // resumes next loop
    }

    return 0;

}
