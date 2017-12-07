#include "Buttons.hpp"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

extern int lcd_key;

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

void TaskReadButtons(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    lcd_key = read_LCD_buttons();
    vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
