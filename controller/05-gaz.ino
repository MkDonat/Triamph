typedef struct{
  const uint8_t pin = 35;
  uint16_t value;
  bool printOnSerial = false;
}GAZ;
GAZ gaz;

void monitoring_gaz_datas(void *arg){
  pinMode(gaz.pin, INPUT);
  for(;;){
    gaz.value = analogRead(gaz.pin);
    //Update to sending data
    SendingData.gaz_value = gaz.value;
    //Print on Serial
    if (gaz.printOnSerial){
      printf(
        "Gaz: %ld\n"
        ,
        gaz.value
      );
    }
    //Delay
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
void CreateTasksForGaz(){
  xTaskCreatePinnedToCore(
    monitoring_gaz_datas, "Update gaz datas"
    ,
     2048
    ,
    NULL // Stack Depth
    ,
    1 //Priority
    ,
    NULL //Task handle
    ,
    CORE_1 // Core on which the task will run
  );
}