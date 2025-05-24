ServoParams parametres_servo_levage_droit = {
  .name = "ServoLevageDroit",
  .servo = &ds3218_droit,
  .last_consigne = &ds3218_last_consigne_pose
};

ServoParams parametres_servo_levage_gauche = {
  .name = "ServoLevageGauche",
  .servo = &ds3218_gauche,
  .last_consigne = &ds3218_last_consigne_pose
};

enum LU_STATES{
  LOADED,
  UNLOADED
};
LU_STATES LU_STATE = LOADED;

//servo droit
TaskHandle_t xTask_LoadUnload_RightServo_Handle = NULL;
//servo gauche
TaskHandle_t xTask_LoadUnload_LeftServo_Handle = NULL;
//Varriables utile pour la tâche
TimerHandle_t xTask_LoadUnload_TimerHandler = NULL;
const uint16_t LoadUnload_Time = 15000;
const uint16_t servo_levage_speed = 20; //0-100
const uint16_t servo_levage_step = 1;
const uint16_t levage_position_chargee = 1;
const uint16_t levage_position_dechargee = 179;

//task timer callback
void xTask_LoadUnload_TimerCallback(TimerHandle_t xTimer){
  //Mark the task as finished even if not (preserving processing time)
  is_LoadUnload_task_complete = true;
}

void vTask_LoadUnload(void* args){
  // Vérifie si le pointeur vers la structure est nul
  if (args == nullptr) {
    Serial.println("ERREUR : paramètre nul !");
    //vTaskDelete(NULL);
    is_LoadUnload_task_complete = true;
    return;
  }
  // Getting data structure
  ServoParams* params = (ServoParams*) args;
  //now we extract parameters
  const char* name = params->name;
  Servo* servo = params->servo;
  uint16_t *last_consigne = params->last_consigne;

  if(*last_consigne <= 1){
    LU_STATE = LOADED;
  }else{
    LU_STATE = UNLOADED;
  }

  switch(LU_STATE){
    case UNLOADED:
      Serial.printf("%s: Déchargement en cours...\n",name);
      servo_rollback(servo, levage_position_dechargee, levage_position_chargee, servo_levage_step, servo_levage_speed);
      LU_STATE = LOADED;
    break;

    case LOADED:
      Serial.printf("%s: Chargement en cours...\n",name);
      servo_roll(servo, levage_position_chargee, levage_position_dechargee, servo_levage_step, servo_levage_speed);
      LU_STATE = UNLOADED;
    break;
  }
  *last_consigne = servo->read();
  vTaskDelay(pdMS_TO_TICKS(50)); // pour laisser le mouvement se terminer
  is_LoadUnload_task_complete = true;

  for(;;){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}