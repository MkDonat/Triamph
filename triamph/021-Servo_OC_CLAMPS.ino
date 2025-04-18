uint16_t PinceFerme = 30;
uint16_t PinceOuverte = 150;
TaskHandle_t xTask_closed_clamps_Handle = NULL;
TaskHandle_t xTask_opened_clamps_Handle = NULL;

void vTask_Opening_or_Closing_Clamps(void* position_OC_CLAMPS){
  uint16_t *pose_OC_CLAMPS = (uint16_t *) position_OC_CLAMPS;

  while (1) {
    servopincedroite.write(*pose_OC_CLAMPS);
    servopincegauche.write(*pose_OC_CLAMPS);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for visibility/stability
  }
}