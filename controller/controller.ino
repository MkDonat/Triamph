#define CORE_1 0
#define CORE_2 1
#include "ESP32_NOW.h"
#include "WiFi.h"
//#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
//#include <vector>
#include <OneButton.h>
#define SYSTEM_BAUD_RATE 115200
#define TIMEOUT 10
//screen libs
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//INA219
#ifndef WIRE_H
#ifndef ARDAFRUIT_INA219_H
#include <Wire.h>
#include <Adafruit_INA219.h>
// unordered map (to store gamepad datas)
#ifndef UNORDERED_MAP
#include <unordered_map>
// Arduino JSON
#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif
#endif
#endif
#endif

//SendingDatas
typedef struct { //250 Bytes MAX
  //Buttons
  char button_msg[16] = "";
  // Gamepad
  int8_t L_J_tor_x = 0;
  int8_t L_J_tor_y = 0;
  int16_t L_J_analog_x = 0;
  int16_t L_J_analog_y = 0;
} SendingPacketData;

SendingPacketData SendingData;

//ReceivedDatas
typedef struct { //250 Bytes MAX
  bool is_on_water = false;
  uint16_t speed_mps = 0;
  char active_state_name[12];
} ReceivedPacketData;

ReceivedPacketData receivedData;
// Gamepad datas stuffs
bool gamepadMode = false;
std::unordered_map<std::string, int> gamepad_datas;

int getUmapValue(const std::unordered_map<std::string, int>& m,
                      const std::string& key,
                      int defaultValue = 0)
{
    auto it = m.find(key);
    return (it != m.end()) ? it->second : defaultValue;
}

//INA219
Adafruit_INA219 ina219;
float battery_level_percentage = 0.0;

//SPI setup pins (SCREEN)
U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI u8g2(
  U8G2_R0
  ,  
  23 // SLAVE CLOCK (SCK) or (SCL)
  ,
  18 // SLAVE DATA (SDA)
  ,
  5 // CHEAP SELECT (CS)
  ,  
  2 // REGISTER SELECT (DC)
  ,
  4 // RESET (RES)
);

//left joystick
uint8_t left_joystick_pin_x = 35;
uint8_t left_joystick_pin_y = 34;

// Triamph controller Buttons
OneButton B_button;
OneButton A_button;
OneButton L3_button;
uint8_t B_button_pin = 13;
uint8_t A_button_pin = 14;
uint8_t L3_button_pin = 26;
// Gamepad button
OneButton Share_Btn;

void setup(){
  //we setup a lower frequency to minimize battery consumption
  setCpuFrequencyMhz(80); // 240Mhz is the maximum
  //serial communication
  Serial.begin(SYSTEM_BAUD_RATE);
  Serial.setTimeout(TIMEOUT);
  //INA219 setup
  ina219_setup();
  //left joystick pin setup
  pinMode(left_joystick_pin_x, INPUT);
  pinMode(left_joystick_pin_y, INPUT);
  //Buttons pin setup
  pinMode(B_button_pin, INPUT_PULLUP);
  pinMode(A_button_pin, INPUT_PULLUP);
  pinMode(L3_button_pin, INPUT_PULLUP);
  //Buttons Callbacks setup
  //B->
  B_button.attachClick(onClick_B);
  B_button.attachLongPressStart(onLongPress_B);
  B_button.attachDuringLongPress(duringLongPress_B);
  B_button.setLongPressIntervalMs(1000);
  //A->
  A_button.attachClick(onClick_A);
  A_button.attachLongPressStart(onLongPress_A);
  A_button.attachDuringLongPress(duringLongPress_A);
  A_button.setLongPressIntervalMs(1000);
  //L3->
  L3_button.attachClick(onClick_L3);
  L3_button.attachLongPressStart(onLongPress_L3);
  L3_button.attachDuringLongPress(duringLongPress_L3);
  L3_button.setLongPressIntervalMs(1000);
  //Share_Btn->
  Share_Btn.attachClick(onClick_ShareBtn);
  //FreeRTOS task creation
  create_task_for_ina219();
  CreateTasksForLeftJoystick();
  CreateTasksForScreen();
  if(
    createTaskForSerialJSONReading()
  ){
    Serial.println("Creating TaskForSerialJSONReading SUCCESS!");
  }else{
    Serial.println("Creating TaskForSerialJSONReading FAILED.");
  }
  //Esp Now communication setup
  setup_broadcast();
}

void loop(){
  //Buttons tick && Gamepad buttons
  B_button.tick(
    digitalRead(B_button_pin) == LOW ||
    getUmapValue(gamepad_datas, "x", 0) == 1 && gamepadMode == true
  );
  A_button.tick(
    digitalRead(A_button_pin) == LOW ||
    getUmapValue(gamepad_datas, "a", 0) == 1 && gamepadMode == true
  );
  L3_button.tick(
    digitalRead(L3_button_pin) == LOW ||
    getUmapValue(gamepad_datas, "leftstick", 0) == 1
  );
  Share_Btn.tick(
    getUmapValue(gamepad_datas, "misc1", 0) == 1
  );
  //ESP-NOW
  broadcast();
  //Handle buttons message
  if(B_button.isIdle() == true && A_button.isIdle() == true && L3_button.isIdle() == true){
    writting_button_message("");
  }
  //wait
  vTaskDelay(pdMS_TO_TICKS(10));
}