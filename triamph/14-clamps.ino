ServoParams right_clamp_params = { 
  .name = "servo_droit",
  .servo = &sg90_droit,
  .last_consigne = &sg90_right_last_consigne,
  .current_state_servo_gauche = &sg90_left_clamp_current_state,
  .current_state_servo_droit = &sg90_right_clamp_current_state,
  .task_complete_flag = &is_OC_Clamps_task_complete,
  .reverse_kinematic = sg90_reverse_kinematic,
  .start_pose = sg90_right_start_pose,
  .end_pose = sg90_right_end_pose,
  .step = sg90_step,
  .roll_speed = sg90_roll_speed,
  .rollback_speed = sg90_rollback_speed,
  .slave_servo_task_handler = &xTask_OC_right_Clamp_Handle
};

ServoParams left_clamp_params = {
  .name = "servo_gauche",
  .servo = &sg90_gauche,
  .last_consigne = &sg90_left_last_consigne,
  .current_state_servo_gauche = &sg90_left_clamp_current_state,
  .current_state_servo_droit = &sg90_right_clamp_current_state,
  .task_complete_flag = &is_OC_Clamps_task_complete,
  .reverse_kinematic = sg90_reverse_kinematic,
  .start_pose = sg90_left_start_pose,
  .end_pose = sg90_left_end_pose,
  .step = sg90_step,
  .roll_speed = sg90_roll_speed,
  .rollback_speed = sg90_rollback_speed,
  .slave_servo_task_handler = &xTask_OC_right_Clamp_Handle
};

void xTask_OC_TimerCallback(TimerHandle_t xTimer){
  is_OC_Clamps_task_complete = true;
}
