#ifndef LEDRGB_H
#define LEDRGB_H

#include <Arduino.h>

#define RED_PIN    D8		// led đỏ
#define GREEN_PIN  D7	    // led xanh lá
#define BLUE_PIN   D6	    // led xanh dương
extern char cmd[];

//hàm xử lý lệnh điều khiển led RGB
void ledRGB_process(int j);

//điều chỉnh độ sáng 3 led trên led RGB
void RGB(uint8_t r, uint8_t g, uint8_t b) ;

#endif