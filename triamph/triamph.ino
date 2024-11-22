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
  //Tasks
  //CreateTasksForLedRed();
  //CreateTasksForStepper();
  //CreateTasksForServo();
  CreateTasksForMotors();
}
void loop() {
  loop_now();
  delay(10);
} 
