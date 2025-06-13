void onEnter_THRUST_CONTROL(){
  Serial.println("ENTERRING THRUST CONTROL MODE");
  thrust_control_mode_select();
}
void onRun_THRUST_CONTROL(){
  if(THRUST_CONTROL_MODE == THRUST_FORWARD){
    raw_duty = map(
      *THRUST_FORCE, 2047, 0
      ,
      0,duty_map_max
    );
  }else if(THRUST_CONTROL_MODE == THRUST_BACKWARD){
    raw_duty = map(
      *THRUST_FORCE, 2047, 4095
      ,
      0,duty_map_max
    );
  }
  duty = Progressive_filtering_of_the_duty_cycle(duty, raw_duty, ramp_rate);
  drive_motors();
}
void onExit_THRUST_CONTROL(){
  duty = 0;
  stop_motors();
  THRUST_CONTROL_MODE = THRUST_IDLE;
  Serial.println("EXITING THRUST CONTROL MODE");
}