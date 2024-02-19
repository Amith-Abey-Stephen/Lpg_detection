
// CODE FOR FINDING MAC-ADDRESS OF ESP32

// #include "WiFi.h"
// void setup() {
//   Serial.begin(115200);
//   WiFi.mode(WIFI_MODE_STA);
//   Serial.println(WiFi.macAddress());
// }

// void loop(){
// }

#include <esp_now.h>
#include <WiFi.h>
// int led_1=14;
int buzzer = 14; // Pin for buzzer
typedef struct struct_message {
  char a[32];
  int b;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Sensor Value: ");
  Serial.println(myData.b);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzer,OUTPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-Now");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // Add the MAC address of the sender
  uint8_t senderMacAddress[] = { mac addres of sender  on form 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0};
  memcpy(peerInfo.peer_addr, senderMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
 
int sv1=myData.b;
 if (sv1 > 750) {
    digitalWrite(buzzer, HIGH);
  } else {
    Serial.println("No Message");
    digitalWrite(buzzer, LOW);
  }

  delay(1000);
}  

