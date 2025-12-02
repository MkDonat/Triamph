/*
  This program read a serilized JSON data formatted as this:
  '{"key":value};'
  The separator ';' aims to tell the pogram he should stop reading
  because the data is complete.
*/

void vTaskSerialJSONReading(void *args){
  // One time code
  for(;;){
    static String message = "";
    while (Serial.available() > 0) {   // as long as there is a characters
      char c = Serial.read();
      if (c == ';') {  // if the separator is detected
        //Serial.println(message);
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
          Serial.print("deserializeJson() returned ");
          Serial.println(error.c_str());
          message = "";
          continue;
        }
        JsonObject obj = doc.as<JsonObject>();
        for (JsonPair kv : obj) {
          const char* key = kv.key().c_str();
          int value = kv.value().as<int>();
          gamepad_datas[key] = value;
        }
        message = "";        
      } 
      else {
        message += c; 
      }
    }
  }
}

bool createTaskForSerialJSONReading(){
  if(
      (
        xTaskCreatePinnedToCore(
          vTaskSerialJSONReading
          ,
          "Readind serial datas"
          ,
          2048 // stack size in words
          ,
          NULL // args
          ,
          1 //Priority
          ,
          NULL //Task handle
          ,
          CORE_2 // Core on which the task will run
        )
      ) != NULL
  ){
    return true;
  }else{
    return false;
  }
}