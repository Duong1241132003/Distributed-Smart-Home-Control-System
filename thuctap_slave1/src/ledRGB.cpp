#include "ledRGB.h"

void ledRGB_process(int j) 
{
    uint8_t r = 0, g = 0, b = 0; //các biến điều chỉnh màu sắc red, green, blue led
    // lấy giá trị red
    while (cmd[j] != ',') 
    {
        r = r * 10 + (cmd[j] - '0');
        j++;
    }
    j++;
    
    // lấy giá trị green
    while (cmd[j] != ',') 
    {
        g = g * 10 + (cmd[j] - '0');
        j++;
    }
    j++;

    // lấy giá trị blue
    while (cmd[j] != '\0') { // Duyệt đến hết chuỗi 
        b = b * 10 + (cmd[j] - '0');
        j++;
    }
    RGB(r, g, b);
}

// điều chỉnh màu sắc
void RGB(uint8_t r, uint8_t g, uint8_t b) 
{
    analogWrite(RED_PIN,r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
}