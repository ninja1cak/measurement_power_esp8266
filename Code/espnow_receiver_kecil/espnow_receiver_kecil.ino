
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library

#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
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

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&dataSensor, incomingData, sizeof(dataSensor));
  
  Serial.print("Bytes received: ");
  Serial.println(len);
  serialDebug(&dataSensor);

  tft.setCursor(0, 0);

  struct_message* checkData = &dataSensor;

  if(checkData->id == 1){
    printMeasurementTextOnLCD(0, 0, 105);
    printValueMeasurementOnLCD(25, 0, &dataSensor);
  }
  
  if(checkData->id == 2){
    printMeasurementTextOnLCD(155, 0, 260);
    printValueMeasurementOnLCD(180, 0, &dataSensor);
  }

  if(checkData->id == 3){
    printMeasurementTextOnLCD(0, 120, 105);
    printValueMeasurementOnLCD(25, 120, &dataSensor);
  }

  if(checkData->id == 4){
    printMeasurementTextOnLCD(155, 120, 260);
    printValueMeasurementOnLCD(180, 120, &dataSensor);
  }
  
} 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info

  printMeasurementTextOnLCD(0, 0, 105);  
  printMeasurementTextOnLCD(155, 0, 260);
  printMeasurementTextOnLCD(0, 120, 105);
  printMeasurementTextOnLCD(155, 120, 260);



  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void serialDebug(struct struct_message* dataSensor){
  Serial.print("Alat: ");         Serial.println(dataSensor->id);
  Serial.print("Voltage: ");      Serial.print(dataSensor->voltage);      Serial.println("V");
  Serial.print("Current: ");      Serial.print(dataSensor->current);      Serial.println("A");
  Serial.print("Power: ");        Serial.print(dataSensor->power);        Serial.println("W");
  Serial.print("Energy: ");       Serial.print(dataSensor->energy);     Serial.println("kWh");
  Serial.print("Frequency: ");    Serial.print(dataSensor->frequency); Serial.println("Hz");
  Serial.print("PF: ");           Serial.println(dataSensor->powerFactor);
  Serial.println();
}

void printValueMeasurementOnLCD(int pointX, int pointY, struct struct_message* dataSensor){
  tft.setCursor(pointX,pointY);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(dataSensor->voltage );
  tft.setCursor(pointX,pointY+20);
  tft.println(dataSensor->current);
  tft.setCursor(pointX,pointY+40);
  tft.println(dataSensor->power);
  tft.setCursor(pointX,pointY+60);
  tft.println(dataSensor->energy);
  tft.setCursor(pointX,pointY+80);
  tft.println(dataSensor->frequency);
}

void printMeasurementTextOnLCD(int pointX, int pointY, int pointMetricX){
  measurementText(TFT_WHITE, 2, "V: ", "V", pointX, pointY, pointMetricX);
  measurementText(TFT_WHITE, 2, "I: ", "A", pointX, pointY+20, pointMetricX);
  measurementText(TFT_WHITE, 2, "P: ", "W", pointX, pointY+40, pointMetricX);
  measurementText(TFT_WHITE, 2, "E: ", "kWh",pointX, pointY+60, pointMetricX);
  measurementText(TFT_WHITE, 2, "F: ", "Hz",pointX, pointY+80, pointMetricX);
}

void measurementText(int textColor, int textSize, String text, String metric,int pointX, int pointY, int pointMetricX){
  tft.setCursor(pointX, pointY);
  tft.setTextColor(textColor, TFT_BLACK);  
  tft.setTextSize(textSize);
  tft.println(text);
  
  tft.setCursor(pointMetricX, pointY);
  tft.println(metric);
}

void loop() {


  // tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  // tft.println("V : ");


}