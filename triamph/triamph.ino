//System
#define SYSTEM_BAUD_RATE 115200
#include "FS.h"
#include <LittleFS.h>
//ArduinoJSON
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
//#define TWOPART
#define FORMAT_LITTLEFS_IF_FAILED true
#define CORE_1 0
#define CORE_2 1
//Communication (ESP-NOW protocole)
#include "ESP32_NOW.h"
#include "WiFi.h"
//Servo motors
#include "ESP32Servo.h"

//File system datas
JsonDocument JSONdatas;
char serializedJSONdatas[] = "{\"sg90_last_consigne_pose\":0,\"ds3218_last_consigne_pose\":0,\"gps_data\":[48.756080,2.302038]}";;
const char *JSONdatas_file_path = "/db.txt";
//Servos
Servo sg90_droit;
Servo sg90_gauche;
uint16_t sg90_last_consigne_pose = 0;
uint8_t sg90_defaut_pose = 1;
Servo ds3218_droit;
Servo ds3218_gauche;
uint16_t ds3218_last_consigne_pose = 0;
uint8_t ds3218_defaut_pose = 1;
uint16_t servo_error = 5; //degrés
//only pins 2,4,12-19,21-23,25-27,32-33 are recommended for servos on ESP32
uint8_t sg90_droit_pin = 4;
uint8_t sg90_gauche_pin = 23;
uint8_t ds3218_droit_pin = 15;
uint8_t ds3218_gauche_pin = 2;
//System bool
bool is_OC_Clamps_task_complete = false;
bool is_LoadUnload_task_complete = false;
bool is_on_water = false;
//Définition des Fonctions
void setup_ssm();
void system_state_machine_execute();

void setup(){
  //Moniteur d’exception Expressif
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  //Cpu frequency
    //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //File system begin
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LittleFS Mount Failed");
    //return;
  }
  //updating varriables from flash
  getting_char_datas_from_flash();
  deserialize_JSONdatas();
  update_vars_from_JSONdatas();
  //Motors
  setup_motors();
  //Verin
  setup_verin();
  //Servos-attach
  ds3218_droit.attach(ds3218_droit_pin);
  ds3218_gauche.attach(ds3218_gauche_pin);
  sg90_droit.attach(sg90_droit_pin);
  sg90_gauche.attach(sg90_gauche_pin);
  //Servos-set defaut poses
  ds3218_droit.write(ds3218_last_consigne_pose);
  ds3218_gauche.write(ds3218_last_consigne_pose);
  sg90_droit.write(sg90_last_consigne_pose);
  sg90_gauche.write(sg90_last_consigne_pose);
  //let servos get to the pose
  delay(100);
  //Servos-dettach
  ds3218_droit.detach();
  ds3218_gauche.detach();
  sg90_droit.detach();
  sg90_gauche.detach();
  //System State Machine
  setup_ssm();
  //Esp-Now setup
  setup_broadcast();
  //Serial.println(serializedJSONdatas);
}

void loop(){
  broadcast();//Esp-now communication
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
  //Serial.printf("sg90:%d , ds3218:%d\n",sg90_last_consigne_pose,ds3218_last_consigne_pose);
}
