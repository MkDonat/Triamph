/*typedef struct {
  byte x_pin = 32; // input only + ADC_1 pin
  byte y_pin = 35; // input only + ADC_1 pin (VN)
  int16_t x_raw_datas;
  int16_t y_raw_datas;
  int16_t x_fixed_datas;
  int16_t y_fixed_datas;
  int8_t  *tor_x = &SendingData.joystick_tor_x;
  int8_t  *tor_y = &SendingData.joystick_tor_y;
  const int16_t x_err = 600;
  const int16_t y_err = 600;
  const int16_t max_analog_lecture = 4095; // 12-bits
  int16_t x_offset = -238; // décallage du au vieillissement
  int16_t y_offset = -138; // décallage du au vieillissement
  bool printOnSerial = true;
} JoystickDatas;
JoystickDatas joystick;

void vTaskGetJoystickInputs(void *arg){

  for(;;){

    analogRead(joystick.x_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    joystick.x_raw_datas = analogRead(joystick.x_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    analogRead(joystick.y_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    joystick.y_raw_datas = analogRead(joystick.y_pin);

    joystick.x_fixed_datas = joystick.x_raw_datas - joystick.x_offset;
    joystick.y_fixed_datas = joystick.y_raw_datas - joystick.y_offset;
    if(joystick.x_fixed_datas > joystick.max_analog_lecture/2 + joystick.x_err){
      *joystick.tor_x = 1;
    }
    else if(joystick.x_fixed_datas < joystick.max_analog_lecture/2 - joystick.x_err){
      *joystick.tor_x = -1;
    }
    else{
      *joystick.tor_x = 0;
    }
    if(joystick.y_fixed_datas > joystick.max_analog_lecture/2 + joystick.y_err){
      *joystick.tor_y = 1;
    }
    else if(joystick.y_fixed_datas < joystick.max_analog_lecture/2 - joystick.y_err){
      *joystick.tor_y = -1;
    }
    else{
      *joystick.tor_y = 0;
    }          
    //print to serial
    if(joystick.printOnSerial){
      Serial.printf(
        "raw_x: %d , x: %d , raw_y: %d , y: %d , tor -> x:%d , y:%d\n"
        ,joystick.x_raw_datas
        ,joystick.x_fixed_datas
        ,joystick.y_raw_datas
        ,joystick.y_fixed_datas
        ,*joystick.tor_x
        ,*joystick.tor_y
      );
    }
    //Delay
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
void CreateTasksForJoystick(){
  xTaskCreatePinnedToCore(
    vTaskGetJoystickInputs,"Getting Joysticks inputs"
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
*/
