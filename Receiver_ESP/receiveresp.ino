//Receiver esp32 code
#include <esp_now.h>
#include <WiFi.h>
#include <String.h>
#include <Arduino.h>
int lightpin = 22;//light pin
int fanpin = 23;//fan pin
String x;
String y;
typedef struct test_struct {
  String x;
  String y;
} test_struct;
test_struct myData;
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
memcpy(&myData, incomingData, sizeof(myData));
Serial.print("Bytes received: ");
Serial.println(len);
Serial.print("x: ");
Serial.println(myData.x);
Serial.print("y: ");
Serial.println(myData.y);
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(lightpin, OUTPUT);
  pinMode(fanpin, OUTPUT);
}
void loop() 
{
  if(myData.x == "LON") {
    digitalWrite(lightpin, HIGH);
    Serial.println("lighton");
  }
  if(myData.x == "LOFF") {
    digitalWrite(lightpin, LOW);
    Serial.println("lightoff");
  }
  if(myData.y == "Fon") {
    digitalWrite(fanpin, HIGH);
    Serial.println("fanton");
  }
  if(myData.y == "Foff") {
    digitalWrite(fanpin, LOW);
    Serial.println("fanoff");
  }
delay(2000);
}
