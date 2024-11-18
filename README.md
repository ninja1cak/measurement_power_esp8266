# WIRING DOCUMENTATION

## LCD TFT ili9341

Source : https://thesolaruniverse.wordpress.com/2021/05/02/wiring-an-ili9341-spi-tft-display-with-esp8266-based-microcontroller-boards-nodemcu-and-wemos-d1-mini/
__Note__ :
- We can use library from adafruit (BusIO, ILI9341, GFX-Libray) or we can use library from TFT_eSPI

| PIN_NAME      | PIN           | CABLE_COLOR   | 
| ------------- |:-------------:|:-------------:|
| VCC           | 3V3           | BROWN
| GND           | GND           | RED
| CS            | D2            | ORANGE
| RESET         | D3            | YELLOW
| DC            | D4            | GREEN
| SDI/MOSI      | D7            | BLUE
| SCK           | D5            | PURPLE
| LED           | 3V3           | BLACK
			

## Sensor PZEM

Source : https://innovatorsguru.com/pzem-004t-v3/

__Note__ : 
- We use wiring from sensor to esp8266 like table down below, because when using D5 as a pin TX sensor not working. 

| PIN_NAME      | PIN           | CABLE_COLOR   | 
| ------------- |:-------------:|:-------------:|
| 5V            | VU            | BROWN
| GND           | GND           | RED
| RX            | D5            | PURPLE
| TX            | D6            | BLUE

