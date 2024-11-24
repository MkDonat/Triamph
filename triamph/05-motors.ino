//Modes
enum ModesMoteurs {
  MODE_STANDBY,
  MARCHE_AVANT,
  MARCHE_ARRIERE,
  ROTATION_HORAIRE,
  ROTATION_ANTI_HORAIRE
};
ModesMoteurs Mode_Moteur_actuel = MODE_STANDBY;
//Direction
typedef struct direction{
  int8_t x = 0; 
  int8_t y = 0;
}direction;
direction direction_triamph;
//DRIVER Moteurs
byte IN1_pin = 27;
byte IN2_pin = 26;
byte IN3_pin = 25;
byte IN4_pin = 33;
//Moteurs
const byte pin_moteur_gauche = 13;
const byte pin_moteur_droit = 14;
const byte cannal_moteurs = 0;
const byte resolution = 12;
uint32_t freq = 500;
uint32_t duty = 0; //Rapport Cyclique courrant

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
void loop_motors(){
  //Formattage des données de la commande des gaz!
  duty = receivedData.gaz_lecture;
  Serial.println(receivedData.gaz_lecture);
  selection_du_mode();
  maj_signaux_directionnels(); //Signaux envoyé sur le driver
  drive_motors(); // Action à effectuer suivant le mode
}
void selection_du_mode(){// Sélection du mode suivant les commandes.
  // Mise à jour de la direction
  direction_triamph.y = receivedData.joystick_y_value;
  direction_triamph.x = receivedData.joystick_x_value;
  if(direction_triamph.y != 0){
    switch(direction_triamph.y){
      case 1:
        Mode_Moteur_actuel = MARCHE_AVANT;
      break;
      case -1:
        Mode_Moteur_actuel = MARCHE_ARRIERE;
      break;
    }
  }
  else{
    switch(direction_triamph.x){
      case 1:
        Mode_Moteur_actuel = ROTATION_HORAIRE;
      break;
      case -1:
        Mode_Moteur_actuel = ROTATION_ANTI_HORAIRE;
      break;
      case 0:
        Mode_Moteur_actuel = MODE_STANDBY;
      break;
    }
  }
}
void maj_signaux_directionnels(){ // Envoie des signaux au driver pour régler le sens de rotation des moteurs
  if(Mode_Moteur_actuel == MARCHE_AVANT){
      digitalWrite(IN1_pin, 1);
      digitalWrite(IN2_pin, 0);
      digitalWrite(IN3_pin, 1);
      digitalWrite(IN4_pin, 0);
  }
  else if(Mode_Moteur_actuel == MARCHE_ARRIERE){
    digitalWrite(IN1_pin, 0);
    digitalWrite(IN2_pin, 1);
    digitalWrite(IN3_pin, 0);
    digitalWrite(IN4_pin, 1);
  }
  else if(Mode_Moteur_actuel == ROTATION_HORAIRE){
    digitalWrite(IN1_pin, 0);
    digitalWrite(IN2_pin, 1);
    digitalWrite(IN3_pin, 1);
    digitalWrite(IN4_pin, 0);
  }
  else if(Mode_Moteur_actuel == ROTATION_ANTI_HORAIRE){
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
  //Action à effectuer suivant le mode
  switch (Mode_Moteur_actuel) {
    case MODE_STANDBY:
      mode_standby();
      break;
    case MARCHE_AVANT:
      marche_avant();
      break;
    case MARCHE_ARRIERE:
      marche_arriere();
      break;
    case ROTATION_HORAIRE:
      rotation_horaire();
      break;
    case ROTATION_ANTI_HORAIRE:
      rotation_anti_horaire();
      break;
  }                             
}
void mode_standby(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    0 //uint32_t duty
  );
}
void marche_avant(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    duty //uint32_t duty
  );
}
void marche_arriere(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    duty //uint32_t duty
  );
}
void rotation_horaire(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    duty //uint32_t duty
  );
}
void rotation_anti_horaire(){
  ledcWriteChannel(
    cannal_moteurs //uint8_t channel
    , 
    duty //uint32_t duty
  );
}
