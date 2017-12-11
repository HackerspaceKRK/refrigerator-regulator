#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "PowerRegulation.hpp"

byte pwmLevel = 70;


inline void setPwmFrequency() {
    TCCR3B = TCCR3B & 0b11111000 | 0x01;
}

void TaskPowerRegulation(void *pvParameters)
{
    (void) pvParameters;

     pinMode(power_regulation_pin, OUTPUT);
     setPwmFrequency();

    for(;;){
      analogWrite(
          power_regulation_pin,
          map(pwmLevel, 0, 100, lower_power_limit, upper_power_limit)
      );
      vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
