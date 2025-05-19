//System
#define SYSTEM_BAUD_RATE 115200
#include "FS.h"
#include <LittleFS.h>
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
String serialized_servos_poses = "500";
char* servos_poses_data_path = "/servos_poses.txt";
String serialized_gps_home_coordinates = "";
//Servos
Servo sg90_droit;
Servo sg90_gauche;
uint16_t sg90_last_consigne_pose;
uint8_t sg90_defaut_pose = 1;
Servo ds3218_droit;
Servo ds3218_gauche;
uint16_t ds3218_last_consigne_pose;
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

void setup() {
  //Moniteur d’exception Expressif
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  //Cpu frequency
    //setCpuFrequencyMhz(80);
  //Serial Communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //File system begin
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
    //return;
  }
  //Getting datas from flash
  serialized_servos_poses = readStringFromFile(LittleFS, servos_poses_data_path);
  //deleteFile(LittleFS, servos_poses_data_path);
  //Motors
  setup_motors();
  //Verin
  setup_verin();
  //Servos-attach
  ds3218_droit.attach(15);
  ds3218_gauche.attach(2);
  sg90_droit.attach(16);
  sg90_gauche.attach(5);
  //Servos-set defaut poses
  ds3218_droit.write(ds3218_defaut_pose);
  ds3218_gauche.write(ds3218_defaut_pose);
  sg90_droit.write(sg90_defaut_pose);
  sg90_gauche.write(sg90_defaut_pose);
  //let servos get to pose
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
}
void loop() {
  broadcast();//Esp-now communication
  system_state_machine_execute();
  vTaskDelay(pdMS_TO_TICKS(10));
  Serial.println(serialized_servos_poses);
}
