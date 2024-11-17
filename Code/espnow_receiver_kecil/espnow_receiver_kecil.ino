#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char name[32];
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
  float powerFactor;
} struct_message;

// Create a struct_message called dataSensor
struct_message dataSensor;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&dataSensor, incomingData, sizeof(dataSensor));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Alat: ");         Serial.println(dataSensor.name);
  Serial.print("Voltage: ");      Serial.print(dataSensor.voltage);      Serial.println("V");
  Serial.print("Current: ");      Serial.print(dataSensor.current);      Serial.println("A");
  Serial.print("Power: ");        Serial.print(dataSensor.power);        Serial.println("W");
  Serial.print("Energy: ");       Serial.print(dataSensor.energy);     Serial.println("kWh");
  Serial.print("Frequency: ");    Serial.print(dataSensor.frequency); Serial.println("Hz");
  Serial.print("PF: ");           Serial.println(dataSensor.powerFactor);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}