void onEnter_LU(){
  //Console info
  Serial.println("Entring LOAD/UNLOAD STATE");
  //Attach-servos
  if(!ds3218_droit.attached()){
    ds3218_droit.attach(ds3218_droit_pin);
  }
  if(!ds3218_gauche.attached()){
    ds3218_gauche.attach(ds3218_gauche_pin);
  }
  if(ds3218_droit.attached() && ds3218_gauche.attached()){
    Serial.println("Servos ds3218 attached");
  }else{
    Serial.println("Failed to attach ds3218 servos");
    is_LoadUnload_task_complete = true;
    return;
  }
  //Creation d’un timer pour la tâche
  one_shot_timer_start(
    "Timer Load/Unload Trash",//Description 
    pdMS_TO_TICKS(LoadUnload_Time),//Ticks To Wait 
    &xTask_LoadUnload_TimerHandler,//Handler
    xTask_LoadUnload_TimerCallback //Callback
  );
  
  //Task creation
  if(xTask_LoadUnload_RightServo_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTaskOperateServos,
      "Levage, servo droit",
      2048, //stack in words (not bytes)
      &right_servo_levage_params, //arguments
      1, // priority
      &xTask_LoadUnload_RightServo_Handle,
      CORE_2
    );
  }
  
  if(xTask_LoadUnload_LeftServo_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTaskOperateServos,
      "Levage, servo gauche",
      2048, //stack in words (not bytes)
      &left_servo_levage_params, //arguments
      1, // priority
      &xTask_LoadUnload_LeftServo_Handle,
      CORE_2
    );
  }
}
void onRun_LU(){
  save_datas_to_flash();
  vTaskDelay(pdMS_TO_TICKS(100));
}
void onExit_LU(){
  //stop and delete timer
  if (xTask_LoadUnload_TimerHandler != NULL) {
    if (xTimerStop(xTask_LoadUnload_TimerHandler, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("LU STATE: xTimerStop a échoué ou timer déjà arrêté.");
    }
    xTimerDelete(xTask_LoadUnload_TimerHandler, pdMS_TO_TICKS(100));
    xTask_LoadUnload_TimerHandler = NULL;
  }
  //Deleting tasks
  if(xTask_LoadUnload_RightServo_Handle != NULL){
    vTaskDelete(xTask_LoadUnload_RightServo_Handle);
    xTask_LoadUnload_RightServo_Handle = NULL;
  }
  if(xTask_LoadUnload_LeftServo_Handle != NULL){
    vTaskDelete(xTask_LoadUnload_LeftServo_Handle);
    xTask_LoadUnload_LeftServo_Handle = NULL;
  }
  //Detach-servos
  if(ds3218_droit.attached()){
     ds3218_droit.detach();
  }
  if(ds3218_gauche.attached()){
     ds3218_gauche.detach();
  }
  if(ds3218_droit.attached() && ds3218_gauche.attached()){
    Serial.println("Failed to detach ds3218 servos");
  }else{
    Serial.println("Servos ds3218 detached");
  }
  //Updating vars
  is_LoadUnload_task_complete = false;
  //Console info
  Serial.println("Exiting LOAD/UNLOAD STATE");
}