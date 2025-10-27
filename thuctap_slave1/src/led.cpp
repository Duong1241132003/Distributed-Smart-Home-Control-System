#include "led.h"
static bool ledState = false;     // Trạng thái LED
static bool lastButtonState = HIGH; // Trạng thái trước đó của nút bấm

// xử lý nút nhấn
void handleButton() 
{
    bool buttonState = digitalRead(buttonPin);
    
    if (buttonState == LOW && lastButtonState == HIGH) // Khi nhấn nút
    { 
        ledState = !ledState; // Đảo trạng thái LED
    }
    lastButtonState = buttonState;
    updateLED();
    
}
// xử lý lệnh điều khiển led
void led_process(int j)
{
    if (cmd[j] == '1') 
    {
      ledState = true;
    } 
    else if (cmd[j] == '0') 
    {
      ledState = false;
    }
}

// Hàm cập nhật trạng thái LED
void updateLED() 
{
    digitalWrite(ledPin, ledState);
}