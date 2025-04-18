void onEnter_IDLE(){

}
void onRun_IDLE(){
  Serial.printf("lT:%d , RT:%d\n",
    receivedData.left_trigger_value,
    receivedData.right_trigger_value
  );
}
void onExit_IDLE(){
  
}