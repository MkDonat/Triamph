typedef struct {
  uint8_t x_pin = 35;
  uint8_t y_pin = 34;
  int16_t x_raw_datas;
  int16_t y_raw_datas;
  int16_t *x_fixed_datas = &SendingData.L_J_analog_x;
  int16_t *y_fixed_datas = &SendingData.L_J_analog_y;
  int8_t  *tor_x = &SendingData.L_J_tor_x;
  int8_t  *tor_y = &SendingData.L_J_tor_y;
  const int16_t x_err = 300;
  const int16_t y_err = 300;
  const int16_t max_analog_lecture = 4095; // 12-bits
  int16_t x_offset = -224; // décallage du au vieillissement
  int16_t y_offset = -127; // décallage du au vieillissement
  bool printOnSerial = true;
} LeftJoystickDatas;
LeftJoystickDatas L_J;

TaskHandle_t xTask_left_joystick_Handle = NULL;

void vTaskLeftJoystickInputs(void *arg){

  for(;;){
    analogRead(L_J.x_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    L_J.x_raw_datas = analogRead(L_J.x_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    analogRead(L_J.y_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    L_J.y_raw_datas = analogRead(L_J.y_pin);

    *L_J.x_fixed_datas = L_J.x_raw_datas-L_J.x_offset;
    *L_J.y_fixed_datas = L_J.y_raw_datas-L_J.y_offset;
    if(*L_J.x_fixed_datas > L_J.max_analog_lecture/2 + L_J.x_err){
      *L_J.tor_x = 1;
    }
    else if(*L_J.x_fixed_datas < L_J.max_analog_lecture/2 - L_J.x_err){
      *L_J.tor_x = -1;
    }
    else{
      *L_J.tor_x = 0;
    }
    if(*L_J.y_fixed_datas > L_J.max_analog_lecture/2 + L_J.y_err){
      *L_J.tor_y = 1;
    }
    else if(*L_J.y_fixed_datas < L_J.max_analog_lecture/2 - L_J.y_err){
      *L_J.tor_y = -1;
    }
    else{
      *L_J.tor_y = 0;
    }          
    //print to serial
    if(L_J.printOnSerial){
      Serial.printf(
        "raw_x: %d , x: %d , raw_y: %d , y: %d , tor -> x:%d , y:%d\n"
        ,L_J.x_raw_datas
        ,*L_J.x_fixed_datas
        ,L_J.y_raw_datas
        ,*L_J.y_fixed_datas
        ,*L_J.tor_x
        ,*L_J.tor_y
      );
    }
    //Delay
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
void CreateTasksForLeftJoystick(){
  xTaskCreatePinnedToCore(
    vTaskLeftJoystickInputs,"Getting Left Joystick inputs"
    ,
     2048
    ,
    NULL // Stack Depth
    ,
    1 //Priority
    ,
    &xTask_left_joystick_Handle //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}

