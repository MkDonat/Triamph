void onEnter_THRUST_CONTROL(){
  thrust_control_mode_select();
}
void onRun_THRUST_CONTROL(){
  duty = receivedData.gaz_value;
  //Signaux envoyé sur le driver
  drive_motors(); // Action à effectuer suivant le mode
}
void onExit_THRUST_CONTROL(){
  THRUST_CONTROL_MODE = THRUST_IDLE;
}