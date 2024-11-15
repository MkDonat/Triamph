const byte potentio_pin = 36;
uint32_t lecture_potentio;
ulong lecture_potentio_mapped;
const byte err_lecture = 15;

void potentio(void *arg){
  pinMode(potentio_pin, INPUT);
  while(1){
    lecture_potentio = analogRead(potentio_pin);
    //Formattage de la lecture en multiple entier de l’erreur
    //i.e lecture = n * err_lecture (avec n appartien à N)
    lecture_potentio = (lecture_potentio/err_lecture)*err_lecture;
    //Mapping
    lecture_potentio_mapped = map(lecture_potentio,0,4095,0,179);
    Serial.println(lecture_potentio);
    delay(100);
  }
}

void CreateTasksForPotentio(){
  xTaskCreatePinnedToCore(
    potentio, "Lecture du potentio", 2048
    ,
    NULL // Stack Depth
    ,
    2 //Priority
    ,
    NULL //Task handle
    ,
    CORE_1 // Core on which the task will run
  );
}