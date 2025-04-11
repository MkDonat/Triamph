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
  //Cpu frequency
  //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //Esp-Now setup
  setup_broadcast();
  //motors
  setup_motors();
  //Creating FreeRTOS-Tasks
   //--nothing yet
  //System State Machine
  setup_ssm();
}
void loop() {
  broadcast();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
