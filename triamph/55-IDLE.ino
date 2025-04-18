void onEnter_IDLE(){

}
void onRun_IDLE(){
  /*Serial.printf("lT:%d , RT:%d\n",
    receivedData.left_trigger_value,
    receivedData.right_trigger_value
  );*/
  Serial.println(
    strcmp(receivedData.B_button_Event, "long_press_start") == 0
  );
}
void onExit_IDLE(){
  
}