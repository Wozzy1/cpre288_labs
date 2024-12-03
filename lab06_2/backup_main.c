//#include<stdio.h>
//#include "open_interface.h"
//#include"timer.h"
////#include"cyBot_Scan.h"
//#include"uart.h"
//#include"scan.h"
//#include"movement.h"
//#include"adc.h"
//#include"servo.h"
//#include"button.h"
//#include"lcd.h"
//#include"ping.h"
//
//typedef struct obj_struct {
//    int startAngle;
//    int endAngle;
//    float distance;
//
//} object;
//
//void printString(char* str);
//void smoothArrayInPlace(float* array, int size, int windowSize);
//int perform_scan(cyBOT_Scan_t* scanPtr, oi_t* sensor_data);
//int count(float irArray[], float pingArray[], object objArray[]);
//int face_smallest_object(object objArray[], int size, cyBOT_Scan_t* scanPtr, oi_t* sensor);
//double calculate(int ir);
//void calibrate();
//
//volatile char flag;
//volatile char uart_data;
//extern int currentGear;
//
//int main_() {
//    cyBOT_Scan_t scanInfo;
//    cyBOT_Scan_t* scan_ptr = &scanInfo;
//    uart_init();
//    adc_init();
////    uart_interrupt_init();
////    lcd_init();
////    cyBOT_init_Scan(0b0111);
//    calibrate();
//
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
//
//
//    int toggleDriveMode = 0;
//    currentGear = 1;
//    char input = 0x00;
//    int dist = 1;
//    while (input != 'q') {
////    while (uart_data != 'q') {
//        int tempStatus = 0;
//        // add function work checking uart for 5 seconds between scans,
//        // if no input, then continue
//        while (toggleDriveMode && !tempStatus) {
//            currentGear = 1;
//            tempStatus = perform_scan(scan_ptr, sensor_data);
//            move_straight_forward(sensor_data, dist);
//            oi_setWheels(0, 0);
//        }
//        toggleDriveMode = 0;
////        printf("%d\n", flag);
//        if (1) {
////            input = uart_data;
//            switch (input) {
//            // toggle auto vs manual
//            case 'h':
//                toggleDriveMode = !toggleDriveMode;
//                break;
//            // toggle first and second gear
//            case 'w':
//                move_straight_forward(sensor_data, dist);
//                break;
//            case 's':
//                move_straight_forward(sensor_data, -dist);
//                break;
//            case 'a':
//                turn_clockwise(sensor_data, -dist);
//                break;
//            case 'd':
//                turn_clockwise(sensor_data, dist);
//                break;
//            case 'm':
//                perform_scan(scan_ptr, sensor_data);
//            case '1':
//                currentGear = 1;
//                break;
//            case '2':
//                currentGear = 2;
//                break;
//            case '3':
//                currentGear = 3;
//                break;
//            case '4':
//                currentGear = 4;
//                break;
//            }
//            oi_setWheels(0, 0);
//            flag = 0;
//        }
//
//        lcd_clear();
//        lcd_putc('D');
//        lcd_putc('r');
//        lcd_putc(' ');
//
//        if (toggleDriveMode) {
//            lcd_putc('A');
//            dist = 1;
//        } else {
//            lcd_putc('M');
//            lcd_putc(' ');
//            lcd_putc('G');
//            lcd_putc('r');
//            lcd_putc(' ');
//            switch (currentGear) {
//            case 4:
//                lcd_putc('4');
//                dist = 20;
//                break;
//            case 3:
//                lcd_putc('3');
//                dist = 10;
//                break;
//            case 2:
//                lcd_putc('2');
//                dist = 5;
//                break;
//            case 1:
//                lcd_putc('1');
//                dist = 1;
//                break;
//            }
//        }
//
//        oi_setWheels(0, 0);
//        input = uart_receive();
//    }
//
//
//}
//
///**
// * Performs 180 degree sweep, notes objects, turns to the smallest one.
// *
// * Return boolean value of within 6cm of object or not.
// *
// */
//int perform_scan(cyBOT_Scan_t* scanPtr, oi_t* sensor_data) {
//
//    int debugMode = 0;
//
//    // =======
//    // printing header
//    char init_msg[50];
//    sprintf(init_msg, "%-15s %-15s %-15s", "Angle (Degrees)", "DistIR (cm)", "Dist PING (cm)");
//    printString(init_msg);
//    free(init_msg);
//    // =======
//
//    // =======
//    // init arrays and perform scan
//    float irValues[91] = {0};
//    float pingValues[91] = {0};
//    int degrees = 0;
//    if (!debugMode) {
//
//        while (degrees <= 180) {
//            float adjustedIR = 0;
//            float pingDistance = 0;
//            float tempIR = 0;
//            int adc = 0;
//
//            // scan and record values
//            cyBOT_Scan(degrees, scanPtr);
//            adjustedIR += calculate((*scanPtr).IR_raw_val);
////            adc = adc_read();
////            tempIR = adc_to_ir(adc);
////            tempIR = tempConvert(adc); TODO UPDATE THIS WITH LAB 8 CONVERT
//            adjustedIR += tempIR;
//
//            pingDistance += (*scanPtr).sound_dist;
//
//            cyBOT_Scan(degrees, scanPtr);
//            adjustedIR += calculate((*scanPtr).IR_raw_val);
////            adc = adc_read();
////            tempIR = adc_to_ir(adc);
////            tempIR = tempConvert(adc);
//            adjustedIR += tempIR;
//
//            pingDistance += (*scanPtr).sound_dist;
//
//            // storing calculated averages in arrays
//            irValues[degrees / 2] = (adjustedIR / 2 >= 85) ? 85.0 : adjustedIR / 2;
//            pingValues[degrees / 2] = (pingDistance / 2 >= 85) ? 85.0 : pingDistance / 2;
//            degrees += 2;
//        }
//    }
//    // =======
//
//    // =======
//    // printing array
//    int i;
////    for (i = 0; i < 91; i++) {
////        char msg[75];
////        if (!debugMode) {
////            sprintf(msg, "%-15d %-15.2f %-15.2f", (180-2*i), irValues[i], pingValues[i]);
////        } else {
////            sprintf(msg, "%-15d %-15.2f %-15.2f", (180-2*i), scan_ir[i], scan_ping[i]);
////        }
////        printString(msg);
////    }
//    // =======
//
////    if (!debugMode) {
////        smoothArrayInPlace(irValues, 90, 3);
////        smoothArrayInPlace(pingValues, 90, 3);
////    } else {
////        smoothArrayInPlace(scan_ir, 90, 3);
////        smoothArrayInPlace(scan_ping, 90, 3);
////    }
//
//    // =======
//    // printing array
//    for (i = 0; i < 91; i++) {
//        char msg[75];
//        if (!debugMode) {
//            sprintf(msg, "%-15d %-15.2f %-15.2f", (180-2*i), irValues[i], pingValues[i]);
//        } else {
//            sprintf(msg, "%-15d %-15.2f %-15.2f", (180-2*i), scan_ir[i], scan_ping[i]);
//        }
//        printString(msg);
//    }
//    // =======
//
//    // =======
//    // init and detect array of objects from array of distances
//    int objCount = 0;
//    object* oPtr = (object*) calloc(25, sizeof(object));
//    if (!debugMode) {
//        objCount = count(irValues, pingValues, oPtr);
//    } else {
//        objCount = count(scan_ir, scan_ping, oPtr);
//    }
//    // =======
//
//    // =======
//    // printing objects header
//    int objectIndex;
//    char line[50];
//    sprintf(line, "%-10s %-10s %-10s %-10s", "Object#", "Angle", "Distance", "Width");
//    printString(line);
//    free(line);
//    // =======
//
//    // =======
//    // printing object details
//    // note: 20 is an arbitrary value
//    for (objectIndex = 0; objectIndex < 20; objectIndex++) {
//        if (!oPtr[objectIndex].startAngle) {
//            break;
//        }
//        char line[50];
//        sprintf(line, "%-10d %-4d %-4d %-10.2f %-10.2d",
////        sprintf(line, "%-10d %-10d %-10.2f %-10.2d",
//                objectIndex+1,
////                (oPtr[objectIndex].endAngle + oPtr[objectIndex].startAngle)/2,
//                oPtr[objectIndex].endAngle, oPtr[objectIndex].startAngle,
//                oPtr[objectIndex].distance,
//                oPtr[objectIndex].endAngle - oPtr[objectIndex].startAngle);
//        printString(line);
//    }
//    // =======
//
//    // =======
//    // PRINTING COUNT TO CONSOLE
//    char debug[50];
//    sprintf(debug, "\nCount: %d", objCount);
//    printString(debug);
//    free(line);
//    // =======
//
//    // turn bot towards smallest object
//    int withinSixCM = 0;
//    withinSixCM = face_smallest_object(oPtr, objCount, scanPtr, sensor_data);
//
//    // free memory
//    free(oPtr);
//
//    return withinSixCM;
//}
//
///**
// * Turns to face the smallest object detected.
// * Additionally returns if that object is within 6cm,
// * 6cm instead of 5 to account for error.
// *
// * Returns if the smallest object is within 6cm.
// */
//int face_smallest_object(object objArray[], int size, cyBOT_Scan_t* scanPtr, oi_t* sensor) {
//    int minWidth = 0x7FFFFFFF;
//    int minIndex;
//    int i;
//    for (i = 0; i < size; i++) {
//        int width = objArray[i].endAngle - objArray[i].startAngle;
//        if (width < minWidth) {
//            minWidth = width;
//            minIndex = i;
//        }
//    }
//
//    int angle = 90 - (objArray[minIndex].endAngle + objArray[minIndex].startAngle)/2;
//    turn_clockwise(sensor, angle);
//
//    return objArray[minIndex].distance < 10; // extra few cm to account for bot size
//}
//
//
///**
// *
// *
// */
//int count(float irArray[], float pingArray[], object objArray[]) {
//    int objCount = 0;
//    int i = 1;
//    int inObject = 0;
//    int openingAngle = 0;
//    int closingAngle = 0;
//    while (i < 89) {
//        float prev = irArray[i-1];
//        float next = irArray[i+1];
//        if (fabs(prev - next) > 10) {
//            if (prev - next > 0 && !inObject) {
//                inObject = 1;
//                openingAngle = i * 2;
//            }
//            else if (prev - next < 0 && inObject) {
//                inObject = 0;
//                closingAngle = i * 2;
//
//                if (closingAngle - openingAngle > 4) {
//                    objArray[objCount].startAngle = openingAngle;
//                    objArray[objCount].endAngle = closingAngle;
//                    objArray[objCount].distance = pingArray[i];
//                    objCount++;
//                }
//            }
//        }
//        i++;
//    }
//
//    return objCount;
//
//}
//
//void smoothArrayInPlace(float* array, int size, int windowSize) {
//    int halfWindow = windowSize / 2;
//
//    // Compute the sum of the first window
//    float windowSum = 0;
//    int i;
//    for (i = 0; i < windowSize && i < size; i++) {
//        windowSum += array[i];
//    }
//
//    // Loop through the array and update each element in place
//    for (i = 0; i < size; i++) {
//        // Calculate the average and update the current element
//        array[i] = windowSum / windowSize;
//
//        if (i - halfWindow >= 0) {
//            windowSum -= array[i - halfWindow];
//        }
//
//        if (i + halfWindow + 1 < size) {
//            windowSum += array[i + halfWindow + 1];
//        }
//    }
//}
//
//void printString(char* str) {
//    int x = 0;
//    while (str[x]) {
//        uart_sendChar(str[x++]);
//    }
//    uart_sendChar('\r');
//    uart_sendChar('\n');
//    printf("%s\n", str);
//}
//
//double calculate(int ir) {
//    // x = (y/13250)^-1.538
//    return 2e6 * pow(ir, -1.533);
//}
//
//void calibrate() {
//    timer_init();
//    lcd_init();
//    cyBOT_init_Scan(0b0111);
////    right_calibration_value = 274750; // for bot 0
////    left_calibration_value = 1214500;
////    right_calibration_value = 280000; // for bot 3
////    left_calibration_value = 1314250;
//
//    right_calibration_value = 264250; // for bot 6
//    left_calibration_value = 1230250;
//
////    right_calibration_value = 222250; // for bot 4
////    left_calibration_value = 1172500;
////      right_calibration_value = 348250; // for bot 9
////      left_calibration_value = 1356250;
////      right_calibration_value = 274750; // for bot 1 downstairs
////      left_calibration_value = 1219750;
////    right_calibration_value = 253750; // for bot 11
////    left_calibration_value = 1256500;
//
////    cyBOT_SERVO_cal();
//}
