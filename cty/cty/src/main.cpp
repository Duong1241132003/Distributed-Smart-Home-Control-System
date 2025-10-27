#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include "common.h"


// Thông tin WiFi
const char* ssid = "LHD";
const char* password = "18121994";

WebServer server(80);  // Tạo webserver trên port 80

bool curtainState = false;

String rgbColor = "#FFFFFF";
String s = "";


// Prototype các hàm
String handleFile(const String& path);
void handleRoot();
void handleData();
void handleToggle();
void handleSwitch();
void handleColor();
String hexToDecString(String hex);
void handleCurtainStatus();
void handleCurtainPercentage();


void setup() {
  init();
  Serial.begin(9600);
 
  // Khởi tạo SPIFFS đầu tiên
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
    while(1);
  }


  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("\n Kết nối WiFi thành công!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Các route (đường dẫn) của webserver
  server.on("/", []() {
    server.send(200, "text/html", handleFile("/cty.html"));
  });
 
  server.on("/cty.css", []() {
    server.send(200, "text/css", handleFile("/cty.css"));
  });
 
  server.on("/data", handleData);
  server.on("/toggle", handleToggle);
  server.on("/curtain/status", handleCurtainStatus);
  server.on("/curtain/percentage", handleCurtainPercentage);
  server.on("/switch", handleSwitch);
  server.on("/color", handleColor);


  server.begin();
  Serial.println("Webserver đã khởi động!");
}


void loop() {
  server.handleClient();
  running();
  // reset();
}


String handleFile(const String& path) {
  File file = SPIFFS.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("Failed to open file: " + path);
    return "File not found";
  }
 
  String content = file.readString();
  file.close();
  return content;
}


void handleData() {
  String json = "{";
  json += "\"temp\":" + String(temperature) + ",";
  json += "\"turbidity\":" + String(turbidity) + ",";
  json += "\"curtain\":" + String(curtainPosition);
  // json += "\"statusSL1\":" + bool(statusSL1);
  json += "}";


  server.send(200, "application/json", json);
}


void handleToggle() {
  if (server.hasArg("state")) {
    waterPump = server.arg("state") == "1";
    s = "Slave 2:water_pump," + String(waterPump ? 1 : 0)+"#";
    Serial.println(s);
  }
  server.send(200, "text/plain", "OK");
}


void handleCurtainStatus() {
  if (server.hasArg("state")) {
    curtainState = server.arg("state") == "1";
    s = "Slave 1:motor,stopMotor#";
    Serial.println(s);
  }
  server.send(200, "text/plain", "OK");
}


void handleCurtainPercentage() {
  if (server.hasArg("value")) {
    curtainPosition = server.arg("value").toInt();
    s = "Slave 1:motor," + String(curtainPosition) + "#";
    Serial.println(s);
  }
  server.send(200, "text/plain", "OK");
}


void handleSwitch() {
  if (server.hasArg("state")) {
    switchState = server.arg("state") == "1";
    s = "Slave 1:led," + String(switchState ? 1 : 0) + "#";
    Serial.println(s);
  }
  server.send(200, "text/plain", "OK");
}


void handleColor() {
  if (server.hasArg("hex")) {
    rgbColor = server.arg("hex");
    s = hexToDecString(rgbColor);
    // Serial.println(rgbColor);
  }
  server.send(200, "text/plain", "OK");
}


String hexToDecString(String hex) {
  // Tách thành 3 phần và chuyển từ hex sang decimal
  int r = strtol(hex.substring(0, 2).c_str(), NULL, 16);
  int g = strtol(hex.substring(2, 4).c_str(), NULL, 16);
  int b = strtol(hex.substring(4, 6).c_str(), NULL, 16);


  // Trả về chuỗi "R,G,B"
  return "Slave 1:rgb," + String(r) + "," + String(g) + "," + String(b) + "#";
}
