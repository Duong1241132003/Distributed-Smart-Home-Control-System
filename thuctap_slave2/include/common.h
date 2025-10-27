#ifndef COMMON_H
#define COMMON_H
// Khai báo các chân RX và TX cho espsoftwareserial
#define RX_PIN D2 // D2
#define TX_PIN D1 // D1
#define TTL_RS_enable       D5   
#define RECEIVE_ENABLE      0
#define TRANSMIT_ENABLE     1
#define BUFFER_SIZE 200
#define UART_BUFFER_SIZE 100  // Kích thước mỗi mảng con
#define TOTAL_UARTS 4  // Số lượng mảng con
#define id1 "Slave 1"  // esp8266 rem
#define id2 "Slave 2"  // esp8266 be ca

#endif