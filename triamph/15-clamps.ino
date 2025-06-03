ServoParams right_clamp_params = {
  .name = "servo_droit",
  .servo = &sg90_droit,
  .last_consigne = &sg90_last_consigne_right_clamp,
  .current_state_servo_gauche = &left_clamp_current_state,
  .current_state_servo_droit = &right_clamp_current_state,
  .task_complete_flag = &is_OC_Clamps_task_complete,
  .reverse_kinematic = sg90_reverse_kinematic,
  .start_pose = sg90_start_pose,
  .end_pose = sg90_end_pose,
  .step = sg90_step,
  .roll_speed = sg90_roll_speed,
  .rollback_speed = sg90_rollback_speed
};

ServoParams left_clamp_params = {
  .name = "servo_gauche",
  .servo = &sg90_gauche,
  .last_consigne = &sg90_last_consigne_left_clamp,
  .current_state_servo_gauche = &left_clamp_current_state,
  .current_state_servo_droit = &right_clamp_current_state,
  .task_complete_flag = &is_OC_Clamps_task_complete,
  .reverse_kinematic = sg90_reverse_kinematic,
  .start_pose = sg90_start_pose,
  .end_pose = sg90_end_pose,
  .step = sg90_step,
  .roll_speed = sg90_roll_speed,
  .rollback_speed = sg90_rollback_speed
};

void xTask_OC_TimerCallback(TimerHandle_t xTimer){
  is_OC_Clamps_task_complete = true;
}

