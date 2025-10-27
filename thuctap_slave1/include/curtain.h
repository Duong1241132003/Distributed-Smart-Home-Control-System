#ifndef CURTAIN_H
#define CURTAIN_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "common.h"
// Extern các biến toàn cục từ main.cpp
extern SoftwareSerial mySerial;

extern uint8_t received_data[];
extern uint8_t* received_uart[];
extern uint8_t indexArrCpt;
extern uint8_t indexArr;
extern uint8_t indexReadPos;
extern int8_t indexUartArr;
extern uint8_t indexUartWritePos[];
extern uint8_t indexUartReadPos;
extern char cmd[];
extern uint8_t percent;
void motor_process(int j);
void sendCommand(String receivedStr);
void cmd_percent(int percent);
void get_response();
void write_SubArr();
void reset();
void printData();
void printArray(uint8_t *arr, uint8_t length);
bool get_percent();

#endif
