#define CORE_1 0
#define CORE_2 1
#define BAUD_RATE 115200

void setup() {
  Serial.begin(BAUD_RATE);
  CreateTasksForLedOrange();
  CreateTasksForLedGreen();
  CreateTasksForLedRed();
  CreateTasksForPotentio();
  CreateTasksForStepper();
  CreateTasksForServo();
}

void loop() {
  // nothing to do here
} 
