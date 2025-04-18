uint16_t BacCharge = 70;
uint16_t BacDecharge = 110;

TaskHandle_t xTask_loaded_trash_Handle = NULL;
TaskHandle_t xTask_unloaded_trash_Handle = NULL;

uint16_t vitesse = 1; // 1 = rapide, 10 = lent

void vTask_Loading_or_Unloading_Trash(void* position_LU){
  uint16_t *targetPose = (uint16_t *) position_LU;

  int currentPose = servodroite.read();

  while(1){
    if (currentPose < *targetPose) currentPose++;
    else if (currentPose > *targetPose) currentPose--;

    servodroite.write(currentPose);
    servogauche.write(currentPose);
    // Pause proportionnelle à la vitesse
    vTaskDelay(pdMS_TO_TICKS(vitesse * 10));

    if (currentPose == *targetPose) {
      vTaskDelay(pdMS_TO_TICKS(200));
    }
  }
}