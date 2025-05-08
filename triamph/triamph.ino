//System
#define SYSTEM_BAUD_RATE 115200
#define CORE_1 0
#define CORE_2 1
//Communication (ESP-NOW protocole)
#include "ESP32_NOW.h"
#include "WiFi.h"
//Servo motors
#include "ESP32Servo.h"

//Servos
Servo sg90_droit;
Servo sg90_gauche;
uint8_t sg90_defaut_pose = 1;
Servo ds3218_droit;
Servo ds3218_gauche;
uint8_t ds3218_defaut_pose = 179;
uint16_t servo_error = 5; //degrés
//System bool
bool is_OC_Clamps_task_complete = false;
bool is_on_water = false;

void setup_ssm();
void system_state_machine_execute();

void setup() {
  //Cpu frequency
    //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //Motors
  setup_motors();
  //Servos-attach
  ds3218_droit.attach(15);
  ds3218_gauche.attach(2);
  sg90_droit.attach(16);
  sg90_gauche.attach(22);
  //Servos-set defaut poses
  ds3218_droit.write(ds3218_defaut_pose);
  ds3218_gauche.write(ds3218_defaut_pose);
  sg90_droit.write(sg90_defaut_pose);
  sg90_gauche.write(sg90_defaut_pose);
  //System State Machine
  setup_ssm();
  //Esp-Now setup
  setup_broadcast();
}
void loop() {
  broadcast();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
