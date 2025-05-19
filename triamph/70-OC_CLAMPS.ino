void onEnter_OC_CLAMPS(){
  Serial.println("Entring OC_CLAMPS STATE");
  //Attach sg90 servos
  if(!sg90_droit.attached()){
    sg90_droit.attach(sg90_droit_pin);
  }
  if(!sg90_gauche.attached()){
    sg90_gauche.attach(sg90_gauche_pin);
  }
  if(sg90_droit.attached() && sg90_gauche.attached()){
    Serial.println("Servos sg90 attached");
  }else{
    Serial.println("Failed to attach sg90 servos");
    is_OC_Clamps_task_complete = false;
    return;
  }
  one_shot_timer_start(
    "Timer open/close clamps",
    pdMS_TO_TICKS(5000),//Ticks To Wait 
    &xTask_OC_TimerHandler,//Handler
    xTask_OC_TimerCallback //Callback
  );
  if(xTask_OC_right_Clamp_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_OC_clamp,
      "Ouverture/Fermeture de la pince droite",
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
  //stop and delete timer
  if (xTask_OC_TimerHandler != NULL) {
    if (xTimerStop(xTask_OC_TimerHandler, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("OC_CLAMPS: xTimerStop a échoué ou timer déjà arrêté.");
    }
    xTimerDelete(xTask_OC_TimerHandler, pdMS_TO_TICKS(100));
    xTask_OC_TimerHandler = NULL;
  }
  //Deleting tasks
  if(xTask_OC_right_Clamp_Handle != NULL){
    vTaskDelete(xTask_OC_right_Clamp_Handle);
    xTask_OC_right_Clamp_Handle = NULL;
  }
  if(xTask_OC_left_Clamp_Handle != NULL){
    vTaskDelete(xTask_OC_left_Clamp_Handle);
    xTask_OC_left_Clamp_Handle = NULL;
  }
  //Detach sg90 servos
  if(sg90_droit.attached()){
    sg90_droit.detach();
  }
  if(sg90_gauche.attached()){
    sg90_gauche.detach();
  }
  if(sg90_droit.attached() && sg90_gauche.attached()){
    Serial.println("Failed to Detach sg90 servos");
  }else{
    Serial.println("Servos sg90 detached");
  }
  //updating vars
  is_OC_Clamps_task_complete = false;
  Serial.println("Exiting OC_CLAMPS STATE");
}