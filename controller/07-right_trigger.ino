/*TaskHandle_t xTask_handling_right_trigger_Handle = NULL;
uint32_t* right_trigger_value = &SendingData.right_trigger_value;

void vTask_handling_right_trigger(void *arg){
  for(;;){
    analogRead(right_trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    *right_trigger_value = analogRead(right_trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
