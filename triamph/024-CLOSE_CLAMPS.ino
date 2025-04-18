void onEnter_CLOSING_CLAMPS(){
  Serial.println("Fermeture...");

  static uint16_t target_pos = PinceFerme;
  xTaskCreatePinnedToCore(
    vTask_Opening_or_Closing_Clamps, "Fermeture", 1000,
    (void*) &target_pos,
    1,
    &xTask_closed_clamps_Handle,
    CORE_2
  );
}

void onRun_CLOSING_CLAMPS(){}

void onExit_CLOSING_CLAMPS(){
  is_button_clicked_OC_CLAMPS = false;
  vTaskDelete(xTask_closed_clamps_Handle);
}