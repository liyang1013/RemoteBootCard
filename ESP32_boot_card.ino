
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Tenda";
const char* password = "961013ly";

ESP8266WebServer server(80);
const int SwitchKey = 4;     //开关控制引脚
const int RestartKey = 5;    //重启控制引脚
const int StateDetect = 13;  //开机状态检测

void handleRoot() {
  server.send(200, "text/plain; charset=utf-8", "ESP32-12F开机卡, /state获取电脑开关机状态, /power电脑执行开机, /reboot电脑重启");
}

void getStateDetect() {
  if (digitalRead(StateDetect) == LOW) {
    server.send(200, "text/plain; charset=utf-8", "关机中");
  } else {
    server.send(200, "text/plain; charset=utf-8", "开机中");
  }
}

void handleReboot() {
  if (digitalRead(StateDetect) == HIGH) {
    digitalWrite(RestartKey, HIGH);
    delay(500);
    digitalWrite(RestartKey, LOW);
    server.send(200, "text/plain; charset=utf-8", "电脑重启中");
  } else {
    server.send(200, "text/plain; charset=utf-8", "电脑未开机，请先开机");
  }
}

void handlePower() {
  if (digitalRead(StateDetect) == LOW) {
    digitalWrite(SwitchKey, HIGH);
    delay(500);
    digitalWrite(SwitchKey, LOW);
    server.send(200, "text/plain; charset=utf-8", "电脑开机中");
  } else {
    server.send(200, "text/plain; charset=utf-8", "电脑已开机，请勿重复开机");
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(SwitchKey, OUTPUT);
  pinMode(RestartKey, OUTPUT);
  pinMode(StateDetect, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("连接Wi-Fi中");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功！IP地址: " + WiFi.localIP().toString());

  server.on("/", handleRoot);  // 根路径
  server.on("/state", getStateDetect);
  server.on("/power", handlePower);
  server.on("/reboot", handleReboot);
  server.begin();
  Serial.println("HTTP服务器已启动");
}

void loop() {
  server.handleClient();
}