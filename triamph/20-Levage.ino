ServoParams right_servo_levage_params = {
  .name = "servo_droit",
  .servo = &ds3218_droit,
  .last_consigne = &ds3218_right_last_consigne,
  .current_state_servo_gauche = &ds3218_left_clamp_current_state,
  .current_state_servo_droit = &ds3218_right_clamp_current_state,
  .task_complete_flag = &is_LoadUnload_task_complete,
  .reverse_kinematic = ds3218_reverse_kinematic,
  .start_pose = ds3218_start_pose,
  .end_pose = ds3218_end_pose,
  .step = ds3218_step,
  .roll_speed = ds3218_roll_speed,
  .rollback_speed = ds3218_rollback_speed,
  .slave_servo_task_handler = &xTask_LoadUnload_RightServo_Handle
};

ServoParams left_servo_levage_params = {
  .name = "servo_gauche",
  .servo = &ds3218_gauche,
  .last_consigne = &ds3218_left_last_consigne,
  .current_state_servo_gauche = &ds3218_left_clamp_current_state,
  .current_state_servo_droit = &ds3218_right_clamp_current_state,
  .task_complete_flag = &is_LoadUnload_task_complete,
  .reverse_kinematic = ds3218_reverse_kinematic,
  .start_pose = ds3218_start_pose,
  .end_pose = ds3218_end_pose,
  .step = ds3218_step,
  .roll_speed = ds3218_roll_speed,
  .rollback_speed = ds3218_rollback_speed,
  .slave_servo_task_handler = &xTask_LoadUnload_RightServo_Handle
};

void xTask_LoadUnload_TimerCallback(TimerHandle_t xTimer){
  //Mark the task as finished even if not (preserving processing time)
  is_LoadUnload_task_complete = true;
}