void onEnter_LOAD_TRASH(){
  Serial.println("Chargement...");

  static uint16_t target_pos = BacCharge;
  xTaskCreatePinnedToCore(
    vTask_Loading_or_Unloading_Trash, "Load", 1000, 
    (void*) &target_pos,
    1,
    &xTask_loaded_trash_Handle,
    CORE_2
  );
}

void onRun_LOAD_TRASH(){}

void onExit_LOAD_TRASH(){
  is_button_clicked_LU = false;
  if (xTask_loaded_trash_Handle != NULL) {
    vTaskDelete(xTask_loaded_trash_Handle);
    xTask_loaded_trash_Handle = NULL;
  }
}