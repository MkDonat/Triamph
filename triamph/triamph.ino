#define CORE_1 0
#define CORE_2 1
#include <esp_now.h>
#include <WiFi.h>
#define BAUD_RATE 115200

//functions
void setup() {
  //Serial monitor
  Serial.begin(BAUD_RATE);
  setup_now();
  setup_motors();
  //Tasks
  //CreateTasksForLedRed();
  //CreateTasksForStepper();
  //CreateTasksForServo();
}
void loop() {
  loop_now();
  loop_motors();
  delay(10);
} 
