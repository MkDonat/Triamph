typedef struct {
  byte x_pin = 34; // input only + ADC_1 pin
  byte y_pin = 39; // input only + ADC_1 pin (VN)
  int16_t x_raw_datas;
  int16_t y_raw_datas;
  int16_t x_fixed_datas;
  int16_t y_fixed_datas;
  int8_t  tor_x;
  int8_t  tor_y;
  const int16_t x_err = 600;
  const int16_t y_err = 600;
  const int16_t max_analog_lecture = 4095; // 12-bits
  int16_t x_offset = -238; // décallage du au vieillissement
  int16_t y_offset = -138; // décallage du au vieillissement
  bool printOnSerial = true;
} JoystickDatas;
JoystickDatas joystick;

void joystick_computations(void *arg){
  pinMode(joystick.x_pin, INPUT);
  pinMode(joystick.y_pin, INPUT);
  for(;;){
    joystick.x_raw_datas = analogRead(joystick.x_pin);
    joystick.y_raw_datas = analogRead(joystick.y_pin);
    joystick.x_fixed_datas = joystick.x_raw_datas - joystick.x_offset;
    joystick.y_fixed_datas = joystick.y_raw_datas - joystick.y_offset;
    if(joystick.x_fixed_datas > joystick.max_analog_lecture/2 + joystick.x_err){
      joystick.tor_x = 1;
    }
    else if(joystick.x_fixed_datas < joystick.max_analog_lecture/2 - joystick.x_err){
      joystick.tor_x = -1;
    }
    else{
      joystick.tor_x = 0;
    }
    if(joystick.y_fixed_datas > joystick.max_analog_lecture/2 + joystick.y_err){
      joystick.tor_y = 1;
    }
    else if(joystick.y_fixed_datas < joystick.max_analog_lecture/2 - joystick.y_err){
      joystick.tor_y = -1;
    }
    else{
      joystick.tor_y = 0;
    }
    //update datas to send
    SendingData.joystick_x_value = joystick.tor_x;
    SendingData.joystick_x_value = joystick.tor_x;            
    //print to serial
    if(joystick.printOnSerial){
      Serial.printf(
        "raw_x: %ld , x: %ld , raw_y: %ld , y: %ld , tor -> x:%ld , y:%ld\n"
        ,joystick.x_raw_datas
        ,joystick.x_fixed_datas
        ,joystick.y_raw_datas
        ,joystick.y_fixed_datas
        ,joystick.tor_x
        ,joystick.tor_y
      );
    }
    //Delay
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void CreateTasksForJoystick(){
  xTaskCreatePinnedToCore(
    joystick_computations,"computing joystick datas"
    ,
     2048
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
