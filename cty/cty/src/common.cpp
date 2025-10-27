#include "common.h"

char cmd[BUFFER_SIZE];
char data_main[BUFFER_SIZE];
char data1[UART_BUFFER_SIZE];
char data2[UART_BUFFER_SIZE];
char data3[UART_BUFFER_SIZE];
char data4[UART_BUFFER_SIZE];
char *data[TOTAL_UARTS] = {
  data1,
  data2,
  data3,
  data4
};

// cac bien de xu ly truyen nhan
uint8_t indexArrCpt = 0;
uint8_t indexArr = 0;                             //vi tri luu tai mang chung
uint8_t indexReadPos = 0;                         //vi tri doc tai mang chung
uint8_t indexDataWritePos[TOTAL_UARTS] = { 0 };   //vi tri ghi cua tung mang con
int8_t  indexDataSubArr = 0;                       //so thu tu mang con de ghi
uint8_t indexDataReadPos = 0;                     //vi tri doc tai mang con
bool flag = false;                                //xac nhan truyen

//cac bien du lieu

//thông số ở slave 1
int8_t curtainPosition;
uint8_t switchState;

//thông số ở slave 2
int8_t temperature;
int turbidity;
uint8_t waterPump;

//cờ xác nhận trạng thái kết nối
bool flagSL1 = false;
bool flagSL2 = false;
unsigned long ms1 = 0;
unsigned long ms2 = 0;
const unsigned long intervalTime = 30000;
bool statusSL1 = false;
bool statusSL2 = false;

void init() {
  pinMode(TTL_RS_enable, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
}

void running() {
  receive_cmd();
  send_cmd();
  receive_status();
  write_SubArr();
  update();
}

//Hàm nhận lệnh từ Serial
void receive_cmd() {
  if (!s.isEmpty())
  {
    s.toCharArray(cmd, sizeof(cmd));
    flag = true;
    indexArrCpt = 0;
    s = "";
  }
}

//Hàm gửi lệnh tới các slave
void send_cmd() 
{
  static unsigned long ms_selectSL = 0;
  static bool selectSL = false;
  if (flag) 
  {
    if((selectSL == true && flagSL1 == true) || (selectSL == false && flagSL2 == true) || millis() - ms_selectSL >= 2000)
    // while(flagSL1==false && flagSL2==false);
    {
      digitalWrite(TTL_RS_enable, TRANSMIT_ENABLE);
      //kích hoạt chế độ truyền
      //gửi lệnh
      Serial2.print(cmd);
      flag = false;
      Serial2.flush();
      digitalWrite(TTL_RS_enable, RECEIVE_ENABLE);
      ms_selectSL = millis();
    }
  }
  else
  {
    if(millis() - ms_selectSL >= 500)
    {
      if (selectSL == true && (flagSL2 == true || millis()-ms_selectSL >= 1000))
      {
        digitalWrite(TTL_RS_enable, TRANSMIT_ENABLE);

        Serial2.print("!");
        selectSL = false;
        ms_selectSL = millis();
        Serial2.flush();
        digitalWrite(TTL_RS_enable, RECEIVE_ENABLE);
      }
      else if (selectSL == false && (flagSL1 == true || millis()-ms_selectSL >= 1000))
      {
        digitalWrite(TTL_RS_enable, TRANSMIT_ENABLE);

        Serial2.print("@");
        selectSL = true;
        ms_selectSL = millis();
        Serial2.flush();
        digitalWrite(TTL_RS_enable, RECEIVE_ENABLE);
      }  
    }
  }
}

void receive_status() 
{
  // reset();
  digitalWrite(TTL_RS_enable, RECEIVE_ENABLE);
  while (Serial2.available()) 
  {
    char c = Serial2.read();
    //loại bỏ nhiễu
    if(c=='\0')
    {
      continue;
    }
    data_main[indexArr++] = c;
    if (indexArr >= BUFFER_SIZE) 
    {
      indexArr = 0;
    }                                      
  }
}

void write_SubArr() {
  if(indexReadPos != indexArr) {
    // Đọc dữ liệu từ mảng chung
    char databyte = data_main[indexReadPos++];
    if (indexReadPos >= BUFFER_SIZE) {
      indexReadPos = 0;
    }

    // Đảm bảo indexDataSubArr hợp lệ trước khi ghi dữ liệu
    if (indexDataSubArr < TOTAL_UARTS && indexDataWritePos[indexDataSubArr] < UART_BUFFER_SIZE ) {
      data[indexDataSubArr][indexDataWritePos[indexDataSubArr]++] = databyte;
      // Serial.print(databyte);
    }

    // Khi gặp ký tự kết thúc, chuyển sang mảng con tiếp theo
    if (databyte == '#') 
    {
      data[indexDataSubArr][indexDataWritePos[indexDataSubArr]] = '\0';
      indexDataSubArr++;
      if (indexDataSubArr >= TOTAL_UARTS) 
      {
        indexDataSubArr = 0;  // Quay về mảng con đầu
      }
      indexDataWritePos[indexDataSubArr] = 0;  // Reset vị trí ghi
    }
  }
}
//cập nhật dữ liệu
void update() {
  for (int i = 0; i < TOTAL_UARTS; i++) 
  {
    //xử lý khi cuối mảng là ký tự kết thúc xâu được quy ước là #
    if (indexDataWritePos[i] > 0 && data[i][indexDataWritePos[i]-1] == '#' && data[i][0] != '\0')
    {
      int temp = 0;
      if (strstr(data[i], id1)) 
      {
        int j = 0;
        while(data[i][j] !=',')
        {
          j++;
        }
        j++;
        flagSL2 = false;
        flagSL1 = true;
        ms1 = millis();
        while(data[i][j]<='9' && data[i][j] >= '0')
        {
          temp = temp*10 + (data[i][j++] - '0');
        }
        j++;
        switchState = temp;
        temp = 0;
        while(data[i][j]<='9' && data[i][j] >= '0')
        {
          temp = temp*10 + (data[i][j++] - '0');
        }
        curtainPosition = temp;
        temp = 0;
        Serial.print("led: " + String(switchState) + " - ");
        Serial.println("curtain: " + String(curtainPosition));
      } 
      else if (strstr(data[i], id2)) 
      {
        int j = 0;
        while(data[i][j] !=',')
        {
          j++;
        }
        j++;
        flagSL1 = false;
        flagSL2 = true;
        ms2 = millis();
        while(data[i][j]<='9' && data[i][j] >= '0')
        {
          temp = temp*10 + (data[i][j++] - '0');
        }
        j++;
        temperature = temp;
        temp = 0;
        while(data[i][j]<='9' && data[i][j] >= '0')
        {
          temp = temp*10 + (data[i][j++] - '0');
        }
        turbidity = temp;
        temp = 0;
        Serial.print("tem: " + String(temperature) +" - ");
        Serial.println("turnidity: " + String(turbidity));
      }
      else 
      {
        flagSL1 = false;
        flagSL2 = false;
      }
      //khi mảng được xử lý xong, reset nó
      for (int j = 0; j < UART_BUFFER_SIZE; j++) 
      {
        data[i][j] = '\0';
      }
    }
  }
}


void check_status()
{
  if(flagSL1 == true)
  {
    statusSL1 = true;
  }
  else
  {
    if(millis() - ms1 >= intervalTime)
    {
      statusSL1 = false;
      Serial.println("Slave 1 disconnected");
    }
  }

  if(flagSL2 == true)
  {
    statusSL2 = true;
  }
  else
  {
    if(millis() - ms2 >= intervalTime)
    {
      statusSL2 = false;
      Serial.println("Slave 2 disconnected");
    }
  }
}

void reset() {
  indexArrCpt = 0;
  indexArr = 0;         //vá»‹ trÃ­ lÆ°u vÃ o máº£ng chung
  indexReadPos = 0;     //vá»‹ trÃ­ Ä‘á»c táº¡i máº£ng chung
  indexDataSubArr = 0;  //sá»‘ thá»© tá»± máº£ng con Ä‘á»ƒ ghi
  for (int i = 0; i < TOTAL_UARTS; i++) 
  {
    memset(data[i], 0, UART_BUFFER_SIZE);  // XÃ³a toÃ n bá»™ máº£ng con
    indexDataWritePos[i] = { 0 };  // Vá»‹ trÃ­ ghi cá»§a tá»«ng máº£ng con
  }
  indexDataReadPos = 0;
}