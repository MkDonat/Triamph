typedef struct{
  const byte pin = 35;
  uint16_t value;
  bool printOnSerial = false;
}GAZ;
GAZ gaz;

void monitoring_gaz_datas(void *arg){
  pinMode(gaz.pin, INPUT);
  for(;;){
    gaz.value = analogRead(gaz.pin);
    //Update to sending data
    SendingData.gaz_lecture = gaz.value;
    //Print on Serial
    if (gaz.printOnSerial){
      printf(
        "Gaz: %ld\n"
        ,
        gaz.value
      );
    }
    //Delay
    vTaskDelay(10 / portTICK_PERIOD_MS);
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