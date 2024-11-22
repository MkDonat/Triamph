#include <ESP32Servo.h>
//#include <analogWrite.h>
//#include <tone.h>
//#include <ESP32Tone.h>
//#include <ESP32PWM.h>

Servo myServo;
//servo.attach(pin, min, max); //optional

void servo_function(void *arg){
  myServo.attach(4);
  while(1){
    myServo.write(3000);
    delay(50);
  }
}

void CreateTasksForServo(){
  xTaskCreatePinnedToCore(
    servo_function,"Tâche du servo", 2048
    ,
    NULL // Stack Depth
    ,
    5 //Priority
    ,
    NULL //Task handle
    ,
    CORE_1 // Core on which the task will run
  );
}