

/**
 * main.c
 */
#include<stdio.h>
#include "open_interface.h"
#include"timer.h"
#include"cyBot_Scan.h"
#include"cyBot_uart.h"
#include"scan.h"
#include"movement.h"

#include"lcd.h"

typedef struct obj_struct {
    int startAngle;
    int endAngle;
    float distance;

} object;

void perform_scan(cyBOT_Scan_t* scanPtr);
void calibrate();
void smooth_data_inplace(float *data);
int count(float* data, object* objArray);
void face_smallest_object(object* objArray, int size, cyBOT_Scan_t* scanPtr);


void printString(char* str) {
    int x = 0;
    while (str[x]) {
        cyBot_sendByte(str[x++]);
    }
    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
    printf("%s\n", str);
}

int main(void)
w{


    cyBOT_Scan_t scanInfo;
    cyBOT_Scan_t* scan_ptr = &scanInfo;
    cyBot_uart_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);

    calibrate();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    char input = '0x00';
    while (input != 'q') {
        perform_scan(scan_ptr);
        input = cyBot_getByte();

        if (input == 'w') {
            move_straight_forward(sensor_data, 10);
        } else if (input == 's') {
            move_straight_forward(sensor_data, -10);
        } else if (input == 'd') {
            turn_clockwise(sensor_data, 10);
        } else if (input == 'a') {
            turn_clockwise(sensor_data, -10);
        }

        oi_setWheels(0, 0);  // stop
    }

	return 0;
}


void calibrate() {
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
//    right_calibration_value = 222250; // for bot 4
//    left_calibration_value = 1172500;
//      right_calibration_value = 348250; // for bot 9
//      left_calibration_value = 1356250;
      right_calibration_value = 274750; // for bot 1 downstairs
      left_calibration_value = 1219750;

//    cyBOT_SERVO_cal();
}


void perform_scan(cyBOT_Scan_t* scanPtr) {
    int debugMode = 0;

    int degrees = 180;

    // printing initial header msg
    char init_msg[50];
    sprintf(init_msg, "%-15s %-15s", "Angle (Degrees)", "Distance (cm)");
    printString(init_msg);

    float values[91];

    // SCAN 180 DEGREE
    if (!debugMode) {
        while (degrees >= 0) {
            float temp = 0;
            cyBOT_Scan(degrees, scanPtr);
            temp += (*scanPtr).sound_dist;

            cyBOT_Scan(degrees, scanPtr);
            temp += (*scanPtr).sound_dist;

            cyBOT_Scan(degrees, scanPtr);
            temp += (*scanPtr).sound_dist;

            values[90 - (degrees / 2)] = temp / 3;
            degrees -= 2;
        }
    }

    // INIT obj array with malloc
    int objCount = 0;
    object* ptr = (object*) calloc(25, sizeof(object));

    // PRINTING OUTPUTS TO FILE AND CONSOLE
    int j;
    for (j = 0; j < 91; j++) {
        char msg[50];

        if (!debugMode) {
            sprintf(msg, "%-15d %-1.2f", (180 - 2*j), values[j]); // Normal run
        } else {
            sprintf(msg, "%-15d %-1.2f", (180 - 2*j), scan[j]); // Debug run
        }

        printString(msg);
    }

    if (!debugMode) {
        objCount = count(values, ptr);
    } else {
        objCount = count(scan, ptr);
    }

    int objectIndex;
    char line[50];
    sprintf(line, "%-10s %-10s %-10s %-10s", "Object#", "Angle", "Distance", "Width");
    printString(line);

    // 20 - ARBITRARY value
    for (objectIndex = 0; objectIndex < 20; objectIndex++) {
        if (!ptr[objectIndex].startAngle) {
            break;
        }
        char line[50];
        sprintf(line, "%-10d %-10d %-10.2f %-10.2d",
                objectIndex+1,
                (ptr[objectIndex].endAngle + ptr[objectIndex].startAngle)/2,
                ptr[objectIndex].distance,
                ptr[objectIndex].endAngle - ptr[objectIndex].startAngle);
        printString(line);
    }

    // PRINTING COUNT TO CONSOLE
    char debug[50];
    sprintf(debug, "\nCount: %d", objCount);
    printString(debug);


    face_smallest_object(ptr, objectIndex, scanPtr);
}


void face_smallest_object(object* objArray, int size, cyBOT_Scan_t* scanPtr) {
    if (!objArray) {
        printString("\nNull pointer caught in face_smallest_object!!!");
        return;
    }

    int i;

    int minWidth = 0x7FFFFFFF;
    int minIndex;
    for (i = 0; i < size; i++) {
        int width = objArray[i].endAngle - objArray[i].startAngle;
        if (width < minWidth) {
            minWidth = width;
            minIndex = i;
        }
    }

    cyBOT_Scan((objArray[minIndex].endAngle + objArray[minIndex].startAngle)/2, scanPtr);

}
//void smooth_data_inplace(float *data) {
//    int i, j;
//    for (i = 0; i < 90; i++) {
//        float sum = 0;
//        int count = 0;
//
//        // create sliding window of width 3
//        for (j = i - (1); j <= i + (1); j++) {
//            if (j >= 0 && j < 90) {
//                sum += data[j];
//                count++;
//            }
//        }
//
//        // perform something on the value at the center of the average
//        float avg = sum / count;
////        printf("%f / % d = %f\n", sum, count, avg);
//        if (abs(data[i] - avg) > 20) {
//            data[i] = avg;
//        } else {
//            data[i] = sum / count; // Replace current value with smoothed value
//        }
//    }
//}


int count(float* data, object* objArray) {
    int objCount = 0;
    int i = 1;
    int inObject = 0;
    int openingAngle = 0;
    int closingAngle = 0;
    // add catch for if end-start < 6
    while (i < 89) {
        float prev = data[i-1];
        float next = data[i+1];
        if (abs(prev - next) > 5) {
            if (prev - next > 0 && !inObject) {
                inObject = 1;
                openingAngle = i * 2;
            }
            else if (prev - next < 0 && inObject) {
                inObject = 0;
                closingAngle = i * 2;

                objArray[objCount].startAngle = openingAngle;
                objArray[objCount].endAngle = closingAngle;
                objArray[objCount].distance = data[i];
                objCount++;
            }
        }
        i++;
    }

    return objCount;

}

int countObjects(float* dist, object* objArray, int* objCountptr) {

    int i, j;
    int opening = 1;
    for (i = 0; i < 90; i++) {
        float sum = 0;
        int count = 0;
        for (j = i - 1; j <= i + 1; j++) {
            if (j >= 0 && j < 90) {
                sum += dist[j];
                count++;
            }
        }
        float avg = sum / count;

        if (abs(dist[i] - avg) > 15) {
            printf("TEMPPM");
        }

        if (opening && abs(dist[i] - avg) > 15) {
            // reallocate more memory - starts at 0, when see first obj inc to size 1
            objArray[*objCountptr].distance = dist[i];
            objArray[*objCountptr].startAngle = i * 2;
            opening = 0;
        }
        else if (!opening && abs(dist[i] - avg) > 15) {
            objArray[*objCountptr].endAngle = i * 2;
            (*objCountptr)++;

            object* temp = objArray;
            objArray = realloc(objArray, ((*objCountptr) + 1) * sizeof(objArray));
            if (!objArray) {
                printf("Error with memory reallocation.\n");
                objArray = temp;
            }
            opening = 1; // denote end of object


        }

    }

    return 0;
}



