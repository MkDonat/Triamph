void onEnter_YAW_CONTROL(){
  yaw_control_mode_select();
}
void onRun_YAW_CONTROL(){
  drive_motors();
}
void onExit_YAW_CONTROL(){
  YAW_CONTROL_MODE = YAW_IDLE;
}