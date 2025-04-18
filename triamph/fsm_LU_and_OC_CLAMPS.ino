#include <OneButton.h>
#include <ESP32Servo.h>

#define SYSTEM_BAUD_RATE 115200
#define CORE_1 0
#define CORE_2 1

//led
bool is_button_clicked_LU = false;
bool is_button_clicked_OC_CLAMPS = false;

// Servo objects LU
Servo servodroite;
Servo servogauche;
// Servo objects OC_CLAMPS
Servo servopincedroite;
Servo servopincegauche;

// Pins
OneButton button_LU;
OneButton button_OC_CLAMPS;
byte trash_load_button_pin = 13;
byte clamps_button_pin = 27;

void setup() {
  Serial.begin(SYSTEM_BAUD_RATE);

  // Attach servos LU
  servodroite.attach(15);
  servogauche.attach(2);
  // Attach servos OC_CLAMPS
  servopincedroite.attach(16);
  servopincegauche.attach(22);

  // Button
  button_LU.setup(trash_load_button_pin,INPUT_PULLUP,true);
  button_OC_CLAMPS.setup(clamps_button_pin,INPUT_PULLUP,true);
  button_LU.attachClick(onClickCallBack_LU);
  button_OC_CLAMPS.attachClick(onClickCallBack_OC_CLAMPS);
  int position_base_LU = 70;
  servodroite.write(position_base_LU);
  servogauche.write(position_base_LU);

  // FSM setup
  setup_fsm();
}

void loop() {
  fsm_execute();
  button_LU.tick();
  button_OC_CLAMPS.tick();
}