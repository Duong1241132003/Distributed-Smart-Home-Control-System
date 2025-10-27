#include<Arduino.h>

#include <SoftwareSerial.h>
#include "common.h"
#include <cambien.h>
// SoftwareSerial mySerial(RX_PIN, TX_PIN);
// Slave 2:water_pump,0#
// Slave 2:water_pump,1#
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
int8_t indexUartArr=0;                         //số thứ tự mảng con để ghi
uint8_t indexUartWritePos[TOTAL_UARTS] = {0};   //Vị trí ghi của từng mảng con
uint8_t indexUartReadPos=0;                     //vị trí đọc tại mảng con
const int time_send = 1000;
void implement(); 
void handleWebData();
void handleSerial();
void water_pump(int j);
void receive_cmd();
void send_status();
void setup()
{
    Init();
    // mySerial.begin(9600);
    Serial.begin(9600);
    pinMode(TTL_RS_enable, OUTPUT);
}

void loop()
{
    implement(); 
}

// Hàm xử lý lệnh từ Serial
void handleSerial() 
{   
  while(Serial.available()) 
  {
    uint8_t c = Serial.read();   // Đọc từng byte
    if(c=='@')
    {
      send_status();
      indexArrCpt = 0;  // Reset để nhận chuỗi mới
      // Serial.read();
      break;
    }
    if (c == '\n'|| c == '\r' || c == '\0' || c=='#') 
    { // Nếu nhận đủ hoặc gặp '\n'
        received_data_web[indexArrCpt] = '\0';  // Kết thúc chuỗi
        indexArrCpt = 0;  // Reset để nhận chuỗi mới
        flag = true;
        Serial.read();
        // while(Serial.available());
        // Serial.println();
    } 
    else 
    {
        received_data_web[indexArrCpt++] = c; // Lưu vào mảng
        // Serial.print((char)c);
    }
  }
  if(flag==true) handleWebData();
}

void handleWebData()
{ 
  flag = false; 
  strcpy(cmd, (char *)received_data_web);
  if (strstr(cmd, id2) != NULL) 
  {
      char s[UART_BUFFER_SIZE];
      int idx = 0;
      int j = 0;

      while (cmd[j] != ':' && j < sizeof(cmd)-1) 
      {
          j++;
      }
      j ++;

      while (cmd[j] != ',' && j < sizeof(cmd)-1) 
      {
          s[idx++] = cmd[j++];
      }
      s[idx] = '\0'; // Thêm ký tự kết thúc chuỗi
      j ++;  
      if (strstr(s, "water_pump")) 
      {
          Serial.println("water_pump processing...");
          water_pump(j);
      }   
  }
}

void water_pump(int j)
{
  if (cmd[j] == '1') 
  {
    digitalWrite(water_pumper,HIGH);
    // Serial.println(1);
  } 
  else if (cmd[j] == '0') 
  {
    digitalWrite(water_pumper,LOW);
    // Serial.println(0);
  }
}
void implement()
{
  // static unsigned long ms=0;
  receive_cmd();
  // if(millis()-ms>=time_send) 
  // {
    get_Val();
    // send_status();
  //   ms = millis();
  // }
}

void send_status()
{   
    digitalWrite(TTL_RS_enable,TRANSMIT_ENABLE);
    Serial.print("Slave 2," + String(temperature) +","+ String(turbidity_val)+"#");
    // mySerial.println("Slave 2," + String(temperature) +","+ String(turbidity_val)+"#");
    Serial.flush();
    digitalWrite(TTL_RS_enable,RECEIVE_ENABLE);
}

void receive_cmd()
{
    digitalWrite(TTL_RS_enable,RECEIVE_ENABLE);
    handleSerial();
}