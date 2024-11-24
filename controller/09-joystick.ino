byte joystick_x_pin = 34; // input only + ADC pin
byte joystick_y_pin = 36; // input only + ADC pin
byte joystick_b_pin = 27; //Joystick BUTTON
int16_t joystick_x_raw_value;
const int16_t joystick_x_err = 600;
int16_t joystick_y_raw_value;
const int16_t joystick_y_err = 600;
const int16_t max_analog_lecture = 4095; // 12-bits
int16_t j_x_offset = 125; // vieillissement du joystick
int16_t j_y_offset = 104;

void setup_joystick(){
  pinMode(joystick_x_pin, INPUT);
  pinMode(joystick_y_pin, INPUT);
  pinMode(joystick_b_pin, INPUT_PULLUP);
}

void monitoring_joystick_datas(){
  joystick_x_raw_value = analogRead(joystick_x_pin);
  if(joystick_x_raw_value > max_analog_lecture/2 - j_x_offset + joystick_x_err){
    data.joystick_x_value = 1;
  }
  else if(joystick_x_raw_value < max_analog_lecture/2 - j_x_offset - joystick_x_err){
    data.joystick_x_value = -1;
  }
  else{
    data.joystick_x_value = 0;
  }
  joystick_y_raw_value = analogRead(joystick_y_pin);
  if(joystick_y_raw_value > max_analog_lecture/2 - j_y_offset + joystick_y_err){
    data.joystick_y_value = 1;
  }
  else if(joystick_y_raw_value < max_analog_lecture/2 - j_y_offset - joystick_y_err){
    data.joystick_y_value = -1;
  }
  else{
    data.joystick_y_value = 0;
  }
  /*Serial.printf(
    "raw_x: %ld , x: %ld , raw_y: %ld , y: %ld\n"
    ,joystick_x_raw_value
    ,data.joystick_x_value
    ,joystick_y_raw_value
    ,data.joystick_y_value
  );*/
}