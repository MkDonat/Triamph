//Modes de fonctionnement du verin
enum ETATS_DU_VERIN {
  VERIN_IDLE,
  VERIN_SORTI,
  VERIN_RENTRE
};
ETATS_DU_VERIN ETAT_DU_VERIN = VERIN_IDLE;
//Verin Motor Driver
byte IN1_pin = 18;
byte IN2_pin = 19;
//Verin
const uint8_t pin_verin = 32;//ENA
const uint8_t cannal_verin = 0;
const uint8_t resolution_verin = 12;
uint32_t freq_verin = 500;
uint32_t duty_verin = 0; //Rapport Cyclique courrant
//COLLECTING Task Timer settings
TimerHandle_t xTask_COLLECTING_TimerHandler = NULL;
uint32_t xTask_COLLECTING_length = 5000; //20sec
void xTask_COLLECTING_TimerCallback(TimerHandle_t xTimer);
//COLLECTING Task settings
bool Collecting_task_completed = false;

void setup_verin(){
  //Driver
  pinMode(IN1_pin,OUTPUT);
  pinMode(IN2_pin,OUTPUT);
  //Attach verin
  ledcAttachChannel(
    pin_verin //uint8_t pin
    , 
    freq_verin //uint32_t freq
    ,
    resolution_verin //uint8_t resolution
    ,
    cannal_verin //int8_t channel
  );
}
void selection_du_mode_de_deplacement(){
  ////code here
  if(ETAT_DU_VERIN == VERIN_SORTI){
    ETAT_DU_VERIN = VERIN_RENTRE;
  }else if(ETAT_DU_VERIN == VERIN_RENTRE){
    ETAT_DU_VERIN == VERIN_SORTI;
  }else{
    ETAT_DU_VERIN == VERIN_SORTI;
  }
  driver_verin_signal_update();
}
void driver_verin_signal_update(){ //Modif du sens de déplacement
  if(ETAT_DU_VERIN == VERIN_SORTI){
      digitalWrite(IN1_pin, 1);
      digitalWrite(IN2_pin, 0);
  }
  else if(ETAT_DU_VERIN == VERIN_RENTRE){
    digitalWrite(IN1_pin, 0);
    digitalWrite(IN2_pin, 1);
  }
}
void deplacement_verin(){
  ledcWrite(
    pin_verin //uint8_t channel
    , 
    2047//duty //uint32_t duty
  );                   
}
void stop_verin(){
  THRUST_CONTROL_MODE = THRUST_IDLE;
  ledcWrite(
    pin_verin //uint8_t channel
    , 
    0 //uint32_t duty
  );
  driver_verin_signal_update();                   
}
void xTask_COLLECTING_TimerCallback(TimerHandle_t xTimer){
  Collecting_task_completed = true;
}
