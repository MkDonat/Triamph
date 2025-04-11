//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
//#include <esp_now.h>
//#include <WiFi.h>
#include "ESP32_NOW.h"
#include "WiFi.h"
#define SYSTEM_BAUD_RATE 115200

void setup() {
  //Starting communications
  Serial.begin(SYSTEM_BAUD_RATE);
  setup_broadcast();
  setup_motors();
  //Creating FreeRTOS-Tasks
  CreateTasksForLedRed();
  //CreateTasksForStepper();
  CreateTasksForTofSensor();
  //System State Machine
  setup_ssm();
}
void loop() {
  broadcast();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
