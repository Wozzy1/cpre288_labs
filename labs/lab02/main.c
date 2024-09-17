#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include <string.h>


void checkpoint_1(oi_t *sensor_data);
void checkpoint_1_1(oi_t *sensor_data);
void checkpoint_2(oi_t *sensor_data);
void checkpoint_3(oi_t *sensor_data);


void main() {
  oi_t *sensor_data = oi_alloc();
  oi_init(sensor_data);

//  checkpoint_1(sensor_data);
//  checkpoint_2(sensor_data);
//  checkpoint_3(sensor_data);
  cyBot_uart_init();
  int value = 0;
wsww
  lcd_init();


  while (value != 'q') {
//    scanf(" %c", &input);
    value = cyBot_getByte();
//    printf("%d | %c", value, value);

    char string[20];
    sprintf(string, "got an %c", value);
    int j = 0;
    lcd_clear();
    for (j = 0; j < strlen(string); j++) {
        lcd_putc(string[j]);
    }

//    lcd_putc(value);

    if (value == 119) {
        checkpoint_1(sensor_data);
    }
    else if (value == 115) {
        checkpoint_1_1(sensor_data);
    }
    else {
        oi_setWheels(0, 0);  // stop
    }
  }







  oi_setWheels(0, 0);  // stop
  oi_free(sensor_data);

}


void checkpoint_1(oi_t *sensor_data) {
    double sum = 0;
    oi_setWheels(500, 500);  // move forward; full speed
    while (sum < 1000) {
      oi_update(sensor_data);
      sum += abs(sensor_data->distance);
    }
}

void checkpoint_1_1(oi_t *sensor_data) {
    double sum = 0;
    oi_setWheels(-500, -500);  // move forward; full speed
    while (sum < 1000) {
      oi_update(sensor_data);
      sum += abs(sensor_data->distance);
    }
}

void checkpoint_2(oi_t *sensor_data) {
    int i;
    int *bumpStatus;
    for (i = 0; i < 4; i++)  {
        turn_clockwise(sensor_data, 90);
        move_forward_with_bump(sensor_data, 50, bumpStatus);
    }
}

void checkpoint_3(oi_t *sensor_data) {
    traverse(sensor_data, 200);
}

