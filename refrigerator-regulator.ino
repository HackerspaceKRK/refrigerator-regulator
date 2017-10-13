#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SleepyDog.h>
#include "config.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

enum states {
  IDLING, COOLING
};

int state = IDLING;
int state_change_time = 0;
float current_temperature = 0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("# Refrigerator Regulator!");
  Serial.println();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Watchdog.enable(interval * 3);
  
  sensors.begin();

  print_csv_header();
}


void go_cooling(){
  Serial.println("# REFRIGERATOR IS GOING ON!");
  state = COOLING;
  digitalWrite(RELAY_PIN, HIGH);
  state_change_time = millis();
}

void go_idling(){
  Serial.println("# REFRIGERATOR IS GOING OFF!");
  state = IDLING;
  digitalWrite(RELAY_PIN, LOW);
  state_change_time = millis();
}


void print_csv_header(){
  Serial.println("time_from_start,time_in_current_state,temperature,state");
}

void print_status_as_csv(){
  Serial.print(millis() / 1000);
  Serial.print(',');
  Serial.print((millis() - state_change_time) / 1000);
  Serial.print(',');
  Serial.print(current_temperature);
  Serial.print(',');
  if(state == IDLING){
    Serial.println("0");
  }else if(state == COOLING){
    Serial.println("1");
  }
}

void print_status_as_text(){
  Serial.print("# Time from start: ");
  Serial.println(millis() / 1000);

  Serial.print("# Time in current state: ");
  Serial.println((millis() - state_change_time) / 1000);
  
  Serial.print("# Current temperature: ");
  Serial.println(current_temperature);

  if (state == IDLING){
    Serial.println("# Current state: IDLING");
  } else if (state == COOLING) {
    Serial.println("# Current state: COOLING");
  }
  Serial.println();
}

void loop(void)
{ 
  Watchdog.reset();

  sensors.requestTemperatures();
  current_temperature = sensors.getTempCByIndex(0);

  print_status_as_csv();
  print_status_as_text();
  
  if (state == IDLING){
    if (current_temperature > temperature_high_threshold) {
      go_cooling();
    }
    
  } else if (state == COOLING) {
    if (current_temperature < temperature_low_threshold) {
      go_idling();
    }
  }
  
  delay(interval);
}
