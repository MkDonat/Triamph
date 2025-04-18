uint16_t LU_Loaded = 70;
uint16_t LU_Unloaded = 110;

TaskHandle_t xTask_LU_Handle = NULL;

uint16_t vitesse = 1; // 1 = rapide, 10 = lent

void vTask_Loading_or_Unloading_Trash(void* position_LU){
  uint16_t *targetPose_LU = (uint16_t *) position_LU;

  int currentPose_LU = servo_LU_droit.read();

  while(1){
    if (currentPose_LU < *targetPose_LU) currentPose_LU++;
    else if (currentPose_LU > *targetPose_LU) currentPose_LU--;

    servo_LU_droit.write(currentPose_LU);
    servo_LU_gauche.write(currentPose_LU);
    // Pause proportionnelle à la vitesse
    vTaskDelay(pdMS_TO_TICKS(vitesse * 10));

    if (currentPose_LU == *targetPose_LU) {
      vTaskDelay(pdMS_TO_TICKS(200));
    }
  }
}