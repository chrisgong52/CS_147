#include <Arduino.h>
#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#define blue_pin 33
#define red_pin 25
#define button_pin 15

#include <HttpClient.h>
#include <WiFi.h>
#include <inttypes.h>
#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"

LSM6DSO myIMU;

char ssid[50];
char pass[50];

const int kNetworkTimeout = 30 * 1000;
const int kNetworkDelay = 1000;

float x_sum = 0;
float y_sum = 0;
float z_sum = 0;

int light_counter;
int dark_counter;

void nvs_access() {
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
    err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  // Open
  Serial.printf("\n");
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... ");
  nvs_handle_t my_handle;
  err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    Serial.printf("Done\n");
    Serial.printf("Retrieving SSID/PASSWD\n");
    size_t ssid_len;
    size_t pass_len;
    err = nvs_get_str(my_handle, "ssid", ssid, &ssid_len);
    err |= nvs_get_str(my_handle, "pass", pass, &pass_len);
    switch (err) {
      case ESP_OK:
      Serial.printf("Done\n");
      //Serial.printf("SSID = %s\n", ssid);
      //Serial.printf("PASSWD = %s\n", pass);
      break;
      case ESP_ERR_NVS_NOT_FOUND:
      Serial.printf("The value is not initialized yet!\n");
      break;
      default:
      Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
  }
  // Close
  nvs_close(my_handle);
}

void setup() {
  Serial.begin(9600);
  sleep(1);
  Wire.begin();
  sleep(1);

  int light_counter = 0;
  int dark_counter = 0;

  nvs_access();
  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }
  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("IMU Loaded Settings.");
  
  pinMode(blue_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(button_pin, INPUT);

  int counter = 0;
  while (true)
  {
    counter += 1;
    x_sum += myIMU.readFloatGyroX();
    y_sum += myIMU.readFloatGyroY();
    z_sum += myIMU.readFloatGyroZ();
    digitalWrite(blue_pin, HIGH);
    delay(500);
    if (counter == 10){
      break;
    }
    digitalWrite(blue_pin, LOW);
    delay(500);
  }

  x_sum = x_sum/counter;
  y_sum = y_sum/counter;
  z_sum = z_sum/counter;
}

void loop() {
  if (digitalRead(button_pin) == HIGH){
    Serial.println("BUTTON PRESSED");
  }

  WiFiClient c;
  HttpClient http(c);
  int err = 0;
  
  Serial.println();

  std::string temp1 = "";

  float current_x = myIMU.readFloatGyroX();
  float current_y = myIMU.readFloatGyroY();
  float current_z = myIMU.readFloatGyroZ();

  if (abs(current_x - x_sum) > 0.5 or abs(current_y - y_sum) > 0.5 or abs(current_z - z_sum) > 0.5) {
    Serial.println("PLANT TIPPED OVER");
    temp1 += "/?tip=true&";
    temp1 += "health=bad&";
   
    digitalWrite(red_pin, HIGH);
  } else {
    temp1 += "/?tip=false&";
    if (myIMU.readTempF() > 80 || myIMU.readTempF() < 60) {
      temp1 += "health=bad&";
    } else {
      temp1 += "health=good&";
    }
    digitalWrite(red_pin, LOW);
  }

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 3);
  
  int light_sensor = analogRead(32);
  Serial.print("\nLight Sensor: ");
  Serial.println(light_sensor);
  if (light_sensor > 3500){
    light_counter += 1;
  } else {
    dark_counter += 1;
  }

  temp1 += "day=" + std::to_string(light_counter) + "&";
  temp1 += "darkness=" + std::to_string(dark_counter) +"&";

  temp1 += "temp=" + std::to_string(myIMU.readTempF());

  http.post("3.21.98.211", 5000, temp1.c_str(), NULL);

  Serial.printf(temp1.c_str());

  sleep(1);
}