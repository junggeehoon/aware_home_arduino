#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <esp_now.h>
#include <WiFi.h>
#define BOARD_LED 2
using namespace std;

uint8_t broadcastAddress[] = {0xF4, 0xCF, 0xA2, 0x89, 0x8D, 0x70};

int scanTime = 1; //In seconds
BLEScan* pBLEScan;

typedef struct struct_message {
  int id;
  int rssi;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

bool checkAppleWatch(uint8_t cManufacturerData[]) {
  return cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00 && cManufacturerData[2] == 0x10 && cManufacturerData[3] == 0x05 && cManufacturerData[5] == 0x98;
}

void sendData(int rssi) {
  myData.id = 10;
  myData.rssi = rssi;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    digitalWrite(BOARD_LED, HIGH);
  }
  else {
    Serial.println("Error sending the data");
  }
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {

    std::string strManufacturerData = advertisedDevice.getManufacturerData();
    uint8_t cManufacturerData[100];
    strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

    if (checkAppleWatch(cManufacturerData)) {
      int rssi = advertisedDevice.getRSSI();
      // Serial.println(rssi);
      sendData(rssi);
      return;
    }
  }
};

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    digitalWrite(BOARD_LED, LOW);
  }
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  pinMode(BOARD_LED,OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();
  delay(800);
}