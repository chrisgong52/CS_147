#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Adafruit_CAP1188.h"

TFT_eSPI tft = TFT_eSPI();
Adafruit_CAP1188 cap = Adafruit_CAP1188();

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