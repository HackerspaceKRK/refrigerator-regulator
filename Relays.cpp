#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "Relays.hpp"
#include "FridgeRegulator.hpp"

extern byte relay_states[4];


void relay_enable(byte relay){
    relay_states[relay] = HIGH;
}

void relay_disable(byte relay){
    relay_states[relay] = LOW;
}

bool relay_status(byte relay){
    return relay_states[relay];
}


void TaskRelays(void *pvParameters)
{
    (void) pvParameters;

    for(auto i = 0; i<4; i++){
      digitalWrite(relay_pin[i], HIGH);
      pinMode(relay_pin[i], OUTPUT);
    }

    for(;;){
      for(char i = 0; i<4; i++){
        if(i != RELAY_PUMP or relay_status(RELAY_PUMP) or can_disable_pump()){
          digitalWrite(relay_pin[i], !relay_states[i]);
        }
      }
      vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
