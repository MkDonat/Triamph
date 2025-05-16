void onEnter_COLLECTING(){
  //console info
  Serial.println("Enterring COLLECTING");
  //Start timer task
  one_shot_timer_start(
    "Collecting task timer",//Description
    pdMS_TO_TICKS(xTask_COLLECTING_length),//Ticks To Wait
    &xTask_COLLECTING_TimerHandler,//Handler
    xTask_COLLECTING_TimerCallback //Callback
  );
  //Creating task
  if(xTask_verin_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_verin,
      "Tache verin (COLLECTE)",
      2048, //stack in words (not bytes)
      NULL, //params
      1, // priority
      &xTask_verin_Handle,
      CORE_2
    );
  }
}
void onRun_COLLECTING(){
}
void onExit_COLLECTING(){
  //Stop and delete timer
  if (xTask_COLLECTING_TimerHandler != NULL) {
    if (xTimerStop(xTask_COLLECTING_TimerHandler, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("COLLECTING: xTimerStop a échoué ou timer déjà arrêté.");
    }
    xTimerDelete(xTask_COLLECTING_TimerHandler, pdMS_TO_TICKS(100));
    xTask_COLLECTING_TimerHandler = NULL;
  }
  //deleting task
  if(xTask_verin_Handle != NULL){
    vTaskDelete(xTask_verin_Handle);
    xTask_verin_Handle = NULL;
  }
  //Arrêt du verrin
  stop_verin();
  //updating vars
  Collecting_task_completed = false;
  Serial.println("Exiting COLLECTING");
}