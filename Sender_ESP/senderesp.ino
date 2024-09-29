//sendercodeesp32
#include <esp_now.h>
#include <WiFi.h>
#include <String.h>
String receivedData;
String dataF;
String dataL;
uint8_t broadcastAddress1[] = {0xB8, 0xD6, 0x1A, 0x6A, 0x65, 0x1C}; //receiver mac address to be replaced
typedef struct test_struct {
  String x;
  String y;
} test_struct;
test_struct test; //to store variable values
esp_now_peer_info_t peerInfo; //to store information about peer
//to check data is sent or not(acknowledgement)
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent); //call back function executed when data is sent
  //now adding info about receiver
  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}
void loop() 
{
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    int commaIndex = receivedData.indexOf(',');
    if (commaIndex != -1) {
      dataL = receivedData.substring(0, commaIndex);
      dataF = receivedData.substring(commaIndex + 1);
      Serial.println(dataL);
      Serial.println(dataF);
      test.x = dataL;
      test.y = dataF;
      esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      } else {
        Serial.println("Error sending the data");
      }
    }
  }
  delay(2000);
}
