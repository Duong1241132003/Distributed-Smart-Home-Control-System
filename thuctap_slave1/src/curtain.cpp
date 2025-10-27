#include "curtain.h"

//transmit
static uint8_t stopMotor[]     = {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x04, 0x00, 0x01, 0x01, 0x11}; // Dừng động cơ
static uint8_t openCurtain[]   = {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x04, 0x00, 0x01, 0x00, 0x10}; // Mở rèm
static uint8_t closeCurtain[]  = {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x04, 0x00, 0x01, 0x02, 0x12}; // Đóng rèm
static uint8_t reverseMotor[]  = {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x05, 0x01, 0x00, 0x01, 0x01, 0x12}; // Đảo chiều động cơ
static uint8_t disableReverse[]= {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x05, 0x01, 0x00, 0x01, 0x00, 0x11}; // Tắt đảo chiều
static uint8_t disableAutoPwr[]= {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x06, 0x01, 0x00, 0x01, 0x00, 0x12}; // Tắt Auto power
static uint8_t enableAutoPwr[] = {0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x06, 0x01, 0x00, 0x01, 0x01, 0x13}; // Bật Auto power
static uint8_t unconnect[]     = {0x55, 0xAA, 0x00, 0x01, 0x00, 0x00, 0x00, 0x55, 0xAA, 0x00, 0x02, 0x00, 0x00, 0x01}; //unconnect and delete device (only for app)
uint8_t percent; //phan tram dong rem

void motor_process(int j)
{
  char str[15];
  uint8_t i=0;
  while(cmd[j]!='\0')
  {
    str[i++]=cmd[j++];
  }
  str[i] = '\0';
  reset();
  sendCommand(str);
 
  // get_response();       //nhận phản hồi từ động cơ
  // write_SubArr();       //ghi phản hồi vào bộ nhớ
  // printData();          //in các phản hồi
  // Serial.println();
}

void sendCommand(String receivedStr) 
{
    if (receivedStr == "stopMotor") 
    {
        mySerial.write(stopMotor, sizeof(stopMotor));
    } 
    else if (receivedStr == "openCurtain") 
    {
        mySerial.write(openCurtain, sizeof(openCurtain));
    } 
    else if (receivedStr == "closeCurtain") 
    {
        mySerial.write(closeCurtain, sizeof(closeCurtain));
    } 
    else if (receivedStr == "reverseMotor") 
    {
        mySerial.write(reverseMotor, sizeof(reverseMotor));
    } 
    else if (receivedStr == "disableReverse") 
    {
        mySerial.write(disableReverse, sizeof(disableReverse));
    } 
    else if (receivedStr == "disableAutoPwr") 
    {
        mySerial.write(disableAutoPwr, sizeof(disableAutoPwr));
    } 
    else if (receivedStr == "enableAutoPwr") 
    {
        mySerial.write(enableAutoPwr, sizeof(enableAutoPwr));
    } 
    else if (receivedStr == "unconnect")
    {
        mySerial.write(unconnect, sizeof(unconnect));
    }
    else 
    {
        percent = receivedStr.toInt(); // Chuyển sang số nguyên
        cmd_percent(percent);
    }
}

void cmd_percent(int percent)
{
  uint8_t XX,YY;
  XX = percent;
  YY = XX + 0x15;
  uint8_t command[] = 
  { 
     0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, 0x02, 0x02, 
     0x00, 0x04, 0x00, 0x00, 0x00, XX, YY 
  };
  mySerial.write(command, sizeof(command));
}

void get_response()
{
  while (mySerial.available()) 
  {
    uint8_t c = mySerial.read();   // Đọc từng byte
    received_data[indexArr++] = c; // ghi từng byte vào mảng
    if(indexArr>=BUFFER_SIZE) //ghi lại từ vị trí 0 khi bộ nhớ đầy
    {
      indexArr = 0;
    }
  } 
  write_SubArr();
}

void write_SubArr()
{
  while(indexReadPos != indexArr)
  {
    if (indexReadPos >= BUFFER_SIZE) 
    {
      indexReadPos = 0; // Khi đọc hết mảng, reset về đầu
    }

    uint8_t data = received_data[indexReadPos]; // Đọc dữ liệu từ mảng chung
    received_data[indexReadPos++]=0;            // Xóa dữ liệu

    if (data == 0x55) 
    { 
        // Nếu gặp 0x55, chuyển sang mảng con tiếp theo và reset vị trí ghi
        indexUartArr++;
        if (indexUartArr == TOTAL_UARTS) 
        {
            indexUartArr = 0; // Nếu vượt quá số mảng con, quay lại mảng đầu tiên
        }
        indexUartWritePos[indexUartArr] = 0; // Reset vị trí ghi mảng con mới
    }

    // Ghi dữ liệu vào mảng con hiện tại nếu còn chỗ
    if (indexUartWritePos[indexUartArr] < UART_BUFFER_SIZE) 
    {
        received_uart[indexUartArr][indexUartWritePos[indexUartArr]++] = data;
    }
  }
}
void reset()
{
    indexArrCpt=0;
    indexArr=0;                             //vị trí lưu vào mảng chung
    indexReadPos=0;                         //vị trí đọc tại mảng chung
    indexUartArr=-1;                         //số thứ tự mảng con để ghi
    for(int i=0;i<TOTAL_UARTS;i++)
    {
      memset(received_uart[i], 0, UART_BUFFER_SIZE); // Xóa toàn bộ mảng con
      indexUartWritePos[TOTAL_UARTS] = {0};   // Vị trí ghi của từng mảng con
    }
    indexUartReadPos=0; 
}
// Hàm in dữ liệu từ các mảng con
void printData() {
    for (int i = 0; i < TOTAL_UARTS; i++) {
        Serial.print("UART");
        Serial.print(i + 1);
        Serial.print(": ");
        printArray(received_uart[i], indexUartWritePos[i]);
    }
}

// Hàm in mảng con ra Serial Monitor
void printArray(uint8_t *arr, uint8_t length) {
    for (int i = 0; i < length; i++) 
    {
        Serial.print(arr[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
bool get_percent()
{
  for (int i = 0; i < TOTAL_UARTS; i++) 
  {
      uint8_t perc      = received_uart[i][indexUartWritePos[i]-2];  //phần trăm
      uint8_t checksum  = received_uart[i][indexUartWritePos[i]-1];  //checksum
      if(checksum - perc == 0x1A)
      {
        percent = perc;
        Serial.println(String(perc)+"%");   //in phần trăm và kết thúc
        return true;
      }
  }
  // Serial.println("running...");
  return false;
}