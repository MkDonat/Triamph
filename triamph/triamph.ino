//System
#define SYSTEM_BAUD_RATE 115200
#define CORE_1 0
#define CORE_2 1
//Communication (ESP-NOW protocole)
#include "ESP32_NOW.h"
#include "WiFi.h"
//Servo motors
#include "ESP32Servo.h"
//Mode de conduite du triamph
enum Modes_de_Conduite {BOAT,CAR,};
//Etat initial boutons
bool is_button_clicked_LU = false;
bool is_button_clicked_OC_CLAMPS = false;
//Servos
uint16_t servo_error = 5; //degrés
//Servo Objects LU
uint16_t position_base_LU = 70;
Servo servo_LU_droit;
Servo servo_LU_gauche;
//Servo Objects OC_CLAMPS
Servo servo_OC_CLAMPS_droit;
Servo servo_OC_CLAMPS_gauche;
//System bool
bool is_on_water = false;

void setup() {
  //Motors
  setup_motors();
  //Cpu frequency
    //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //Esp-Now setup
  setup_broadcast();
  //Attach servos LU -> Load Unload
  servo_LU_droit.attach(15);
  servo_LU_gauche.attach(2);
  servo_LU_droit.write(position_base_LU);
  servo_LU_gauche.write(position_base_LU);
  //Attache servos OC_CLAMPS -> Open Close CLAMPS
  servo_OC_CLAMPS_droit.attach(16);
  servo_OC_CLAMPS_gauche.attach(22);
  //System State Machine
  setup_ssm();
}
void loop() {
  broadcast();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
