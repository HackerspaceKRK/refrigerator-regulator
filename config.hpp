#include <Arduino.h>

const int ONE_WIRE_BUS = 2;

const float temperature_low_threshold = 12.0;
const float temperature_high_threshold = 18.0;
const int interval = 3000;

const unsigned int relay_pin[] = {A4, A3, A5, A1};

enum relays {
  RELAY_REFRIGERATOR, RELAY_PUMP, RELAY_AIR_ASSIST, RELAY_EXHAUST
};

const int power_regulation_pin = 3;


/*
 * Pins in use:
 * Serial 0-1
 * free: 2
 * power regulation: 3
 * LCD 4-9
 * wtf 10-12
 * LED 13
 * Buttons: A0
 * Relays A1-A4
 * 
 */



 /*
  * Relays
  * 1 - water pump
  * 2 - cooler
  * 3 - exhaust fan + air assist
  * 4 - (laser enable?)
  */
