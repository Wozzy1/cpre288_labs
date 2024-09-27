

/**
 * main.c
 */
#include<stdio.h>
#include"timer.h"
#include"cyBot_Scan.h"
#include"cyBot_uart.h"

#include"lcd.h"

void perform_scan(cyBOT_Scan_t* scanPtr);
void calibrate();
void smooth_data_inplace(float *data);

typedef struct obj_struct {
    int startAngle;
    int endAngle;
    float distance;

} object;

int main(void)
{


    cyBOT_Scan_t scanInfo;
    cyBOT_Scan_t* scan_ptr = &scanInfo;
    cyBot_uart_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);

    calibrate();

    perform_scan(scan_ptr);
	return 0;
}


void calibrate() {
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    right_calibration_value = 222250; // for bot 4
    left_calibration_value = 1172500;
//      right_calibration_value = 348250; // for bot 9
//      left_calibration_value = 1356250;
    //cyBOT_SERVO_cal();
}


void perform_scan(cyBOT_Scan_t* scanPtr) {
    int degrees = 180;
//    fclose(fopen("scan_result.txt", "w"));

    // printing initial header msg
    char init_msg[50];
    sprintf(init_msg, "%-15s %-15s\r\n", "Degrees", "Distance (cm)");
    int x = 0;
    while (init_msg[x]) {
        cyBot_sendByte(init_msg[x++]);
    }
    printf("%s\n", init_msg);

    float values[91];

    // SCAN 180 DEGREE
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

    int objCount = 0;
    int* objCountptr = &objCount;
    object* ptr = (object*) calloc(1, sizeof(object));


    // take average of 5 data points. if current data is outside of threshold,
    // set data to avg
//    smooth_data_inplace(values);


    // PRINTING OUTPUTS TO FILE AND CONSOLE
    int j;
    for (j = 0; j < 91; j++) {
        char msg[50];

        sprintf(msg, "%-15d %-1.2f", (180 - 2*j), values[j]);
        int i = 0;
        while (msg[i]) {
            cyBot_sendByte(msg[i++]);
        }
        cyBot_sendByte('\r');
        cyBot_sendByte('\n');
        printf("%s\n", msg);
    }

    countObjects(values, ptr, objCountptr);


    // PRINTING COUNT TO CONSOLE
    char debug[50];
    sprintf(debug, "\nCount: %d", objCount);
    int i = 0;
    while (debug[i] != '\0') {
        cyBot_sendByte(debug[i++]);
    }
    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
    printf("%s", debug);

}

void smooth_data_inplace(float *data) {
    int i, j;
    for (i = 0; i < 90; i++) {
        float sum = 0;
        int count = 0;

        // create sliding window of width 3
        for (j = i - (1); j <= i + (1); j++) {
            if (j >= 0 && j < 90) {
                sum += data[j];
                count++;
            }
        }

        // perform something on the value at the center of the average
        float avg = sum / count;
//        printf("%f / % d = %f\n", sum, count, avg);
        if (abs(data[i] - avg) > 20) {
            data[i] = avg;
        } else {
            data[i] = sum / count; // Replace current value with smoothed value
        }
    }
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



