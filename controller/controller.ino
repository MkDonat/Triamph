//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
//#include <esp_now.h>
//#include <WiFi.h>
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
#include <vector>
#define SYSTEM_BAUD_RATE 115200

void setup(){
  //Starting communications
  Serial.begin(SYSTEM_BAUD_RATE);
  setup_broadcast();
  //Creating FreeRTOS-Tasks
  CreateTasksForJoystick();
  CreateTasksForGaz();
  CreateTasksForScreen();
}

void loop(){
  loop_broadcast();
  vTaskDelay(pdMS_TO_TICKS(10));
}