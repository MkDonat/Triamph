//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
#include <esp_now.h>
#include <WiFi.h>
#define SYSTEM_BAUD_RATE 115200

void setup() {
  //Starting communications
  Serial.begin(SYSTEM_BAUD_RATE);
  setup_now_sender();
  setup_now_receiver();
  setup_motors();
  //Creating FreeRTOS-Tasks
  CreateTasksForLedRed();
  CreateTasksForStepper();
  CreateTasksForTofSensor();
  //vTaskStartScheduler();
}
void loop() {
  loop_now_sender();
  loop_now_receiver();
  loop_motors();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
