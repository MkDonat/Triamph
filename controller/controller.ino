#define CORE_1 0
#define CORE_2 1
#include "ESP32_NOW.h"
#include "WiFi.h"
//#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
//#include <vector>
#include <OneButton.h>
#define SYSTEM_BAUD_RATE 115200
//screen libs
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//INA219
#ifndef WIRE_H
#ifndef ARDAFRUIT_INA219_H
#include <Wire.h>
#include <Adafruit_INA219.h>
#endif
#endif

//INA219
Adafruit_INA219 ina219;
float battery_level_percentage = 0.0;

//SPI setup pins (SCREEN)
U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI u8g2(
  U8G2_R0
  ,  
  23 // SLAVE CLOCK (SCK) or (SCL)
  ,
  18 // SLAVE DATA (SDA)
  ,
  5 // CHEAP SELECT (CS)
  ,  
  2 // REGISTER SELECT (DC)
  ,
  4 // RESET (RES)
);

//left joystick
uint8_t left_joystick_pin_x = 35;
uint8_t left_joystick_pin_y = 34;

//Buttons
OneButton B_button;
OneButton A_button;
OneButton L3_button;
uint8_t B_button_pin = 13;
uint8_t A_button_pin = 14;
uint8_t L3_button_pin = 26;


void setup(){
  //we setup a lower frequency to minimize battery consumption
  setCpuFrequencyMhz(80); // 240Mhz is the maximum
  //serial communication
  Serial.begin(SYSTEM_BAUD_RATE);
  //INA219 setup
  ina219_setup();
  //left joystick pin setup
  pinMode(left_joystick_pin_x, INPUT);
  pinMode(left_joystick_pin_y, INPUT);
  //Buttons pin setup
  pinMode(B_button_pin, INPUT_PULLUP);
  pinMode(A_button_pin, INPUT_PULLUP);
  pinMode(L3_button_pin, INPUT_PULLUP);
  //Buttons Callbacks setup
  //B->
  B_button.attachClick(onClick_B);
  B_button.attachLongPressStart(onLongPress_B);
  B_button.attachDuringLongPress(duringLongPress_B);
  B_button.setLongPressIntervalMs(1000);
  //A->
  A_button.attachClick(onClick_A);
  A_button.attachLongPressStart(onLongPress_A);
  A_button.attachDuringLongPress(duringLongPress_A);
  A_button.setLongPressIntervalMs(1000);
  //L3->
  L3_button.attachClick(onClick_L3);
  L3_button.attachLongPressStart(onLongPress_L3);
  L3_button.attachDuringLongPress(duringLongPress_L3);
  L3_button.setLongPressIntervalMs(1000);
  //FreeRTOS task creation
  create_task_for_ina219();
  CreateTasksForLeftJoystick();
  CreateTasksForScreen();
  //Esp Now communication setup
  setup_broadcast();
}

void loop(){
  //Buttons tick
  B_button.tick(digitalRead(B_button_pin) == LOW);
  A_button.tick(digitalRead(A_button_pin) == LOW);
  L3_button.tick(digitalRead(L3_button_pin) == LOW);
  //ESP-NOW
  broadcast();
  //Handle buttons message
  if(B_button.isIdle() == true && A_button.isIdle() == true && L3_button.isIdle() == true){
    writting_button_message("");
  }
  //wait
  vTaskDelay(pdMS_TO_TICKS(10));
}