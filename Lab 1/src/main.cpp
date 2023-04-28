#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Adafruit_CAP1188.h"

TFT_eSPI tft = TFT_eSPI();
uint8_t k_sclk = 25;
uint8_t k_mosi = 32;
uint8_t k_miso = 33;
uint8_t k_cspin = 26;
int8_t k_reset = 27;
// Adafruit_SPIDevice *cap = new Adafruit_SPIDevice(k_cspin, k_sclk, k_miso, k_mosi, 2000000);
Adafruit_CAP1188 cap = Adafruit_CAP1188(k_sclk, k_miso, k_mosi, k_cspin, 0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  cap.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.setCursor(5,3);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(5);
  int touched = cap.touched();
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (touched & (1 << i)) {
      count++;
    }
  }
  tft.print(count);
  Serial.println(count);
  usleep(250000);
  tft.fillScreen(TFT_BLACK);
}