#include <Arduino.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#include "class.h"

String SensTemp::request_temperatures(float* result, int numSensors, DallasTemperature sensors1)
{
  sensors1.requestTemperatures(); 
//  sensors2.requestTemperatures(); 
//  sensors3.requestTemperatures(); 
//  for(int i=0; i<numSensors; i++)
//  {
//    result[i] = i;
//  }

//  result[2] = sensors1.getTempCByIndex(0);
  result[0] = sensors1.getTempCByIndex(0);
  result[1] = sensors1.getTempCByIndex(1);
  result[2] = sensors1.getTempCByIndex(2);
  result[3] = sensors1.getTempCByIndex(3);
  result[4] = sensors1.getTempCByIndex(4);

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