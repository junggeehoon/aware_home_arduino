#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int id;
  int rssi;
} struct_message;

const int NUMBER_OF_BOARD = 12;
struct_message myData;
struct_message boardsStruct[NUMBER_OF_BOARD];

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&myData, incomingData, sizeof(myData));
  boardsStruct[myData.id].rssi = myData.rssi;
  for (int i = 0; i < NUMBER_OF_BOARD; i++) {
    Serial.print(boardsStruct[i].rssi);
    Serial.print(" ");
  }

  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  delay(1000);
}