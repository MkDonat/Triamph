/*void onEnter_NORMAL(){
  if(xTask_handling_left_trigger_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_handling_left_trigger
      ,
      "Gestion des données du trigger gauche"
      ,
      1000 // Stack depth
      ,
      NULL // pvParameters
      ,
      1 //Priority
      ,
      &xTask_handling_left_trigger_Handle //Task handle
      ,
      CORE_2 // Core on which the task will run
    );
  }
  if(xTask_handling_right_trigger_Handle == NULL){
    xTaskCreatePinnedToCore(
      vTask_handling_right_trigger
      ,
      "Gestion des données du trigger droit"
      ,
      1000 // Stack depth
      ,
      NULL // pvParameters
      ,
      1 //Priority
      ,
      &xTask_handling_right_trigger_Handle //Task handle
      ,
      CORE_2 // Core on which the task will run
    );
  }
  
}
void onRun_NORMAL(){
  Serial.println("OK");
}
void onExit_NORMAL(){
  
}