#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#include "class.h"

#define ONE_WIRE_BUS_1 13
OneWire oneWire1(ONE_WIRE_BUS_1); 
DallasTemperature sensors1(&oneWire1);

//#define ONE_WIRE_BUS_2 4
//OneWire oneWire2(ONE_WIRE_BUS_2); 
//DallasTemperature sensors2(&oneWire2);

//#define ONE_WIRE_BUS_3 4
//OneWire oneWire3(ONE_WIRE_BUS_3); 
//DallasTemperature sensors3(&oneWire3);

SensTemp temperature;

// In case you may need faster transmition or less battery usage you may
// look into "esp-now".

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define HIGH_ACCURACY

VL53L0X sensor;

DeviceAddress therm_1, therm_2, therm_3, therm_4, therm_5;

const int pot_pin = 16;
int counter = 0;

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
//  int test_temp = 50;
  float result[5];

//  Serial.println(temperature.request_temperatures(result, 5, sensors));
//  return String(test_temp);
  return String(temperature.request_temperatures(result, 5, sensors1));
//  return String(temperature.request_temperatures(result, 2, sensors1, sensors2));
}

String readHumi() {
  float test_humidity = 34.6;
  return String(test_humidity);
}

String readPres() {
  int test_pressure = 482;
  return String(test_pressure);
}

String readData(){
//  counter += 1;
//  Serial.print(sensor.readRangeSingleMillimeters());
  // String return_string = String(sensor.readRangeSingleMillimeters());
//  String return_string;
//  return_string += String(counter);
//  return_string += (" ");
  String return_string = String(sensor.readRangeSingleMillimeters());
  // this needs to be readjusted so it doesnt print to serial in order
  // to prevent crashing of the matlab script
//  if(sensor.timeoutOccurred()) Serial.print("TIMEOUT");
  //if(sensor.timeoutOccurred()) return_string.append("TIMEOUT");

  // fake a second to fifth sensor input
  // int value_second_sensor = analogRead(pot_pin);
//  float value_second_sensor = 30.52;
//  float value_third_sensor = 2.46;
//  float value_fourth_sensor = 300.1634;
//  float value_fifth_sensor = 65.635;

//  return_string += (" ");
//  return_string += (String(value_second_sensor));
//  return_string += (" ");
//  return_string += (String(value_third_sensor));
//  return_string += (" ");
//  return_string += (String(value_fourth_sensor));
//  return_string += (" ");
//  return_string += (String(value_fifth_sensor));

//  Serial.print(' ');                        // space as separator for matlab to detect next value
//  Serial.print(value_second_sensor);
//  Serial.print(' ');
//  Serial.print(value_third_sensor);
//  Serial.print(' ');
//  Serial.print(value_fourth_sensor);
//  Serial.print(' ');
//  Serial.println(value_fifth_sensor);      // last print has to be newline (println)

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
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readData().c_str());
  });
  
  // bool status;
  
  // Start server
  server.begin();

  // temp sens
  sensors1.begin();

  Serial.println(sensors1.getDeviceCount(), DEC);

  if (!sensors1.getAddress(therm_1, 0)) Serial.println("nummer 1 verkackt");
  if (!sensors1.getAddress(therm_2, 1)) Serial.println("nummer 2 verkackt");
  if (!sensors1.getAddress(therm_3, 2)) Serial.println("nummer 3 verkackt");
  if (!sensors1.getAddress(therm_4, 3)) Serial.println("nummer 4 verkackt");
  if (!sensors1.getAddress(therm_5, 4)) Serial.println("nummer 5 verkackt");

//  sensors2.begin();
//  sensors3.begin();

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
  Serial.println(readTemp().c_str());
  Serial.println(readData().c_str());
}

/*
// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include <Wire.h>

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";


//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  float test = 10;
  return String(test);
  //return String(1.8 * bme.readTemperature() + 32);
}

String readHumi() {
  float test = 40;
  return String(test);
}

String readPres() {
  float test = 64;
  return String(test);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
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
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
  
  
  // Start server
  server.begin();
}
 
void loop(){
  
}*/