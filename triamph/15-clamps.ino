ServoParams right_clamp_params = {
  .name = "right_clamp",
  .servo = &sg90_droit,
  .last_consigne = &sg90_last_consigne_pose
};

ServoParams left_clamp_params = {
  .name = "left_clamp",
  .servo = &sg90_gauche,
  .last_consigne = &sg90_last_consigne_pose
};

enum clamps_states{
  CLOSED,
  OPENED
};
clamps_states clamps_current_state = CLOSED;

const uint8_t clamps_Closed_pose = 179; //Position chargée en degrés
const uint8_t clamps_Opened_pose = 1; //Position déchargée en degrés
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
  ServoParams* params = (ServoParams*) pvParameters;

  //now we extract parameters
  const char* name = params->name;
  Servo* servo = params->servo;
  uint16_t *last_consigne = params->last_consigne;

  if(*last_consigne <= 1){
    clamps_current_state = OPENED;
  }else{
    clamps_current_state = CLOSED;
  }

  switch(clamps_current_state){
    case CLOSED:
      Serial.println("ROLLBACK");
      servo_rollback(servo, clamps_Closed_pose, clamps_Opened_pose, clamps_servo_step, clamps_rollback_speed);
      clamps_current_state = OPENED;
    break;

    case OPENED:
      Serial.println("ROLL");
      servo_roll(servo, clamps_Opened_pose, clamps_Closed_pose, clamps_servo_step, clamps_roll_speed);
      clamps_current_state = CLOSED;
    break;
  }
  *last_consigne = servo->read();
  vTaskDelay(pdMS_TO_TICKS(50)); //Pour laisser le mouvement se terminer
  is_OC_Clamps_task_complete = true;
  for(;;){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}