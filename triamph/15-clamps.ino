/* 
  Les SG90 sont utilisés pour gérer l’ouverture fermeture des pinces :)
*/
uint16_t clamps_last_consigne_pose = 1;
const uint8_t clamps_Closed_pose = 179; //Position chargée en degrés
const uint8_t clamps_Opened_pose = 1; //Position déchargée en degrés
uint16_t clamps_target_pose = 1;
const uint16_t clamps_servo_step = 1;
const uint16_t clamps_roll_speed = 100;
const uint16_t clamps_rollback_speed = 100;
TaskHandle_t xTask_OC_Clamps_Handle = NULL; //Task Handler
TimerHandle_t xTask_OC_TimerHandler = NULL;
enum clamps_states{
  CLOSED,
  OPENED,
  NONE
};
clamps_states clamps_current_state = OPENED;

//functions
void xTask_OC_TimerCallback(TimerHandle_t xTimer){
  //Mark the tast as finished even if not (preserving processing time)
  is_OC_Clamps_task_complete = true;
}

void vTask_OC_clamps(void* args){

  if(clamps_current_state == CLOSED){
    clamps_target_pose = clamps_Opened_pose;
  }
  else if(clamps_current_state == OPENED){
    clamps_target_pose = clamps_Closed_pose;
  }
  switch(clamps_current_state){
    case CLOSED:
      //actions
      servo_rollback(
        sg90_droit, //servo instance
        clamps_last_consigne_pose, //start pose
        clamps_target_pose, //finish pose
        clamps_servo_step, //step
        clamps_rollback_speed //speed
      );
      clamps_last_consigne_pose = clamps_target_pose;
      clamps_current_state = OPENED;
      //nescessary delay to complete all operations
      vTaskDelay(pdMS_TO_TICKS(15));
      break;
    case OPENED:
      //actions
      servo_roll(
        sg90_droit, //servo instance
        clamps_last_consigne_pose, //start pose
        clamps_target_pose, //finish pose
        clamps_servo_step,  //step
        clamps_roll_speed   //speed
      );
      clamps_last_consigne_pose = clamps_target_pose;
      clamps_current_state = CLOSED;
      //nescessary delay to complete all operations
      vTaskDelay(pdMS_TO_TICKS(15));
      break;
  }
  //Mark the tast as finished
  is_OC_Clamps_task_complete = true;
  for(;;){
    //nothing to do here :) 
  }
}
