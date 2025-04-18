uint16_t OC_CLAMPS_Closed = 30;
uint16_t OC_CLAMPS_Opened = 150;
TaskHandle_t xTask_OC_CLAMPS_Handle = NULL;

void vTask_Opening_or_Closing_Clamps(void* position_OC_CLAMPS){
  uint16_t *pose_OC_CLAMPS = (uint16_t *) position_OC_CLAMPS;

  int currentPose_OC_CLAMPS = servo_OC_CLAMPS_droit.read();

  while (1) {
    servo_OC_CLAMPS_droit.write(*pose_OC_CLAMPS);
    servo_OC_CLAMPS_gauche.write(*pose_OC_CLAMPS);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for visibility/stability
  }
}