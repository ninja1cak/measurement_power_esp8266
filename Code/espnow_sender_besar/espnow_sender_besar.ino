#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN D6
#define PZEM_TX_PIN D5
#endif


SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);


#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x8C, 0xCE, 0x4E, 0xCA, 0x5C, 0x77};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id;
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
  float powerFactor;

} struct_message;

// Create a struct_message called dataSensor
struct_message dataSensor;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

bool isNumber(float number){
  return number == number ? true : false;
}

void loop() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float powerFactor = pzem.pf();
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    dataSensor.id = 1;
    dataSensor.voltage = isNumber(voltage) ? voltage : 0.0;
    dataSensor.current = isNumber(current) ? current : 0.0;
    dataSensor.power = isNumber(power) ?  power : 0.0;
    dataSensor.energy = isNumber(energy) ? energy : 0.0;
    dataSensor.frequency = isNumber(frequency) ? frequency : 0.0;
    dataSensor.powerFactor = isNumber(powerFactor) ? powerFactor : 0.0;
    // Send message via ESP-NOW

    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(powerFactor);
    esp_now_send(broadcastAddress, (uint8_t *) &dataSensor, sizeof(dataSensor));

    lastTime = millis();
  }
}
