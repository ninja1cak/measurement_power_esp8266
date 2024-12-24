
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

const int tableRows = 6;
const int tableCols = 4;
int cellWidth;
int voltageWidth = 30;
const int defaultCellWidth = 75;
const int cellHeight = 30;
const int startX = 0;
const int startY = 10;

String dummyData[tableRows][tableCols] = {
  {"S", "V", "I", "P"},
  {"1", "0.000", "0.000", "0.000"},
  {"2", "0.000", "0.000", "0.000"},
  {"3", "0.000", "0.000", "0.000"},
  {"4", "0.000", "0.000", "0.000"},
  {"5", "0.000", "0.000", "0.000"},
};

String data[tableRows][tableCols];
String TABLE_HEADER[tableCols] = {"S", "V (V)", "I (A)", "P (W)"};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&dataSensor, incomingData, sizeof(dataSensor));
  
  Serial.print("Bytes received: "); 
  Serial.println(len);
  serialDebug(&dataSensor);

  tft.setCursor(0, 0);

  struct_message* checkData = &dataSensor;
  
  for(int col = 0; col<5; col++){
    data[0][col] = TABLE_HEADER[col];
  }

  if(checkData->id == 1){
    saveValueOnArray(&dataSensor);
  }
  
  if(checkData->id == 2){
    saveValueOnArray(&dataSensor);
  }

  if(checkData->id == 3){
    saveValueOnArray(&dataSensor);
  }

  if(checkData->id == 4){
    saveValueOnArray(&dataSensor);
  }

  if(checkData->id == 5){
    saveValueOnArray(&dataSensor);
  }

  drawTable(data);
  
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
  drawTable(dummyData);
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

void saveValueOnArray(struct struct_message* dataSensor){
  data[dataSensor->id][0] = String(dataSensor->id);
  data[dataSensor->id][1] = String(dataSensor->voltage, 3);
  data[dataSensor->id][2] = String(dataSensor->current, 3);
  data[dataSensor->id][3] = String(dataSensor->current*dataSensor->voltage, 3);
}

void loop() {
  // tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  // tft.println("V : ");

}

void drawTable(String data[tableRows][tableCols]) {
  // Draw the table grid
  for (int row = 0; row <= tableRows; row++) {
    int y = startY + row * cellHeight;
    tft.drawLine(startX, y, startX + tableCols * defaultCellWidth, y, TFT_WHITE); // Horizontal line
  }

  for (int col = 0; col <= tableCols; col++) {
    Serial.println("test");

    cellWidth = col == 1 ? voltageWidth : defaultCellWidth;
   
    Serial.println(cellWidth);
    int x = startX + col * cellWidth;
    tft.drawLine(x, startY, x, startY + tableRows * cellHeight, TFT_WHITE); // Vertical line
  }

  // Fill in the table with some example data
  for (int row = 0; row < tableRows; row++) {
    for (int col = 0; col < tableCols; col++) {
      String text = "R" + String(row + 1) + "C" + String(col + 1); // Example text
      cellWidth = col == 1  ? voltageWidth : defaultCellWidth; 
      int textX = startX + col * cellWidth + 5; // Add some padding
      int textY = startY + row * cellHeight + 5; // Add some padding
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextSize(2);
      tft.setCursor(textX, textY);
      tft.print(data[row][col]);
    }
  }
}
