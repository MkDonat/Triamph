//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
#include "ESP32_NOW.h"
#include "WiFi.h"
  //#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
  //#include <vector>
//Buttons
#include <OneButton.h>
#define SYSTEM_BAUD_RATE 115200

//triggers
byte left_trigger_pin = 39;
byte right_trigger_pin = 34;
//left joystick
byte left_joystick_pin_x = 32;
byte left_joystick_pin_y = 35;
//B Button
OneButton B_button; //pin 36

void setup(){
  //setCpuFrequencyMhz(80);
  //communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //triggers
  pinMode(left_trigger_pin,INPUT);
  pinMode(right_trigger_pin,INPUT);
  //left joystick
  pinMode(left_joystick_pin_x,INPUT);
  pinMode(left_joystick_pin_y,INPUT);
  //Buttons
  B_button.setup(36, INPUT_PULLUP, true);
  B_button.attachClick(onClick_B_button);
  B_button.attachLongPressStart(onLongPressStart_B_button);
  //FREERTOS TASKS
    //CreateTasksForJoystick();
  setup_broadcast();
  //Controller state machine
  setup_csm();
}

void loop(){
  broadcast();
  csm_execute();
  B_button.tick();
  vTaskDelay(pdMS_TO_TICKS(10));
}