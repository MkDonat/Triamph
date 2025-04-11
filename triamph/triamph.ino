//System
#define SYSTEM_BAUD_RATE 115200
#define CORE_1 0
#define CORE_2 1
//Communication (ESP-NOW protocole)
#include "ESP32_NOW.h"
#include "WiFi.h"
//Mode de conduite du triamph
enum Modes_de_Conduite {BOAT,CAR,};

void setup() {
  //Cpu frequency
  //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //Esp-Now setup
  setup_broadcast();
  //System State Machine
  setup_ssm();
}
void loop() {
  broadcast();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
