#define CORE_1 0
#define CORE_2 1
#include <esp_now.h>
#include <WiFi.h>
#define SYSTEM_BAUD_RATE 115200

void setup() {
  Serial.begin(SYSTEM_BAUD_RATE);
  setup_now();
  setup_motors();
  CreateTasksForLedRed();
  CreateTasksForStepper();
}
void loop() {
  loop_now();
  loop_motors();
  delay(10);
} 
