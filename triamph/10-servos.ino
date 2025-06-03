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

} ServoParams;

void servo_rollback(Servo* myservo, uint16_t from, uint16_t to, const uint8_t step, uint16_t speed){
  if(from < to) return; //avoid infinite loop in case of misplaced 'from' and 'to' positions
  speed = constrain(speed, 1, 100);
  speed = map(speed,1,100,1,1000);
  for (uint16_t pos = from; pos >= to; pos -= step){
    myservo->write(pos);             
    vTaskDelay(pdMS_TO_TICKS(15000/speed));
    //Serial.println(myservo->read());
    if (pos < step) break; // avoid underflow of uint16_t               
  }
  myservo->write(to); // drawing last exact position
}

void servo_roll(Servo *myservo, uint16_t from, uint16_t to, const uint8_t step, uint16_t speed){
  if(from > to) return; //avoid infinite loop in case of misplaced 'from' and 'to' positions
  speed = constrain(speed, 1, 100);
  speed = map(speed,0,100,1,1000);
  for (uint16_t pos = from; pos <= to; pos += step){
    myservo->write(pos);         
    vTaskDelay(pdMS_TO_TICKS(15000/speed));
    //Serial.println(myservo->read());
    if (UINT16_MAX - pos < step) break; // avoid overflow of uint16_t             
  }
  myservo->write(to); // drawing last exact position
}

void vTaskOperateServos(void* pvParameters){

  if (pvParameters == nullptr) {
    Serial.println("ERREUR : paramètre nul !");
    //vTaskDelete(NULL);
    is_OC_Clamps_task_complete = true;
    return;
  }
  ServoParams* params = (ServoParams*) pvParameters;

  //args extraction
  const char* name = params->name;
  Servo* servo = params->servo;
  uint16_t *last_consigne = params->last_consigne;
  servo_states *current_state_servo_gauche = params->current_state_servo_gauche;
  servo_states *current_state_servo_droit = params->current_state_servo_droit;
  bool *task_complete_flag = params->task_complete_flag;
  bool reverse_kinematic = params->reverse_kinematic;
  const uint8_t start_pose = params->start_pose;
  const uint8_t end_pose = params->end_pose;
  const uint8_t step = params->step;
  uint16_t roll_speed = params->roll_speed;
  uint16_t rollback_speed = params->rollback_speed;

  //master->slave coordination
  servo_states *this_servo_state;
  if(name == "servo_droit"){      //<-slave
    ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS(100));
    if(*current_state_servo_gauche == OPENED){
      if(reverse_kinematic==true){
        *current_state_servo_droit = CLOSED;
      }else{
        *current_state_servo_droit = OPENED;
      }
    }else{
      if(reverse_kinematic==true){
        *current_state_servo_droit = OPENED;
      }else{
        *current_state_servo_droit = CLOSED;
      }
    }
    this_servo_state = current_state_servo_droit;
  }else if(name == "servo_gauche"){  //<-master
    if(*last_consigne <= 1){
      *current_state_servo_droit = OPENED;
    }else{
      *current_state_servo_droit = CLOSED;
    }
    xTaskNotifyGive(xTask_OC_right_Clamp_Handle);
    this_servo_state = current_state_servo_gauche;
  }

  //operating servo
  switch(*this_servo_state){
    case CLOSED:
      Serial.print(name);
      Serial.println(": ROLLBACK");
      servo_rollback(servo, start_pose, end_pose, step, rollback_speed);
      *this_servo_state = OPENED;
    break;
    case OPENED:
      Serial.print(name);
      Serial.println(": ROLL");
      servo_roll(servo, end_pose, start_pose, step, roll_speed);
      *this_servo_state = CLOSED;
    break;
  }
  vTaskDelay(pdMS_TO_TICKS(50));//let servos operate
  *task_complete_flag = true;
  for(;;){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}