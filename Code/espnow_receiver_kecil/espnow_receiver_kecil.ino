
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC D4
#define TFT_CS D2

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


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
  Serial.print("Voltage: ");      Serial.print((isNumber(dataSensor.voltage)) ? dataSensor.voltage : 0);      Serial.println("V");
  Serial.print("Current: ");      Serial.print(dataSensor.current);      Serial.println("A");
  Serial.print("Power: ");        Serial.print(dataSensor.power);        Serial.println("W");
  Serial.print("Energy: ");       Serial.print(dataSensor.energy);     Serial.println("kWh");
  Serial.print("Frequency: ");    Serial.print(dataSensor.frequency); Serial.println("Hz");
  Serial.print("PF: ");           Serial.println(dataSensor.powerFactor);
  Serial.println();

  tft.setCursor(0, 0);
  measurementText(ILI9341_WHITE, 2, "V: ", "V", 0, 0);
  measurementText(ILI9341_WHITE, 2, "I: ", "A", 0, 20);
  measurementText(ILI9341_WHITE, 2, "P: ", "W",0, 40);
  measurementText(ILI9341_WHITE, 2, "E: ", "kWh",0, 60);
  measurementText(ILI9341_WHITE, 2, "F: ", "Hz",0, 80);

  tft.setTextSize(2);
  tft.setCursor(25,0);
  tft.fillRect(25,20, 20, 20, ILI9341_RED);
  tft.println((isNumber(dataSensor.voltage)) ? dataSensor.voltage : 0.00000);
  tft.setCursor(25,20);
  tft.println((isNumber(dataSensor.current)) ? dataSensor.current : 0.00000);
  tft.setCursor(25,40);
  tft.println((isNumber(dataSensor.power)) ? dataSensor.power : 0.00000);
  tft.setCursor(25,60);
  tft.println((isNumber(dataSensor.energy)) ? dataSensor.energy : 0.00000);
  tft.setCursor(25,80);
  tft.println((isNumber(dataSensor.frequency)) ? dataSensor.frequency : 0.00000);

} 
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

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

void measurementText(int textColor, int textSize, String text, String metric,int pointX, int pointY){
  tft.setCursor(pointX, pointY);
  tft.setTextColor(textColor);  
  tft.setTextSize(textSize);
  tft.println(text);
  
  tft.setCursor(100, pointY);
  tft.println(metric);
}

bool isNumber(float number){
  return (number == number) ? true : false;
}

void loop() {


  // tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  // tft.println("V : ");


}