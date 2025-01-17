//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
#include <esp_now.h>
#include <WiFi.h>
#define BAUD_RATE 115200

void setup(){
  //Starting communications
  Serial.begin(BAUD_RATE);
  setup_now();
  //Creating FreeRTOS-Tasks
  CreateTasksForJoystick();
  CreateTasksForGaz();
  CreateTasksForScreen();
}

void loop(){
  loop_now();
  vTaskDelay(10 / portTICK_PERIOD_MS);
}