int16_t* THRUST_FORCE = &receivedData.L_J_analog_y;
int16_t* YAWL_FORCE = &receivedData.L_J_analog_x;
int8_t* dir_x = &receivedData.L_J_tor_x;
int8_t* dir_y = &receivedData.L_J_tor_y;
//Modes de fonctionnement des moteurs
enum THRUST_CONTROL_MODES { //Modes de poussée
  THRUST_IDLE,
  THRUST_BACKWARD,
  THRUST_FORWARD
};
THRUST_CONTROL_MODES THRUST_CONTROL_MODE = THRUST_IDLE;
enum YAW_CONTROL_MODES { //Modes de rotation
  YAW_IDLE,
  YAW_CLOCKWISE,
  YAW_COUNTERCLOCKWISE
};
YAW_CONTROL_MODES YAW_CONTROL_MODE = YAW_IDLE;
//Motor Driver
byte IN1A_pin = 27;
byte IN2A_pin = 26;
byte IN1B_pin = 25;
byte IN2B_pin = 33;
//Motors
const uint8_t resolution = 12;
uint32_t freq = 5000;
uint32_t duty = 0; //safe and usable duty
uint32_t raw_duty = 0;
float ramp_rate = 3.0; // lowest == smothest
uint32_t start_duty = 0;
uint32_t target_duty = 0;
uint8_t duty_map_max = 247; //97% * 255

uint32_t Progressive_filtering_of_the_duty_cycle(uint32_t duty_safe, uint32_t duty_target, float ramp_rate){
  if (duty_safe < duty_target) {
    duty_safe += min((int)ramp_rate, (int)(duty_target - duty_safe));
  } else if (duty_safe > duty_target) {
    duty_safe -= min((int)ramp_rate, (int)(duty_safe - duty_target));
  }
  return duty_safe;
}

void setup_motors() {
  //Driver
  pinMode(IN1A_pin,OUTPUT);
  pinMode(IN2A_pin,OUTPUT);
  pinMode(IN1B_pin,OUTPUT);
  pinMode(IN2B_pin,OUTPUT);
  analogWriteResolution(
    IN1A_pin,//uint8_t pin, 
    resolution//uint8_t resolution
  );
  analogWriteResolution(
    IN2A_pin,//uint8_t pin, 
    resolution//uint8_t resolution
  );
  analogWriteResolution(
    IN1B_pin,//uint8_t pin, 
    resolution//uint8_t resolution
  );
  analogWriteResolution(
    IN2B_pin,//uint8_t pin, 
    resolution//uint8_t resolution
  );
  analogWriteFrequency(
    IN1A_pin,//uint8_t pin, 
    freq//uint32_t freq
  );
  analogWriteFrequency(
    IN2A_pin,//uint8_t pin, 
    freq//uint32_t freq
  );
  analogWriteFrequency(
    IN1B_pin,//uint8_t pin, 
    freq//uint32_t freq
  );
  analogWriteFrequency(
    IN2B_pin,//uint8_t pin, 
    freq//uint32_t freq
  );
}

void thrust_control_mode_select(){
  if( *dir_y == 1 ){
    THRUST_CONTROL_MODE = THRUST_FORWARD;
  }else if(*dir_y == -1){ //Condition à définir
    THRUST_CONTROL_MODE = THRUST_BACKWARD;
  }
  else{
    THRUST_CONTROL_MODE = THRUST_IDLE;
  }
}

void yaw_control_mode_select(){
  if( *dir_x == 1 ){
    YAW_CONTROL_MODE = YAW_CLOCKWISE;
  }
  else if( *dir_x == -1 ){
    YAW_CONTROL_MODE = YAW_COUNTERCLOCKWISE;
  }else{
    YAW_CONTROL_MODE = YAW_IDLE;
  }
}

void drive_motors(){
  if(THRUST_CONTROL_MODE == THRUST_FORWARD){
    analogWrite(IN1A_pin, duty);
    analogWrite(IN2A_pin, 0);
    analogWrite(IN1B_pin, duty);
    analogWrite(IN2B_pin, 0);
  }
  else if(THRUST_CONTROL_MODE == THRUST_BACKWARD){
    analogWrite(IN1A_pin, 0);
    analogWrite(IN2A_pin, duty);
    analogWrite(IN1B_pin, 0);
    analogWrite(IN2B_pin, duty);
  }
  else if(YAW_CONTROL_MODE == YAW_CLOCKWISE){
    analogWrite(IN1A_pin, 0);
    analogWrite(IN2A_pin, duty);
    analogWrite(IN1B_pin, duty);
    analogWrite(IN2B_pin, 0);
  }
  else if(YAW_CONTROL_MODE == YAW_COUNTERCLOCKWISE){
    analogWrite(IN1A_pin, duty);
    analogWrite(IN2A_pin, 0);
    analogWrite(IN1B_pin, 0);
    analogWrite(IN2B_pin, duty);
  }
}

void stop_motors(){
  analogWrite(IN1A_pin, 0);
  analogWrite(IN1B_pin, 0);
  analogWrite(IN2A_pin, 0);
  analogWrite(IN2B_pin, 0);
}