#include <esp_now.h>
#include <WiFi.h>
#define CORE_1 0
#define CORE_2 1
#define BAUD_RATE 115200

void setup(){
  Serial.begin(BAUD_RATE);
  setup_now();
  setup_gaz();
  setup_joystick();
  setup_InfoLed();
  CreateTasksForGaz();
}

void loop(){
  // nothing to do here
  loop_now();
  //loop_InfoLed();
  delay(10);
}