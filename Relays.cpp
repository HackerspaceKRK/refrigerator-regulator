#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "Relays.hpp"
#include "FridgeRegulator.hpp"

extern byte relay_states[4];
extern bool venting_confiirmed;
extern bool powering_off;


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

    pinMode(13, OUTPUT);

    for(auto i = 0; i<4; i++){
      digitalWrite(relay_pin[i], HIGH);
      pinMode(relay_pin[i], OUTPUT);
    }

    for(;;){
      if(powering_off){
        relay_states[RELAY_LASER_ENABLE] = LOW;
        if(can_disable_pump()){
          digitalWrite(SELF_POWERDOWN, LOW);
        }
      }else{
        relay_states[RELAY_LASER_ENABLE] = relay_states[RELAY_PUMP] && venting_confiirmed;
      }
      digitalWrite(13, relay_states[RELAY_LASER_ENABLE]);

      for(char i = 0; i<4; i++){
        if(i != RELAY_PUMP or relay_status(RELAY_PUMP) or can_disable_pump()){
          digitalWrite(relay_pin[i], !relay_states[i]);
        }
      }
      vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
