const byte gaz_pin = 35;

void setup_gaz(){
  pinMode(gaz_pin, INPUT);
}
void monitoring_gaz_datas(void *arg){
  for(;;){
      data.gaz_lecture = analogRead(gaz_pin);
      delay(10);
  }
}
void CreateTasksForGaz(){
  xTaskCreatePinnedToCore(
    monitoring_gaz_datas,"Mettre à jour la valeur des gaz", 2048
    ,
    NULL // Stack Depth
    ,
    1 //Priority
    ,
    NULL //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}