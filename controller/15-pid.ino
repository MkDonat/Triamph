#include <PID_v1.h>

#define PIN_INPUT 0
#define PIN_OUTPUT 13

//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp=5, Ki=1, Kd=1.5;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void vTaskpid_test(void *arg){
  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 100;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  for(;;){
    Input = analogRead(PIN_INPUT);
    myPID.Compute();
    analogWrite(PIN_OUTPUT, Output);
  }
}

void CreateTaskForPID(){
  xTaskCreatePinnedToCore(
    vTaskpid_test, "PID computations tests"
    ,
     2048 // Stack Depth
    ,
    NULL // arg
    ,
    1 //Priority
    ,
    NULL //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}
