void onEnter_THRUST_CONTROL(){
  Serial.println("ENTERRING THRUST CONTROL MODE");
  thrust_control_mode_select();
}
void onRun_THRUST_CONTROL(){
  if(THRUST_CONTROL_MODE == THRUST_FORWARD){
    duty = map(
      (uint32_t)*THRUST_FORCE, 2047, 4095,
      0,230
    );
  }else if(THRUST_CONTROL_MODE == THRUST_BACKWARD){
    duty = map(
      (uint32_t)*THRUST_FORCE, 2047, 0,
      0,230
    );
  }
  drive_motors();
}
void onExit_THRUST_CONTROL(){
  Serial.println("EXITING THRUST CONTROL MODE");
  stop_motors();
}