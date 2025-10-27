#ifndef LED_H
#define LED_H

#include <Arduino.h>

#define buttonPin    D3      // Nút bấm trên GPIO 0
#define ledPin       D4      // LED trên GPIO 2
// #define ledStatus    D5      // lấy trạng thái của led

extern char cmd[];
// xử lý nút nhấn
void handleButton();

// xử lý lệnh điều khiển led
void led_process(int j);

// Hàm cập nhật trạng thái LED
void updateLED();

#endif