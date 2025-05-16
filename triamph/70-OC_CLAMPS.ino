void onEnter_OC_CLAMPS(){
  Serial.println("Entring OC_CLAMPS STATE");
  one_shot_timer_start(
    "Timer open/close clamps",//Description 
    pdMS_TO_TICKS(5000),//Ticks To Wait 
    &xTask_OC_TimerHandler,//Handler
    xTask_OC_TimerCallback //Callback
  );
  if(xTask_OC_right_Clamp_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_OC_clamp,
      "Ouverture/Fermeture de la pince droite",  //description
      2048, //stack in words (not bytes)
      &right_clamp_params, //params
      1, // priority
      &xTask_OC_right_Clamp_Handle,
      CORE_2
    );
  }
  if(xTask_OC_left_Clamp_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_OC_clamp,
      "Ouverture/Fermeture de la pince gauche",
      2048, //stack in words (not bytes)
      &left_clamp_params, //params
      1, // priority
      &xTask_OC_left_Clamp_Handle,
      CORE_2
    );
  }
}
void onRun_OC_CLAMPS(){

}
void onExit_OC_CLAMPS(){
  if (xTask_OC_TimerHandler != NULL) {
    if (xTimerStop(xTask_OC_TimerHandler, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("xTimerStop a échoué ou timer déjà arrêté.");
    }
    xTimerDelete(xTask_OC_TimerHandler, pdMS_TO_TICKS(100));
    xTask_OC_TimerHandler = NULL;
  }
  Serial.println("Exiting OC_CLAMPS STATE");
  is_OC_Clamps_task_complete = false;
  if(xTask_OC_right_Clamp_Handle != NULL){
    vTaskDelete(xTask_OC_right_Clamp_Handle);
    xTask_OC_right_Clamp_Handle = NULL;
  }
  if(xTask_OC_left_Clamp_Handle != NULL){
    vTaskDelete(xTask_OC_left_Clamp_Handle);
    xTask_OC_left_Clamp_Handle = NULL;
  }
}