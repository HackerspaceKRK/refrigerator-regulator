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

extern float current_temperature;
extern byte relay_states[4];
extern bool cooling_enabled;

void refrigerator_down(){
  cooling_enabled = false;
}
void pump_down(){
  cooling_enabled = false;
  relay_disable(RELAY_PUMP);
}
void pump_up(){
  relay_enable(RELAY_PUMP);
}
void refrigerator_up(){
  pump_up();
  cooling_enabled = true;
}

void air_assist_down(){
  relay_disable(RELAY_AIR_ASSIST);
}
void exhaust_up(){
  relay_enable(RELAY_EXHAUST);
}
void exhaust_down(){
  air_assist_down();
  relay_disable(RELAY_EXHAUST);
}
void air_assist_up(){
  exhaust_up();
  relay_enable(RELAY_AIR_ASSIST);
}

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


LiquidLine air_assist_line(0, 1, " AIR:", relay_states[2]);
LiquidLine exhaust_line(9, 1, "EXHA:", relay_states[3]);
LiquidScreen cooling_screen(air_assist_line, exhaust_line);

LiquidLine pwm_line(0, 0, "Power: ", pwmLevel);
LiquidLine pwm_coarse_line(0, 1, "COARSE");
LiquidLine pwm_fine_line(9, 1, "FINE");

LiquidScreen pwm_screen(pwm_line, pwm_coarse_line, pwm_fine_line);

LiquidMenu menu(lcd);

extern int lcd_key;

float last_temperature = 0.0;



LiquidLine temperature_line(0, 0, "TEMP:", last_temperature);
LiquidLine refrigerator_line(0, 1, "COOL:", cooling_enabled);
LiquidLine pump_line(9, 1, "PUMP:", relay_states[1]);
LiquidScreen temperature_screen(temperature_line, refrigerator_line, pump_line);


void TaskPrintToLCD(void *pvParameters)
{
    (void) pvParameters;
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
    
  refrigerator_line.attach_function(1, refrigerator_up);
  refrigerator_line.attach_function(2, refrigerator_down);
  pump_line.attach_function(1, pump_up);
  pump_line.attach_function(2, pump_down);
  air_assist_line.attach_function(1, air_assist_up);
  air_assist_line.attach_function(2, air_assist_down);
  exhaust_line.attach_function(1, exhaust_up);
  exhaust_line.attach_function(2, exhaust_down);
    

 /* temperature_line.attach_function(1, pwm_up);
  temperature_line.attach_function(2, pwm_down);
  */
  pwm_coarse_line.attach_function(1, pwm_up_coarse);
  pwm_coarse_line.attach_function(2, pwm_down_coarse);
  pwm_fine_line.attach_function(1, pwm_up_fine);
  pwm_fine_line.attach_function(2, pwm_down_fine);

  menu.add_screen(temperature_screen);
  menu.add_screen(cooling_screen);
  menu.add_screen(pwm_screen);
//uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

  int last_lcd_key = 0;
    menu.update();

  for(;;){
    if(last_temperature != current_temperature){
      last_temperature = current_temperature;
      menu.update();
    }
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
  
    
