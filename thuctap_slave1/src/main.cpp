#include<Arduino.h>
#include <SoftwareSerial.h>
#include "common.h"
#include "led.h"
#include "ledRGB.h"
#include "curtain.h"

SoftwareSerial mySerial(RX_PIN, TX_PIN);

bool flag = false;           // Cờ xác nhận việc hoàn tất nhận dữ liệu từ esp32      
//receive
uint8_t received_data[BUFFER_SIZE];
uint8_t received_data_web[UART_BUFFER_SIZE];
uint8_t received_data_uart1[UART_BUFFER_SIZE];
uint8_t received_data_uart2[UART_BUFFER_SIZE];
uint8_t received_data_uart3[UART_BUFFER_SIZE];
uint8_t received_data_uart4[UART_BUFFER_SIZE];
char cmd[UART_BUFFER_SIZE];
uint8_t* received_uart[TOTAL_UARTS] = {
    received_data_uart1, 
    received_data_uart2, 
    received_data_uart3, 
    received_data_uart4
};
uint8_t indexArrCpt=0;
uint8_t indexArr=0;                             //vị trí lưu vào mảng chung
uint8_t indexReadPos=0;                         //vị trí đọc tại mảng chung
int8_t indexUartArr=-1;                         //số thứ tự mảng con để ghi
uint8_t indexUartWritePos[TOTAL_UARTS] = {0};   //Vị trí ghi của từng mảng con
uint8_t indexUartReadPos=0;                     //vị trí đọc tại mảng con
const int time_send = 1000;
void implement(); 
void handleWebData();
void handleSerial();
void InitConfig();
void send_status();
void receive_cmd();

// Slave 1:led,0# Slave 1:led,1#
// Slave 1:rgb,100,100,100# Slave 1:rgb,0,0,0#
void setup()
{
    InitConfig();
}

void loop()
{
    implement(); 
    // send_status();
}

void InitConfig()
{
    pinMode(buttonPin, INPUT_PULLUP); // Kéo lên nội bộ
    pinMode(ledPin, OUTPUT);
    // pinMode(ledStatus, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(TTL_RS_enable,OUTPUT);
    mySerial.begin(9600);
    Serial.begin(9600);
}
// Hàm xử lý lệnh từ Serial Monitor
void handleSerial() 
{
  while(Serial.available()) 
  {
    uint8_t c = Serial.read();   // Đọc từng byte
    
    if(c=='!')
    {
      send_status();
      // Serial.read();
      break;
    }
    // Serial.println(c);
    if (c == '\n'|| c == '\r' || c == '\0' || c=='#') 
    { // Nếu nhận đủ hoặc gặp '\n'
        received_data_web[indexArrCpt] = '\0';  // Kết thúc chuỗi
        indexArrCpt = 0;  // Reset để nhận chuỗi mới
        flag = true;
        while(Serial.available());

    } 
    else 
    {
        received_data_web[indexArrCpt++] = c; // Lưu vào mảng
    }
  }
  if(flag==true) handleWebData();
}

void handleWebData()
{ 
  flag = false; 
  strcpy(cmd, (char *)received_data_web);
  if (strstr(cmd, id1) != NULL) 
  {
      char s[UART_BUFFER_SIZE];
      int idx = 0;
      unsigned int j = 0;

      while (cmd[j] != ':'&& j < sizeof(cmd)-1) 
      {
          j++;
      }
      j ++;

      while (cmd[j] != ','&& j < sizeof(cmd)-1) 
      {
          s[idx++] = cmd[j++];
      }
      s[idx] = '\0'; // Thêm ký tự kết thúc chuỗi
      j ++;  
      if (strstr(s, "led")) 
      {
        //   Serial.println("led processing...");
          led_process(j);
      } 
      else if (strstr(s, "rgb")) 
      {
        //   Serial.println("ledRGB processing...");
          ledRGB_process(j);
      }
      else if (strstr(s,"motor"))
      {
        //   Serial.println("motor processing...");
          motor_process(j);
      }     
  }
}

void implement()
{
  // unsigned long ms = 0;
  
  handleButton();
  get_response();
  // if(millis()-ms>=time_send) 
  // {
  //   send_status();
  //   ms = millis();
  // }
  receive_cmd();
}

void send_status()
{   
    digitalWrite(TTL_RS_enable,TRANSMIT_ENABLE);
    if(get_percent())
    {
        Serial.print("Slave 1,"+String(digitalRead(ledPin) ? "1," : "0," +String(percent) + "%#"));
    }
    else
    {
        Serial.print("Slave 1,"+String(digitalRead(ledPin) ? "1," : "0,") + "-1#");
    }
    Serial.flush();
    // digitalWrite(TTL_RS_enable,RECEIVE_ENABLE);   
}
void receive_cmd()
{
    digitalWrite(TTL_RS_enable,RECEIVE_ENABLE);
    handleSerial();
}