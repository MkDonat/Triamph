#define CORE_1 0
#define CORE_2 1
#include "ESP32_NOW.h"
#include "WiFi.h"
//#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
//#include <vector>
#include <OneButton.h>
#define SYSTEM_BAUD_RATE 115200
//screen libs
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

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

//triggers
uint8_t left_trigger_pin = 33;
uint8_t right_trigger_pin = 32;

//left joystick
uint8_t left_joystick_pin_x = 35;
uint8_t left_joystick_pin_y = 34;

//Buttons pins
uint8_t B_button_pin = 13;
uint8_t A_button_pin = 14;

//Buttons
OneButton B_button;
OneButton A_button;

//-----FreeRTOS-----
TaskHandle_t xTask_left_joystick_Handle = NULL;


void setup(){
  setCpuFrequencyMhz(80);
  //communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //left joystick
  pinMode(left_joystick_pin_x, INPUT);
  pinMode(left_joystick_pin_y, INPUT);
  //Buttons B PIN SETUP
  pinMode(B_button_pin, INPUT_PULLUP);
  pinMode(A_button_pin, INPUT_PULLUP);
  //Buttons Callbacks
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
  //FREERTOS TASKS
    //CreateTasksForJoystick();
  setup_broadcast();
  //Controller state machine
  setup_csm();
}

void loop(){
  //Buttons tick
  B_button.tick(digitalRead(B_button_pin) == LOW);
  A_button.tick(digitalRead(A_button_pin) == LOW);
  //ESP-NOW
  broadcast();
  //State machine
  csm_execute();
  //Handle buttons message
  if(B_button.isIdle() == true && A_button.isIdle() == true){
    writting_button_message("");
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}