#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "Buttons.hpp"
#include "LCD.hpp"
#include "config.hpp"
#include "Relays.hpp"
#include <Arduino.h>

const byte LCD_RS = 8;
const byte LCD_E = 9;
const byte LCD_D4 = 4;
const byte LCD_D5 = 5;
const byte LCD_D6 = 6;
const byte LCD_D7 = 7;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

byte pwmLevel = 50;

extern byte relay_states[4];

// PERIPHERIALS SCREEN
extern float current_temperature;
extern bool cooling_enabled;
float last_temperature = 0.0;
LiquidLine temperature_line(0, 0, last_temperature, "\337C");
LiquidLine power_line(8, 0, "PWR:", pwmLevel, "%");

LiquidLine cooling_line(0, 1, "COOL:", cooling_enabled);
LiquidLine exhaust_line(8, 1, "EXHA:", relay_states[RELAY_EXHAUST]);
LiquidScreen peripherials_screen(temperature_line, power_line, cooling_line, exhaust_line);

void cooling_off(){
  cooling_enabled = false;
  relay_disable(RELAY_PUMP);
}
void cooling_on(){
  relay_enable(RELAY_PUMP);
  cooling_enabled = true;
}

void exhaust_on(){
  relay_enable(RELAY_EXHAUST);
}
void exhaust_off(){
  relay_disable(RELAY_EXHAUST);
}

// POWER SCREEN
LiquidLine pwm_line(0, 0, "Power: ", pwmLevel, "%");
LiquidLine pwm_coarse_line(0, 1, "COARSE");
LiquidLine pwm_fine_line(9, 1, "FINE");
LiquidScreen pwm_screen(pwm_line, pwm_coarse_line, pwm_fine_line);

void pwm_up_coarse(){
  pwmLevel = min(pwmLevel + 10, 100);
}

void pwm_down_coarse(){
  pwmLevel = max(pwmLevel - 10, 0);
}

void pwm_up_fine(){
  pwmLevel = min(pwmLevel + 2, 100);
}

void pwm_down_fine(){
  pwmLevel = max(pwmLevel - 2, 0);
}

extern int lcd_key;
LiquidMenu menu(lcd);

inline void setupLCD(){
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
}

inline void setupMenu(){
  cooling_line.attach_function(1, cooling_on);
  cooling_line.attach_function(2, cooling_off);
  exhaust_line.attach_function(1, exhaust_on);
  exhaust_line.attach_function(2, exhaust_off);
  power_line.attach_function(1, pwm_up_coarse);
  power_line.attach_function(2, pwm_down_coarse);
  menu.add_screen(peripherials_screen);

  pwm_coarse_line.attach_function(1, pwm_up_coarse);
  pwm_coarse_line.attach_function(2, pwm_down_coarse);
  pwm_fine_line.attach_function(1, pwm_up_fine);
  pwm_fine_line.attach_function(2, pwm_down_fine);
  menu.add_screen(pwm_screen);

  menu.update();
}

void TaskPrintToLCD(void *pvParameters)
{
  (void) pvParameters;

  setupLCD();
  setupMenu();

  int last_lcd_key = 0;
  for(;;){
    if(last_temperature != current_temperature){
      last_temperature = current_temperature;
      menu.update();
    }
    relay_states[RELAY_LASER_ENABLE] = relay_states[RELAY_PUMP] && relay_states[RELAY_EXHAUST];

    if(last_lcd_key != lcd_key){
      last_lcd_key = lcd_key;

      switch (lcd_key){
           case btnRIGHT:{
                menu.call_function(1);
                break;
           }
           case btnLEFT:{
                 menu.call_function(2);
                 break;
           }
           case btnUP:{
                 menu.next_screen();
                 break;
           }
           case btnDOWN:{
                 menu.previous_screen();
                 break;
           }
           case btnSELECT:{
                 menu.switch_focus();
                 break;
           }
           case btnNONE:{
                 break;
           }
      }
    }
    vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}
