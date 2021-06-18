#include <Arduino.h>
#include <DallasTemperature.h>

class SensTemp{
String concat_result(float* result, int numSensors);
public:
  String request_temperatures(float* result, int numSensors, DallasTemperature sensors1);
};