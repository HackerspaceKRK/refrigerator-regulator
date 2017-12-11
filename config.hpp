#include <Arduino.h>

const byte LCD_RS = 8;
const byte LCD_E = 9;
const byte LCD_D4 = 4;
const byte LCD_D5 = 5;
const byte LCD_D6 = 6;
const byte LCD_D7 = 7;

const int ONE_WIRE_BUS = 2;
const int LASER_ACTIVE_PIN = 11;
const int SELF_POWERDOWN = 22;

const float temperature_low_threshold = 14.0;
const float temperature_high_threshold = 24.0;
const int interval = 3000;

const unsigned int relay_pin[] = {A4, A3, A5, A1};

enum relays {
  RELAY_REFRIGERATOR, RELAY_PUMP, RELAY_EXHAUST, RELAY_LASER_ENABLE
};

const int power_regulation_pin = 3;

const long fridge_cycle_time = 600000L;

const int lower_power_limit = 220;
const int upper_power_limit = 75;

/*
 * Pins in use:
 * Serial 0-1
 * one wire: 2
 * power regulation: 3
 * LCD 4-9
 * wtf?! 10
 * laser active 11
 * LED 13
 * Buttons: A0
 * Relays A1, A3, A4, A5
 *
 */



 /*
  * Relays
  * 1 - water pump
  * 2 - cooler
  * 3 - exhaust fan + air assist
  * 4 - (laser enable?)
  */
