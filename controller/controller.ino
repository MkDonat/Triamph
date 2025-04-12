//CORES
#define CORE_1 0
#define CORE_2 1
//COMMUNICATIONS
#include "ESP32_NOW.h"
#include "WiFi.h"
//#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
//#include <vector>
#define SYSTEM_BAUD_RATE 115200

//triggers
byte left_trigger_pin = 39;
byte right_trigger_pin = 34;

void setup(){
  //triggers
  pinMode(left_trigger_pin,INPUT);
  pinMode(right_trigger_pin,INPUT);
  //setCpuFrequencyMhz(80);
  //communications
  Serial.begin(SYSTEM_BAUD_RATE);
  setup_broadcast();
  //Controller state machine
  setup_csm();
}

void loop(){
  broadcast();
  csm_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
}