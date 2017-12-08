#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"
#include "Relays.hpp"
#include "FridgeRegulator.hpp"

enum states {
  IDLING, COOLING
};

int state = IDLING;
long state_change_time = 0;

extern float current_temperature;

bool cooling_enabled = false;

bool can_cool(){
  return (cooling_enabled and relay_status(RELAY_PUMP));
}

bool fridge_rested(){
  return state_change_time == 0 or ((millis() - state_change_time) > fridge_cycle_time);
}

bool can_disable_pump(){
  return (state == IDLING) and fridge_rested();
}

void go_cooling(){
  Serial.println("# REFRIGERATOR IS GOING ON!");
  state = COOLING;
  relay_enable(RELAY_REFRIGERATOR);
  state_change_time = millis();
}

void go_idling(){
  Serial.println("# REFRIGERATOR IS GOING OFF!");
  state = IDLING;
  relay_disable(RELAY_REFRIGERATOR);
  state_change_time = millis();
}

void TaskRefrigeratorRegulator(void *pvParameters)
{
    (void) pvParameters;
    state_change_time = 0;

    for(;;){
      if(can_cool()){
        if (state == IDLING) {
          if (current_temperature > temperature_high_threshold and fridge_rested()) {
            go_cooling();
          }
        } else if (state == COOLING) {
          if (current_temperature < temperature_low_threshold) {
            go_idling();
          }
        }
      }else if(state == COOLING) {
        go_idling();
      }
      vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
  

