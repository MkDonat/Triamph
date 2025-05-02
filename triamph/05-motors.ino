int16_t* THRUST_FORCE = &receivedData.L_J_analog_y;
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
byte IN1_pin = 27;
byte IN2_pin = 26;
byte IN3_pin = 25;
byte IN4_pin = 33;
//Motors
const uint8_t pin_moteur_gauche = 13;
const uint8_t pin_moteur_droit = 14;
const uint8_t cannal_moteurs = 0;
const uint8_t resolution = 12;
uint32_t freq = 500;
uint16_t duty = 0; //Rapport Cyclique courrant

void setup_motors() {
  //Driver
  pinMode(IN1_pin,OUTPUT);
  pinMode(IN2_pin,OUTPUT);
  pinMode(IN3_pin,OUTPUT);
  pinMode(IN4_pin,OUTPUT);
  //Attach Left Motor
  ledcAttachChannel(
    pin_moteur_gauche //uint8_t pin
    , 
    freq //uint32_t freq
    ,
    resolution //uint8_t resolution
    ,
    cannal_moteurs //int8_t channel
  );
  //Attach Right Motor
  ledcAttachChannel(
    pin_moteur_droit //uint8_t pin
    , 
    freq //uint32_t freq
    ,
    resolution //uint8_t resolution
    ,
    cannal_moteurs //int8_t channel
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
  driver_signal_update();
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
  driver_signal_update();
}
void driver_signal_update(){ //Modif du sens de rotation
  if(THRUST_CONTROL_MODE == THRUST_FORWARD){
      digitalWrite(IN1_pin, 1);
      digitalWrite(IN2_pin, 0);
      digitalWrite(IN3_pin, 1);
      digitalWrite(IN4_pin, 0);
  }
  else if(THRUST_CONTROL_MODE == THRUST_BACKWARD){
    digitalWrite(IN1_pin, 0);
    digitalWrite(IN2_pin, 1);
    digitalWrite(IN3_pin, 0);
    digitalWrite(IN4_pin, 1);
  }
  else if(YAW_CONTROL_MODE == YAW_CLOCKWISE){
    digitalWrite(IN1_pin, 0);
    digitalWrite(IN2_pin, 1);
    digitalWrite(IN3_pin, 1);
    digitalWrite(IN4_pin, 0);
  }
  else if(YAW_CONTROL_MODE == YAW_COUNTERCLOCKWISE){
    digitalWrite(IN1_pin, 1);
    digitalWrite(IN2_pin, 0);
    digitalWrite(IN3_pin, 0);
    digitalWrite(IN4_pin, 1);
  }
  else{ //Roue Libre
    digitalWrite(IN1_pin, 0); 
    digitalWrite(IN2_pin, 0);
    digitalWrite(IN3_pin, 0);
    digitalWrite(IN4_pin, 0);
  }
}
void drive_motors(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    duty //uint32_t duty
  );                   
}
void stop_motors(){
  THRUST_CONTROL_MODE = THRUST_IDLE;
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    0 //uint32_t duty
  );
  driver_signal_update();                   
}
