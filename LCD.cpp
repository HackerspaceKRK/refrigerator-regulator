#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "Buttons.hpp"
#include "LCD.hpp"
#include "config.hpp"
#include "Relays.hpp"
#include <Arduino.h>

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

extern byte pwmLevel;
extern byte relay_states[4];

// PERIPHERIALS SCREEN
extern float current_temperature;
extern bool cooling_enabled;
extern bool force_extraction;
extern bool venting_confiirmed;
extern bool powering_off;

float last_temperature = 0.0;
LiquidLine temperature_line(0, 0, last_temperature, "\337C");
LiquidLine power_line(8, 0, "PWR:", pwmLevel, "%");

// LiquidLine cooling_line(0, 1, "COOL:", cooling_enabled);
// LiquidLine exhaust_line(8, 1, "EXHA:", last_laser_active);
LiquidLine powerdown_line(0, 0, "WYLACZ LASER?");
LiquidLine exhaust_line(0, 1, "WENTYLUJ?: ", relay_states[RELAY_EXHAUST]);

// LiquidLine laser_active_line(14, 1, laser_active);

//
// void cooling_off(){
//   cooling_enabled = false;
//   relay_disable(RELAY_PUMP);
// }
// void cooling_on(){
//   relay_enable(RELAY_PUMP);
//   cooling_enabled = true;
// }

void exhaust_on(){
  force_extraction = true;
}
void exhaust_off(){
  force_extraction = false;
}

// POWER SCREEN
LiquidLine pwm_line(0, 0, "Power: ", pwmLevel, "%");
LiquidLine pwm_coarse_line(0, 1, "COARSE");
LiquidLine pwm_fine_line(9, 1, "FINE");

LiquidScreen peripherials_screen(temperature_line, power_line, pwm_coarse_line, pwm_fine_line);
LiquidScreen secondary_screen(powerdown_line, exhaust_line);

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

LiquidLine powering_off_line(0, 0, "OGRZEWANIE...");
LiquidLine cancel_powering_off_line(0, 1, "ANULUJ");
LiquidScreen powering_off_screen(powering_off_line, cancel_powering_off_line);
LiquidMenu powering_off_menu(lcd, powering_off_screen);

LiquidLine vented_line(0, 0, "ODPOWIETRZONY?");
LiquidLine vented_no_line(0, 1, "NIE");
LiquidLine vented_yes_line(9, 1, "TAK");
LiquidScreen vented_screen(vented_line, vented_no_line, vented_yes_line);
LiquidMenu startupMenu(lcd, vented_screen);

LiquidSystem menu_system(startupMenu, menu, powering_off_menu);

void switch_to_running(){
  menu_system.change_menu(menu);
  venting_confiirmed=true;
  cooling_enabled = true;
}

void power_off(){
  cooling_enabled = false;
  venting_confiirmed = false;
  powering_off = true;
  menu_system.change_menu(powering_off_menu);
}

void cancel_power_off(){
  powering_off = false;
  venting_confiirmed = true;
  cooling_enabled = true;
  menu_system.change_menu(menu);
}

void configurePoweringOffMenu(){
  cancel_powering_off_line.attach_function(1, cancel_power_off);
  cancel_powering_off_line.attach_function(2, cancel_power_off);
}

void configureStartupMenu(){
  vented_no_line.attach_function(1, power_off);
  vented_no_line.attach_function(2, power_off);
  vented_yes_line.attach_function(1, switch_to_running);
  vented_yes_line.attach_function(2, switch_to_running);
}

void configureMenu(){
  // cooling_line.attach_function(1, cooling_on);
  // cooling_line.attach_function(2, cooling_off);

  pwm_coarse_line.attach_function(1, pwm_up_coarse);
  pwm_coarse_line.attach_function(2, pwm_down_coarse);
  pwm_fine_line.attach_function(1, pwm_up_fine);
  pwm_fine_line.attach_function(2, pwm_down_fine);
  // power_line.attach_function(1, pwm_up_coarse);
  // power_line.attach_function(2, pwm_down_coarse);
  menu.add_screen(peripherials_screen);

  powerdown_line.attach_function(1, power_off);
  powerdown_line.attach_function(2, power_off);
  exhaust_line.attach_function(1, exhaust_on);
  exhaust_line.attach_function(2, exhaust_off);
  menu.add_screen(secondary_screen);
}

void TaskPrintToLCD(void *pvParameters)
{
  (void) pvParameters;

  pinMode(SELF_POWERDOWN, OUTPUT);
  digitalWrite(SELF_POWERDOWN, HIGH);


  setupLCD();
  configureStartupMenu();
  configureMenu();
  configurePoweringOffMenu();

  int last_lcd_key = 0;
  for(;;){
    if(last_temperature != current_temperature){
      last_temperature = current_temperature;
      menu_system.update();
    }

    if(last_lcd_key != lcd_key){
      last_lcd_key = lcd_key;

      switch (lcd_key){
           case btnRIGHT:{
               menu_system.next_screen();
                break;
           }
           case btnLEFT:{
                 menu_system.previous_screen();
                 break;
           }
           case btnUP:{
                 menu_system.call_function(1);
                 break;
           }
           case btnDOWN:{
                 menu_system.call_function(2);
                 break;
           }
           case btnSELECT:{
                 menu_system.switch_focus();
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
