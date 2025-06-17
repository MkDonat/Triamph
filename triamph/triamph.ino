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
char serializedJSONdatas[] = "{\"sg90_left_last_consigne\":1,\"sg90_right_last_consigne\":90,\"ds3218_left_last_consigne\":1,\"ds3218_right_last_consigne\":1,\"gps_data\":[48.75608,2.302038]}";
const char *JSONdatas_file_path = "/db.txt";

//System enum
enum TRIAMPH_MILIEU{
  ON_WATER,
  ON_LAND
};

//Servos (general)
enum servo_states{
  CLOSED,
  OPENED
};

uint16_t servo_error = 5; //degrés

//Servos sg90 (clamps)
servo_states sg90_left_clamp_current_state = CLOSED;
servo_states sg90_right_clamp_current_state = OPENED;
uint8_t sg90_droit_pin = 4;//only pins 2,4,12-19,21-23,25-27,32-33 are recommended for servos on ESP32
uint8_t sg90_gauche_pin = 23;
Servo sg90_droit;
Servo sg90_gauche;
uint16_t sg90_left_last_consigne = 0;
uint16_t sg90_right_last_consigne = 0;
const uint8_t sg90_left_start_pose = 90;
const uint8_t sg90_left_end_pose = 179;
const uint8_t sg90_right_start_pose = 1;
const uint8_t sg90_right_end_pose = 90;
const uint16_t sg90_step = 1;
uint16_t sg90_roll_speed = 100; //0-100
uint16_t sg90_rollback_speed = 100; //0-100
bool sg90_reverse_kinematic = true;
bool is_OC_Clamps_task_complete = false;
const uint16_t OC_Time = 5000;

//Servos ds3218 (Levage système or godet)
servo_states ds3218_left_clamp_current_state = CLOSED;
servo_states ds3218_right_clamp_current_state = OPENED;
uint8_t ds3218_droit_pin = 15;//only pins 2,4,12-19,21-23,25-27,32-33 are recommended for servos on ESP32
uint8_t ds3218_gauche_pin = 2;
Servo ds3218_droit;
Servo ds3218_gauche;
uint16_t ds3218_left_last_consigne = 0;
uint16_t ds3218_right_last_consigne = 0;
const uint8_t ds3218_start_pose = 179;
const uint8_t ds3218_end_pose = 1;
const uint16_t ds3218_step = 1;
uint16_t ds3218_roll_speed = 20; //0-100
uint16_t ds3218_rollback_speed = 20; //0-100
bool ds3218_reverse_kinematic = true;
bool is_LoadUnload_task_complete = false;
const uint16_t LoadUnload_Time = 15000;

// === Water sensor ===
const int sensorPin = 34; // Analog-capable pin on ESP32
int sensorValue = 0;
bool sensorPresent = true;
const int water_threshold = 1500;
const int max_adc_value = 4095;
bool waterSensorError = false;

//FreeRTOS
TimerHandle_t xTask_OC_TimerHandler = NULL;
TaskHandle_t xTask_OC_right_Clamp_Handle = NULL;
TaskHandle_t xTask_OC_left_Clamp_Handle = NULL;

TaskHandle_t xTask_LoadUnload_RightServo_Handle = NULL;
TaskHandle_t xTask_LoadUnload_LeftServo_Handle = NULL;
TimerHandle_t xTask_LoadUnload_TimerHandler = NULL;

//functions
void setup_ssm();
void system_state_machine_execute();
void xTask_OC_TimerCallback(TimerHandle_t xTimer);
void xTask_LoadUnload_TimerCallback(TimerHandle_t xTimer);

void setup(){
  //Moniteur d’exception Expressif
    //->esp_log_level_set("*", ESP_LOG_VERBOSE);
  //Cpu frequency
    //->setCpuFrequencyMhz(80);
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
  //Servos-set latest known poses
  ds3218_droit.write(ds3218_right_last_consigne);
  ds3218_gauche.write(ds3218_left_last_consigne);
  sg90_droit.write(sg90_right_last_consigne);
  sg90_gauche.write(sg90_left_last_consigne);
  //let servos get to the pose
  delay(100);
  //Servos-dettach
  //ds3218_droit.detach();
  //ds3218_gauche.detach();
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
  operating_water_sensor();
}
