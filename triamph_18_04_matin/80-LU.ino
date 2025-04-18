void onEnter_LU(){
  int currentPose_LU = servo_LU_droit.read();
  static uint16_t targetPose_LU;
  if (is_in_water == true){ // is_in_water true si dans l'eau, false sinon
    Serial.println("Système dans l'eau, déchargement impossible");
    targetPose_LU = LU_Loaded;
  }
  else if (currentPose_LU == LU_Loaded && is_in_water == false) {
    Serial.println("Déchargement du bac");
    targetPose_LU = LU_Unloaded;
  }
  else if (currentPose_LU == LU_Unloaded && is_in_water == false){
    Serial.println("Chargement du bac");
    targetPose_LU = LU_Loaded;
  }
  xTaskCreatePinnedToCore(
      vTask_Loading_or_Unloading_Trash, "Chargement ou Déchargement", 1000,
      (void*) &targetPose_LU,
      1,
      &xTask_LU_Handle,
      CORE_2
    );
}
void onRun_LU(){

}
void onExit_LU(){
  is_button_clicked_LU = false;
  if (xTask_LU_Handle != NULL) {
    vTaskDelete(xTask_LU_Handle);
    xTask_LU_Handle = NULL;
  }
}