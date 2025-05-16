#define CORE_1 0
#define CORE_2 1
#include <Adafruit_PCF8574.h>
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
uint8_t left_joystick_pin_x = 34;
uint8_t left_joystick_pin_y = 35;
//PCF8574 PINS (IO EXPANDER)
uint8_t pcf_P0 = 0;
uint8_t pcf_P1 = 1;
uint8_t pcf_P2 = 2;
uint8_t pcf_P3 = 3;
uint8_t pcf_P4 = 4;
uint8_t pcf_P5 = 5;
uint8_t pcf_P6 = 6;
uint8_t pcf_P7 = 7;
//Buttons pins
uint8_t B_button_pin;
uint8_t A_button_pin;
uint8_t X_button_pin;
uint8_t Y_button_pin;
uint8_t LEFT_button_pin;
uint8_t RIGHT_button_pin;
uint8_t UP_button_pin;
uint8_t DOWN_button_pin;
uint8_t SHARE_button_pin;
//Buttons
OneButton B_button;
OneButton A_button;
OneButton X_button;
OneButton Y_button;
OneButton LEFT_button;
OneButton RIGHT_button;
OneButton UP_button;
OneButton DOWN_button;
OneButton SHARE_button;
//PCF8574 OBJ
Adafruit_PCF8574 pcf;

void setup(){
  //setCpuFrequencyMhz(80);
  //communications
  Serial.begin(SYSTEM_BAUD_RATE);
  //triggers
  //pinMode(left_trigger_pin,INPUT);
  //pinMode(right_trigger_pin,INPUT);
  //left joystick
  pinMode(left_joystick_pin_x,INPUT);
  pinMode(left_joystick_pin_y,INPUT);
  //Configuring PCF8574 button mode
  if (!pcf.begin(0x38, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }
  for (uint8_t p=0; p<8; p++) {
    pcf.pinMode(p, INPUT_PULLUP);
  }
  //Button pin assign
  B_button_pin = 25;
  A_button_pin = pcf_P0;
  X_button_pin = pcf_P1;
  Y_button_pin = pcf_P2;
  LEFT_button_pin = pcf_P3;
  RIGHT_button_pin = pcf_P4;
  UP_button_pin = pcf_P5;
  DOWN_button_pin = pcf_P6;
  SHARE_button_pin = pcf_P7;
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
  //X->
  X_button.attachClick(onClick_X);
  X_button.attachLongPressStart(onLongPress_X);
  X_button.attachDuringLongPress(duringLongPress_X);
  X_button.setLongPressIntervalMs(1000);
  //Y->
  Y_button.attachClick(onClick_Y);
  Y_button.attachLongPressStart(onLongPress_Y);
  Y_button.attachDuringLongPress(duringLongPress_Y);
  Y_button.setLongPressIntervalMs(1000);
  //LEFT->
  LEFT_button.attachClick(onClick_LEFT);
  LEFT_button.attachLongPressStart(onLongPress_LEFT);
  LEFT_button.attachDuringLongPress(duringLongPress_LEFT);
  LEFT_button.setLongPressIntervalMs(1000);
  //RIGH->
  RIGHT_button.attachClick(onClick_RIGHT);
  RIGHT_button.attachLongPressStart(onLongPress_RIGHT);
  RIGHT_button.attachDuringLongPress(duringLongPress_RIGHT);
  RIGHT_button.setLongPressIntervalMs(1000);
  //UP->
  UP_button.attachClick(onClick_UP);
  UP_button.attachLongPressStart(onLongPress_UP);
  UP_button.attachDuringLongPress(duringLongPress_UP);
  UP_button.setLongPressIntervalMs(1000);
  //DOWN->
  DOWN_button.attachClick(onClick_DOWN);
  DOWN_button.attachLongPressStart(onLongPress_DOWN);
  DOWN_button.attachDuringLongPress(duringLongPress_DOWN);
  DOWN_button.setLongPressIntervalMs(1000);
  //SHARE->
  SHARE_button.attachClick(onClick_SHARE);
  SHARE_button.attachLongPressStart(onLongPress_SHARE);
  SHARE_button.attachDuringLongPress(duringLongPress_SHARE);
  SHARE_button.setLongPressIntervalMs(1000);
  //FREERTOS TASKS
    //CreateTasksForJoystick();
  setup_broadcast();
  //Controller state machine
  setup_csm();
}

void loop(){
  //Buttons tick
  B_button.tick(digitalRead(B_button_pin) == LOW);
  A_button.tick(pcf.digitalRead(A_button_pin) == LOW);
  X_button.tick(pcf.digitalRead(X_button_pin) == LOW);
  Y_button.tick(pcf.digitalRead(Y_button_pin) == LOW);
  LEFT_button.tick(pcf.digitalRead(LEFT_button_pin) == LOW);
  RIGHT_button.tick(pcf.digitalRead(RIGHT_button_pin) == LOW);
  UP_button.tick(pcf.digitalRead(UP_button_pin) == LOW);
  DOWN_button.tick(pcf.digitalRead(DOWN_button_pin) == LOW);
  SHARE_button.tick(pcf.digitalRead(SHARE_button_pin) == LOW);
  //NOW
  broadcast();
  //State machine tick
  csm_execute();
  //Handle buttons message
  if(B_button.isIdle()==true && A_button.isIdle()==true && X_button.isIdle()==true && Y_button.isIdle()==true && LEFT_button.isIdle()==true && RIGHT_button.isIdle()==true && UP_button.isIdle()==true && DOWN_button.isIdle()==true && SHARE_button.isIdle()==true){
    writting_button_message("");
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}