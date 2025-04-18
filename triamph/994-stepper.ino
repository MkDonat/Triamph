#include <Stepper.h>

const int stepsPerRevolution = 1000;  // change this to fit the number of steps per revolution for your motor
const uint32_t rotation_delay = 1000;

// initialize the stepper library on pins x1 through x4:
Stepper myStepper(stepsPerRevolution, 5, 19, 18, 21); //IN1, IN3, IN2, IN4

void stepper_one_revolution(void *arg){
 // set the speed at 60 rpm:
  myStepper.setSpeed(30);
  // initialize the serial port:
  while(1){
    // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(rotation_delay);
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(rotation_delay);
  }
}
void CreateTasksForStepper(){
  xTaskCreatePinnedToCore(
    stepper_one_revolution,"Rotation du stepper dans un sens puis dans l autre", 2048
    ,
    NULL // Stack Depth
    ,
    2 //Priority
    ,
    NULL //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}