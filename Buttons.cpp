#include "Buttons.hpp"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "config.hpp"

extern int lcd_key;
extern int laser_active;

inline int read_LCD_buttons(){
    int adc_key_in = analogRead(0);
    if (adc_key_in > 1000) return btnNONE;
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
    return btnNONE;
}

void check_laser_active(){
  laser_active = digitalRead(LASER_ACTIVE_PIN) == LOW;
}

void TaskReadButtons(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    lcd_key = read_LCD_buttons();
    laser_active = digitalRead(LASER_ACTIVE_PIN) == LOW;
    Serial.print("LASER PIN: ");
    Serial.println(laser_active);
    vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
