void onEnter_UNLOAD_TRASH(){
  Serial.println("Déchargement...");

  static uint16_t target_pos = BacDecharge;
  xTaskCreatePinnedToCore(
    vTask_Loading_or_Unloading_Trash, "Unload", 1000,
    (void*) &target_pos,
    1,
    &xTask_unloaded_trash_Handle,
    CORE_2
  );
}

void onRun_UNLOAD_TRASH(){}

void onExit_UNLOAD_TRASH(){
  is_button_clicked_LU = false;
  if (xTask_unloaded_trash_Handle != NULL) {
    vTaskDelete(xTask_unloaded_trash_Handle);
    xTask_unloaded_trash_Handle = NULL;
  }
}