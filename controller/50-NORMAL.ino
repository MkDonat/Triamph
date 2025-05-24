void onEnter_NORMAL(){
  //Creating task for right trigger
  if(xTask_right_trigger_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_Trigger
      ,
      "Tâche Trigger droit"
      ,
      2048 // Stack depth in words (not bytes)
      ,
      &right_trigger_params // pvParameters
      ,
      1 //Priority
      ,
      &xTask_right_trigger_Handle //Task handle
      ,
      CORE_2 // Core on which the task will run
    );
  }
  //Creating task for left trigger
  if(xTask_left_trigger_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_Trigger
      ,
      "Tâche Trigger gauche"
      ,
      2048 // Stack depth in words (not bytes)
      ,
      &left_trigger_params // pvParameters
      ,
      1 //Priority
      ,
      &xTask_left_trigger_Handle //Task handle
      ,
      CORE_2 // Core on which the task will run
    );
  }
  CreateTasksForLeftJoystick();
  CreateTasksForScreen();
}
void onRun_NORMAL(){
 Serial.println(SendingData.button_msg);
}
void onExit_NORMAL(){
  
}