#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include "class.h"

#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// In case you may need faster transmition or less battery usage you may
// look into "esp-now".

#define HIGH_ACCURACY
#define NUM_SENSORS 5

SensTemp temperature;

VL53L0X sensor;

DeviceAddress therm_1, therm_2, therm_3, therm_4, therm_5;

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  float result[NUM_SENSORS];
  return String(temperature.request_temperatures(result, NUM_SENSORS, sensors));
}

//String readHumi() {
//  float test_humidity = 34.6;
//  return String(test_humidity);
//}
//
//String readPres() {
//  int test_pressure = 482;
//  return String(test_pressure);
//}

String readTOF(){
  String return_string = String(sensor.readRangeSingleMillimeters());
  return return_string;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Serial.println();
  Serial.println("Starting ESP-Testrig...");
  int SDA = 14;
  int SCL = 15;
  Wire.begin(SDA,SCL);
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
//  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readHumi().c_str());
//  });
//  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readPres().c_str());
//  });
  server.on("/tof", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTOF().c_str());
  });
  
  
  // Start server
  server.begin();

  // temp sensors
  sensors.begin();

  if (!sensors.getAddress(therm_1, 0)) Serial.println("sens 1 failed");
  if (!sensors.getAddress(therm_2, 1)) Serial.println("sens 2 failed");
  if (!sensors.getAddress(therm_3, 2)) Serial.println("sens 3 failed");
  if (!sensors.getAddress(therm_4, 3)) Serial.println("sens 4 failed");
  if (!sensors.getAddress(therm_5, 4)) Serial.println("sens 5 failed");

  sensor.setTimeout(500);
  if(!sensor.init())
  {
    Serial.println("Failed to detect and init sensor!");
    while(1){}
  }

  #if defined HIGH_ACCURACY
    sensor.setMeasurementTimingBudget(200000);
  #endif
}
 
void loop(){
  // only for debug purposes, delete in final
//  Serial.println(readTemp().c_str());
//  Serial.println(readTOF().c_str());
}