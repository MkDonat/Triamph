void onEnter_OC_CLAMPS(){
  Serial.println("Entring OC_CLAMPS STATE");
  one_shot_timer_start(
    "Timer open/close clamps",//Description 
    pdMS_TO_TICKS(7000),//Ticks To Wait 
    &xTask_OC_TimerHandler,//Handler
    xTask_OC_TimerCallback //Callback
  );
  if(xTask_OC_Clamps_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_OC_clamps,
      "Ouverture/Fermeture des pinces",  //description
      2048, //stack in words (not bytes)
      NULL, //args
      1, // priority
      &xTask_OC_Clamps_Handle,
      CORE_2
    );
  }
}
void onRun_OC_CLAMPS(){

}
void onExit_OC_CLAMPS(){
  xTimerStop(xTask_OC_TimerHandler,5);
  Serial.println("Exiting OC_CLAMPS STATE");
  is_OC_Clamps_task_complete = false;
  if(xTask_OC_Clamps_Handle != NULL){
    vTaskDelete(xTask_OC_Clamps_Handle);
    xTask_OC_Clamps_Handle = NULL;
  }
}