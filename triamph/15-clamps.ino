/* 
  Les SG90 sont utilisés pour gérer l’ouverture fermeture des pinces :)
*/
typedef struct {
  Servo* servo;
} ClampTaskParams;

ClampTaskParams right_clamp_params = {
  .servo = &sg90_droit
};
ClampTaskParams left_clamp_params = {
  .servo = &sg90_gauche
};

enum clamps_states{
  CLOSED,
  OPENED
};
clamps_states clamps_current_state = OPENED;

uint16_t clamps_last_consigne_pose = 1;
const uint8_t clamps_Closed_pose = 179; //Position chargée en degrés
const uint8_t clamps_Opened_pose = 1; //Position déchargée en degrés
uint16_t clamps_target_pose = 1;
const uint16_t clamps_servo_step = 1;
const uint16_t clamps_roll_speed = 100;
const uint16_t clamps_rollback_speed = 100;

TimerHandle_t xTask_OC_TimerHandler = NULL;
TaskHandle_t xTask_OC_right_Clamp_Handle = NULL; //Task Handler
TaskHandle_t xTask_OC_left_Clamp_Handle = NULL; //Task Handler

//task timer callback
void xTask_OC_TimerCallback(TimerHandle_t xTimer){
  //Mark the task as finished even if not (preserving processing time)
  is_OC_Clamps_task_complete = true;
}

void vTask_OC_clamp(void* pvParameters){

  // Vérifie si le pointeur vers la structure est nul
  if (pvParameters == nullptr) {
    Serial.println("ERREUR : paramètre nul !");
    //vTaskDelete(NULL);
    is_OC_Clamps_task_complete = true;
    return;
  }

  ClampTaskParams* params = (ClampTaskParams*) pvParameters;

  if(params->servo == nullptr) {
    Serial.println("ERREUR : pointeur de servo invalide !");
    //vTaskDelete(NULL);
    is_OC_Clamps_task_complete = true;
    return;
  }

  Servo* servo = params->servo;

  if (!servo->attached()) {
    Serial.println("Servo non attaché !");
    //vTaskDelete(NULL);
    is_OC_Clamps_task_complete = true;
    return;
  }
  switch(clamps_current_state){
    case CLOSED:
      clamps_target_pose = clamps_Opened_pose;
      Serial.println("ROLLBACK");
      servo_rollback(params->servo, clamps_last_consigne_pose, clamps_target_pose, clamps_servo_step, clamps_rollback_speed);
      clamps_last_consigne_pose = clamps_target_pose;
      clamps_current_state = OPENED;
    break;

    case OPENED:
      clamps_target_pose = clamps_Closed_pose;
      Serial.println("ROLL");
      servo_roll(params->servo, clamps_last_consigne_pose, clamps_target_pose, clamps_servo_step, clamps_roll_speed);
      clamps_last_consigne_pose = clamps_target_pose;
      clamps_current_state = CLOSED;
    break;
  }
  vTaskDelay(pdMS_TO_TICKS(50)); // pour laisser le mouvement se terminer
  is_OC_Clamps_task_complete = true;
  for(;;){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}