#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include "Buttons.hpp"
#include "Relays.hpp"
#include "TemperatureSensor.hpp"
#include "PowerRegulation.hpp"
#include "FridgeRegulator.hpp"
#include "config.hpp"
#include "LCD.hpp"

#if defined(__AVR_ATmega2560__)
const int TASK_STACK_SMALL = 64;
const int TASK_STACK_HUGE = 256;
#else
#error It won't fit...
#endif


void setup() {
     Serial.begin(115200);
     Serial.println("# Refrigerator Regulator!");
     Serial.println();


  xTaskCreate(
    TaskTemperatureSensor
    ,  NULL
    ,  TASK_STACK_HUGE
    ,  NULL
    ,  1
    ,  NULL );

  xTaskCreate(
    TaskPowerRegulation
    ,  NULL
    ,  TASK_STACK_SMALL
    ,  NULL
    ,  2
    ,  NULL );

  xTaskCreate(
    TaskReadButtons
    ,  NULL
    ,  TASK_STACK_SMALL
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  xTaskCreate(
    TaskRelays
    ,  NULL
    ,  TASK_STACK_SMALL
    ,  NULL
    ,  3  // Priority
    ,  NULL );

  xTaskCreate(
    TaskRefrigeratorRegulator
    ,  NULL
    ,  TASK_STACK_SMALL
    ,  NULL
    ,  3  // Priority
    ,  NULL );


  xTaskCreate(
    TaskPrintToLCD
    ,  NULL
    ,  TASK_STACK_HUGE
    ,  NULL
    ,  0  // Priority
    ,  NULL );

}

float current_temperature = 0.0;
int lcd_key = btnNONE;

byte relay_states[] = {LOW, HIGH, LOW, LOW};

void loop() {
}
