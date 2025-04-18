void onEnter_OC_CLAMPS(){
  int currentPose_OC_CLAMPS = servo_OC_CLAMPS_droit.read();
  static uint16_t targetPose_OC_CLAMPS;

  if (currentPose_OC_CLAMPS == OC_CLAMPS_Closed){
    Serial.println("Ouverture des Pinces");
    targetPose_OC_CLAMPS = OC_CLAMPS_Opened;
  }
  if (currentPose_OC_CLAMPS == OC_CLAMPS_Opened){
    Serial.println("Fermeture des Pinces");
    targetPose_OC_CLAMPS = OC_CLAMPS_Closed;
  }
  xTaskCreatePinnedToCore(
    vTask_Opening_or_Closing_Clamps, "Ouverture ou Fermeture", 1000,
    (void*) &targetPose_OC_CLAMPS,
    1,
    &xTask_OC_CLAMPS_Handle,
    CORE_2
  );
}
void onRun_OC_CLAMPS(){

}
void onExit_OC_CLAMPS(){
  is_button_clicked_OC_CLAMPS = false;
  vTaskDelete(xTask_OC_CLAMPS_Handle);
}