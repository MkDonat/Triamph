/*TaskHandle_t xTask_handling_left_trigger_Handle = NULL;
uint32_t* left_trigger_value = &SendingData.left_trigger_value;

void vTask_handling_left_trigger(void *arg){
  for(;;){
    analogRead(left_trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    *left_trigger_value = analogRead(left_trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}*/
