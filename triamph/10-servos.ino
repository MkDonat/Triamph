typedef struct {
  const char* name;
  Servo* servo;
  uint16_t *last_consigne;
  servo_states *current_state_servo_gauche;
  servo_states *current_state_servo_droit;
  bool *task_complete_flag;
  bool reverse_kinematic;
  const uint8_t start_pose;
  const uint8_t end_pose;
  const uint8_t step;
  uint16_t roll_speed;
  uint16_t rollback_speed;
  TaskHandle_t *slave_servo_task_handler;
} ServoParams;

void servo_rollback(Servo* myservo, uint16_t from, uint16_t to, const uint8_t step, uint16_t speed, uint16_t *last_consigne){
  if(from < to) return; //avoid infinite loop in case of misplaced 'from' and 'to' positions
  speed = constrain(speed, 1, 100);
  speed = map(speed,1,100,1,1000);
  for (uint16_t pos = from; pos >= to; pos -= step){
    myservo->write(pos);
    *last_consigne = myservo->read();             
    vTaskDelay(pdMS_TO_TICKS(15000/speed));
    //Serial.println(myservo->read());
    if (pos < step) break; // avoid underflow of uint16_t               
  }
  myservo->write(to); // drawing last exact position
}

void servo_roll(Servo *myservo, uint16_t from, uint16_t to, const uint8_t step, uint16_t speed, uint16_t *last_consigne){
  if(from > to) return; //avoid infinite loop in case of misplaced 'from' and 'to' positions
  speed = constrain(speed, 1, 100);
  speed = map(speed,0,100,1,1000);
  for (uint16_t pos = from; pos <= to; pos += step){
    myservo->write(pos);
    *last_consigne = myservo->read();          
    vTaskDelay(pdMS_TO_TICKS(15000/speed));
    //Serial.println(myservo->read());
    if (UINT16_MAX - pos < step) break; // avoid overflow of uint16_t             
  }
  myservo->write(to); // drawing last exact position
}

void vTaskOperateServos(void* pvParameters) {
  if (pvParameters == nullptr) {
    Serial.println("ERREUR : paramètre nul !");
    is_OC_Clamps_task_complete = true;
    return;
  }

  ServoParams* params = (ServoParams*) pvParameters;

  // Extraction des arguments
  const char* name = params->name;
  Servo* servo = params->servo;
  uint16_t *last_consigne = params->last_consigne;
  servo_states* current_state_servo_gauche = params->current_state_servo_gauche;
  servo_states* current_state_servo_droit = params->current_state_servo_droit;
  bool* task_complete_flag = params->task_complete_flag;
  bool reverse_kinematic = params->reverse_kinematic;
  const uint8_t start_pose = params->start_pose;
  const uint8_t end_pose = params->end_pose;
  const uint8_t step = params->step;
  uint16_t roll_speed = params->roll_speed;
  uint16_t rollback_speed = params->rollback_speed;
  TaskHandle_t* slave_servo_task_handler = params->slave_servo_task_handler;

  servo_states* this_servo_state;

  // --------- MAÎTRE : servo_gauche ---------
  if (strcmp(name, "servo_gauche") == 0) {
    // Alterne l'état du maître
    if (*current_state_servo_gauche == CLOSED) {
      *current_state_servo_gauche = OPENED;
    } else {
      *current_state_servo_gauche = CLOSED;
    }

    // Définit l'état du servo droit en fonction de reverse_kinematic
    if (reverse_kinematic) {
      *current_state_servo_droit = (*current_state_servo_gauche == OPENED) ? CLOSED : OPENED;
    } else {
      *current_state_servo_droit = *current_state_servo_gauche;
    }

    // Envoie une notification à l'esclave
    xTaskNotifyGive(*slave_servo_task_handler);

    this_servo_state = current_state_servo_gauche;

  // --------- ESCLAVE : servo_droit ---------
  } else if (strcmp(name, "servo_droit") == 0) {
    // Attend d'être notifié par le maître
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // L’état du servo droit a déjà été défini par le maître
    this_servo_state = current_state_servo_droit;
  }

  // --------- Mouvement du servo ---------
  Serial.print(name);
  Serial.print(" | État : ");
  Serial.println(*this_servo_state == OPENED ? "OPENED" : "CLOSED");

  uint16_t start_point = *last_consigne;
  switch (*this_servo_state) {
    case CLOSED:
      Serial.print(name); Serial.println(" : ROLLBACK");
      servo_rollback(servo, start_point/*start_pose*/, end_pose, step, rollback_speed, last_consigne);
    break;
    case OPENED:
      Serial.print(name); Serial.println(" : ROLL");
      servo_roll(servo, start_point/*end_pose*/, start_pose, step, roll_speed, last_consigne);
    break;
  }

  // Drapeau de complétion + tempo
  vTaskDelay(pdMS_TO_TICKS(50));
  *task_complete_flag = true;

  // Boucle passive (garde la tâche vivante)
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
