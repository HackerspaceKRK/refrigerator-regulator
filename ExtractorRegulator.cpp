#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "ExtractorRegulator.hpp"
#include "Relays.hpp"

extern int laser_active;
extern bool force_extraction;

void TaskExtractorRegulator(void *pvParameters)
{
    (void) pvParameters;
    for(;;){
      if(force_extraction or laser_active){
        relay_enable(RELAY_EXHAUST);
      }else{
        relay_disable(RELAY_EXHAUST);
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
