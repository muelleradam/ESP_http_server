#include <Arduino.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#include "class.h"

String SensTemp::request_temperatures(float* result, int numSensors, DallasTemperature sensors)
{
  sensors.requestTemperatures(); 
  for(int i=0; i<numSensors; i++)
  {
    result[i] = sensors.getTempCByIndex(i);
  }

  return SensTemp::concat_result(result, numSensors);
}

String SensTemp::concat_result(float* result, int numSensors)
{
  String resultString;
  for(int i=0; i<numSensors; i++)
  {
    resultString = resultString + ' ' + result[i];
  }

  return resultString;
}