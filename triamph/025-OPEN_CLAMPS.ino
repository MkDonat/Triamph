void onEnter_OPENING_CLAMPS(){
  Serial.println("Ouverture...");

  static uint16_t target_pos = PinceOuverte;
  xTaskCreatePinnedToCore(
    vTask_Opening_or_Closing_Clamps, "Ouverture", 1000,
    (void*) &target_pos,
    1,
    &xTask_opened_clamps_Handle,
    CORE_2
  );
}

void onRun_OPENING_CLAMPS(){}

void onExit_OPENING_CLAMPS(){
  is_button_clicked_OC_CLAMPS = false;
  vTaskDelete(xTask_opened_clamps_Handle);
}