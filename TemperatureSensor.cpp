#include <Arduino_FreeRTOS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TemperatureSensor.hpp"
#include "config.hpp"

extern float current_temperature;

void TaskTemperatureSensor(void *pvParameters)
{
  (void) pvParameters;
  
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  sensors.begin();
  
  for(;;)
  {
    sensors.requestTemperatures();
    current_temperature = sensors.getTempCByIndex(0);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
