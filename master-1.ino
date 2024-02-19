#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xE0, 0x13, 0x5C};
int button = 14;
#define MQ2pin 34 // Pin on the ESP32 connected to the MQ2 sensor


typedef struct struct_message {
  char a[32];
  int b;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  Serial.print("Warming up");
  delay(3000);

  Serial.println();
  
  // pinMode(MQ2pin,INPUT);


  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-Now");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

  int sensorValue = analogRead(MQ2pin);
  Serial.print("Analog Value: ");
  Serial.println(sensorValue);
  int sv = sensorValue;
  Serial.println();
  if (sv > 750) {
    strcpy(myData.a, "Gas Detected!");
    myData.b = sv; // Store integer value directly
    }
  else {
    strcpy(myData.a, "No Messagee!");
    myData.b = sv;
  }

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending Confirmed");
  } else {
    Serial.println("Sending Error");
  }
  delay(1000);
}
