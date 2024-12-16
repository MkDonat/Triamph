//COMMUNICATIONS
#include <esp_now.h>
#include <WiFi.h>
#define BAUD_RATE 115200
//CORES
#define CORE_1 0
#define CORE_2 1
//SCREEN
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

void setup(){
  Serial.begin(BAUD_RATE);
  setup_now();
  setup_gaz();
  setup_joystick();
  screen_setup();
  //setup_InfoLed();
  //FreeRTOS Tasks
  CreateTasksForGaz();
}

void loop(){
  // nothing to do here
  loop_now();
  loop_screen();
  //loop_InfoLed();
  loop_joystick();
  //delay(10);
}