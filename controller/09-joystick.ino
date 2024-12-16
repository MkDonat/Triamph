byte joystick_x_pin = 34; // input only + ADC_1 pin
byte joystick_y_pin = 39; // input only + ADC_1 pin (VN)
int16_t joystick_x_raw_datas; //données brutes
int16_t joystick_y_raw_datas; //données brutes
int16_t joystick_x_fixed_datas; //données corrigés
int16_t joystick_y_fixed_datas; //données corrigés
const int16_t joystick_x_err = 600;
const int16_t joystick_y_err = 600;
const int16_t max_analog_lecture = 4095; // 12-bits
int16_t j_x_offset = 230; // décallage du au vieillissement
int16_t j_y_offset = 132; // décallage du au vieillissement
bool joystick_print_on_serial = false;

void setup_joystick(){
  pinMode(joystick_x_pin, INPUT);
  pinMode(joystick_y_pin, INPUT);
}

void loop_joystick(){
  get_raw_joystick_datas();
  correct_raw_datas();
  set_digital_output();
  print_joystick_datas();
}
void get_raw_joystick_datas(){
  joystick_x_raw_datas = analogRead(joystick_x_pin);
  joystick_y_raw_datas = analogRead(joystick_y_pin);
}
void correct_raw_datas(){
  joystick_x_fixed_datas = joystick_x_raw_datas - j_x_offset;
  joystick_y_fixed_datas = joystick_y_raw_datas - j_y_offset;
}
void set_digital_output(){
  if(joystick_x_raw_datas > max_analog_lecture/2 + joystick_x_err){
    SendingData.joystick_x_value = 1;
  }
  else if(joystick_x_raw_datas < max_analog_lecture/2 - joystick_x_err){
    SendingData.joystick_x_value = -1;
  }
  else{
    SendingData.joystick_x_value = 0;
  }
  if(joystick_y_raw_datas > max_analog_lecture/2 + joystick_y_err){
    SendingData.joystick_y_value = 1;
  }
  else if(joystick_y_raw_datas < max_analog_lecture/2 - joystick_y_err){
    SendingData.joystick_y_value = -1;
  }
  else{
    SendingData.joystick_y_value = 0;
  }
}
void print_joystick_datas(){
  if(joystick_print_on_serial){
    Serial.printf(
      "raw_x: %ld , x: %ld , raw_y: %ld , y: %ld\n"
      ,joystick_x_raw_datas
      ,SendingData.joystick_x_value
      ,joystick_y_raw_datas
      ,SendingData.joystick_y_value
    );
  }
}