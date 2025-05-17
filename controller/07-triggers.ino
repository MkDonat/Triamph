//Triggers parameters
typedef struct{
  const char* name;
  const uint8_t trigger_pin;
  uint16_t* lecture;
}TriggersParameters;

//Left trigger
TaskHandle_t xTask_left_trigger_Handle = NULL;
TriggersParameters left_trigger_params = {
  .name = "left_trigger",
  .trigger_pin = left_trigger_pin,
  .lecture = &SendingData.left_trigger_value
};

//Right trigger
TaskHandle_t xTask_right_trigger_Handle = NULL;
TriggersParameters right_trigger_params = {
  .name = "right_trigger",
  .trigger_pin = right_trigger_pin,
  .lecture = &SendingData.right_trigger_value
};

void vTask_Trigger(void *args){

  // Vérifie si le pointeur vers la structure est nul
  if (args == nullptr) {
    Serial.println("Trigger : paramètre nul !");
    vTaskDelete(NULL);
  }
  // On récupère la structure de données
  TriggersParameters* params = (TriggersParameters*) args;
  // On récupère les paramètres
  const char* name = params->name;
  const uint8_t trigger_pin = params->trigger_pin;
  uint16_t* lecture = params->lecture;

  for(;;){
    analogRead(trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    *lecture = analogRead(trigger_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    //Serial.printf("%s lecture : %d\n", name, *lecture);
  }
}
