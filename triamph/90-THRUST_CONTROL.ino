void onEnter_THRUST_CONTROL(){
  Serial.println("ENTERRING THRUST CONTROL MODE");
  thrust_control_mode_select();
}
void onRun_THRUST_CONTROL(){
  duty = *THRUST_FORCE;
  //Signaux envoyé sur le driver
  drive_motors(); // Action à effectuer suivant le mode
}
void onExit_THRUST_CONTROL(){
  Serial.println("EXITING THRUST CONTROL MODE");
  stop_motors();
}