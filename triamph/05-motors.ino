const byte left_motor = 12;
const byte right_motor = 14;
const byte lr_motors = 0;
const byte resolution = 16;
uint32_t freq = 500;
uint32_t max_duty = (int)(pow(2, resolution) - 1);
uint32_t semi_duty = int(50*(max_duty/100));
uint32_t duty = 0; //Rapport Cyclique courrant
uint32_t treshold_trigger_duty = int(35*(max_duty/100)); //Rapport Cyclique de déclenchement en %
uint32_t min_operating_duty = int(10*(max_duty/100)); //Rapport Cyclique min de fonctionnement en %
bool motor_trigged = false;

void Avance_moteur(void *arg) {
  //Attach Left Motor
  ledcAttachChannel(
  left_motor //uint8_t pin
  , 
  freq //uint32_t freq
  ,
  resolution //uint8_t resolution
  ,
  lr_motors //int8_t channel
  );
  //Attach Right Motor
  ledcAttachChannel(
  right_motor //uint8_t pin
  , 
  freq //uint32_t freq
  ,
  resolution //uint8_t resolution
  ,
  lr_motors //int8_t channel
  );
  for(;;){
    duty = map(
    receivedData.gaz_lecture,
    0 , 4095 //ESP32 native reading resolution.
    ,
    min_operating_duty , max_duty
    );
    if(duty <= min_operating_duty + receivedData.gaz_err){
      duty = 0;
    }
    else if(duty < semi_duty){
      duty = (int)((duty/receivedData.gaz_err)*receivedData.gaz_err);
    }
    if(duty > min_operating_duty + receivedData.gaz_err && motor_trigged == false ){
      ledcWriteChannel( //Then we trigge motor
        lr_motors //uint8_t channel
        , 
        treshold_trigger_duty //uint32_t duty
      );
      motor_trigged = true ;
      delay(10);
    }
    else if(duty <= 0){  // not need
      motor_trigged = false ;
    }
    ledcWriteChannel(
      lr_motors //uint8_t channel
      , 
      duty //uint32_t duty
    );
    /*Serial.printf("duty: %d/%d (pot_lec: %d) Trigged: %d\n"
    ,duty,max_duty,lecture_potentio_moteur,motor_trigged); */                                             
  }          
}
void CreateTasksForMotors(){
  xTaskCreatePinnedToCore(
    Avance_moteur,"Faire avancer le triamph", 2048
    ,
    NULL // Stack Depth
    ,
    5 //Priority
    ,
    NULL //Task handle
    ,
    CORE_1 // Core on which the task will run
  );
}