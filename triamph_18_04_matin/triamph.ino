//System
#define SYSTEM_BAUD_RATE 115200
#define CORE_1 0
#define CORE_2 1
//Communication (ESP-NOW protocole)
#include "ESP32_NOW.h"
#include "WiFi.h"
#include "ESP32Servo.h"
#include "OneButton.h"
//Mode de conduite du triamph
enum Modes_de_Conduite {BOAT,CAR,};
// Etat initial boutons
bool is_button_clicked_LU = false;
bool is_button_clicked_OC_CLAMPS = false;
// Servo Objects LU
Servo servo_LU_droit;
Servo servo_LU_gauche;
// Servo Objects OC_CLAMPS
Servo servo_OC_CLAMPS_droit;
Servo servo_OC_CLAMPS_gauche;
// Boutons
//OneButton button_LU;
//OneButton_OC_CLAMPS;
//byte LU_button_pin = 13;
//byte OC_CLAMPS_button_pin = 27;

void setup() {
  //Motors
  setup_motors();
  //Cpu frequency
  //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //Esp-Now setup
  setup_broadcast();
  // Attach servos LU
  servo_LU_droit.attach(15);
  servo_LU_gauche.attach(2);
  // Attache servos OC_CLAMPS
  servo_OC_CLAMPS_droit.attach(16);
  servo_OC_CLAMPS_gauche.attach(22);
  //Button
  //button_LU.setup(LU_button_pin,INPUT_PULLUP,true);
  //button_OC_CLAMPS.setup(OC_CLAMPS_button_pin,INPUT_PULLUP,true);
  //button_LU.attachClick(onClickCallBack_LU);
  //button_OC_CLAMPS.attachClick(onClickCallBack_OC_CLAMPS);
  int position_base_LU = 70;
  servo_LU_droit.write(position_base_LU);
  servo_LU_gauche.write(position_base_LU);
  //System State Machine
  setup_ssm();
}
void loop() {
  broadcast();
  //button_LU.tick();
  //button_OC_CLAMPS.tick();
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
} 
