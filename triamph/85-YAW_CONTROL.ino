void onEnter_YAW_CONTROL(){
  Serial.println("ENTERRING YAW CONTROL MODE");
  yaw_control_mode_select();
}
void onRun_YAW_CONTROL(){
  if(YAW_CONTROL_MODE == YAW_CLOCKWISE){
    duty = map(
      (uint32_t)*YAWL_FORCE, 2047, 4095,
      0,duty_map_max
    );
  }else if(YAW_CONTROL_MODE == YAW_COUNTERCLOCKWISE){
    duty = map(
      (uint32_t)*YAWL_FORCE, 2047, 0,
      0,duty_map_max
    );
  }
  drive_motors();
}
void onExit_YAW_CONTROL(){
  YAW_CONTROL_MODE = YAW_IDLE;
  Serial.println("EXITING YAW CONTROL MODE");
  stop_motors();
}