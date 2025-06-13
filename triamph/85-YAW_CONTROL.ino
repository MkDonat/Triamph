void onEnter_YAW_CONTROL(){
  Serial.println("ENTERRING YAW CONTROL MODE");
  yaw_control_mode_select();
}
void onRun_YAW_CONTROL(){
  if(YAW_CONTROL_MODE == YAW_CLOCKWISE){
    raw_duty = map(
      *YAWL_FORCE, 2047, 0
      , 
      0, duty_map_max
    );
  }else if(YAW_CONTROL_MODE == YAW_COUNTERCLOCKWISE){
    raw_duty = map(
      *YAWL_FORCE, 2047, 4095
      ,
      0,duty_map_max
    );
  }
  duty = Progressive_filtering_of_the_duty_cycle(duty, raw_duty, ramp_rate);
  drive_motors();
}
void onExit_YAW_CONTROL(){
  duty = 0;
  stop_motors();
  YAW_CONTROL_MODE = YAW_IDLE;
  Serial.println("EXITING YAW CONTROL MODE");
}