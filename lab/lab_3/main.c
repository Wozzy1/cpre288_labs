

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
void smooth_data_inplace(float *data, int *objCount, int *objArr);

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
    fclose(fopen("scan_result.txt", "w"));

//    fopen("scan_result.txt", "r");

    // printing initial header msg
    char init_msg[50];
    sprintf(init_msg, "%-15s %-15s\r\n", "Degrees", "Distance (m)");
    int x = 0;
    while (init_msg[x]) {
        cyBot_sendByte(init_msg[x++]);
    }
    printf("%s\n", init_msg);


    float values[91];

    // begin sweep from 180 to 0
    while (degrees >= 0) {
        cyBOT_Scan(degrees, scanPtr);

        values[90 - (degrees / 2)] = (*scanPtr).sound_dist/100;

        degrees -= 2;

    }

//    int fk = 0;
//    for (; fk < 90; fk++) {
//        printf("%f\n", values[fk]);
//    }

    int objCount = 0;
    int *objCountPtr = &objCount;


    int objArr[10];


    smooth_data_inplace(values, objCountPtr, objArr);

    // take average of 5 data points. if current data is outside of threshold,
    // set data to avg

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
    }

    char debug[50];
    sprintf(debug, "\nCount: %d", &objCount);
    int i = 0;
    while (debug[i] != '\0') {
        cyBot_sendByte(debug[i++]);
    }
    cyBot_sendByte('\r');
    cyBot_sendByte('\n');


}

void smooth_data_inplace(float *data, int *objCount, int *objArr) {
    int i, j;

    int encounter = 0;
    int startDegree = 0;

    for (i = 0; i < 90; i++) {
        float sum = 0;
        int count = 0;

        // create sliding window of width 5
        for (j = i - (2); j <= i + (2); j++) {
            if (j >= 0 && j < 90) {
                sum += data[j];
                count++;
            }
        }

        // perform something on the value at the center of the average
        float avg = sum / count;


//        printf("%f / % d = %f\n", sum, count, avg);
        if (abs(data[i] - avg) > 0.15) {
            data[i] = avg;
            // inc count of objects seen if change in dist > 0.3m
            if (abs(data[i] - avg) > 0.3) {
                if (!encounter) {
                    encounter = 1;
                    startDegree = i * 2;
                    (*objCount)++;
                } else {
                    encounter = 0;
                    // recorded object number objCount-1 has angle width of start - stop
                    objArr[(*objCount) - 1] = startDegree - (i * 2);
                }
            }
        } else {
            data[i] = sum / count; // Replace current value with smoothed value
        }


//        data[i] = sum / count;
    }

}




