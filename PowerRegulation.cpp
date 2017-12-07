#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "PowerRegulation.hpp"

extern byte pwmLevel;


inline void setPwmFrequency() {
    TCCR2B = TCCR2B & 0b11111000 | 0x01;
}

void TaskPowerRegulation(void *pvParameters)
{
    (void) pvParameters;

     pinMode(power_regulation_pin, OUTPUT);
     setPwmFrequency();

    for(;;){
      analogWrite(
          power_regulation_pin,
          map(pwmLevel, 0, 100, 255, 0)
      );
      vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
