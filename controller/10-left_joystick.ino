//Left joystick
typedef struct {
  uint8_t *x_pin = &left_joystick_pin_x;
  uint8_t *y_pin = &left_joystick_pin_y;
  int16_t x_raw_datas;
  int16_t y_raw_datas;
  int16_t *x_fixed_datas = &SendingData.L_J_analog_x;
  int16_t *y_fixed_datas = &SendingData.L_J_analog_y;
  int16_t rest_point_x_data;
  int16_t rest_point_y_data;
  int8_t  *tor_x = &SendingData.L_J_tor_x;
  int8_t  *tor_y = &SendingData.L_J_tor_y;
  int16_t x_err = 50;
  int16_t y_err = 50;
  const int16_t input_resolution = 12;
  const int16_t output_resolution = 12;
  bool printOnSerial = true;
} LeftJoystickDatas;
LeftJoystickDatas L_J;

uint16_t fix_raw_datas(uint16_t raw_datas, uint16_t rest_datas, uint16_t error, uint8_t input_resolution, uint8_t output_resolution){
  
  uint16_t input_max = pow(2, input_resolution) - 1;
  uint16_t input_min = 0;
  uint16_t input_mid = (int)trunc((input_max + input_min)/2);

  uint16_t output_max = pow(2, output_resolution) - 1;
  uint16_t output_min = 0;
  uint16_t output_mid = (int)trunc((output_max + output_min)/2);

  if(raw_datas >= input_min && raw_datas < rest_datas - error){

    return map(
      raw_datas, input_min, rest_datas
      ,
      output_min, output_mid
    );

  }else if(raw_datas <= input_max && raw_datas >= rest_datas + error){

    return map(
      raw_datas, rest_datas, input_max
      ,
      output_mid, output_max
    );

  }else return output_mid;
}

uint8_t tor_joystick_output(uint16_t data, uint8_t resolution, int16_t error){

  uint16_t max = pow(2, resolution);
  uint16_t min = 0;
  uint16_t mid = (int)trunc((max+min)/2);

  if(data > mid + error){

    return 1;

  }
  else if(data < mid - error){

    return -1;

  }else 

    return 0;

}

void vTaskLeftJoystickInputs(void *arg){

  //-----Auto Calibration-----//
    //->TO DO...

  for(;;){

    L_J.x_raw_datas = analogRead(*L_J.x_pin);
    vTaskDelay(pdMS_TO_TICKS(10));
    L_J.y_raw_datas = analogRead(*L_J.y_pin);

    //-----Fixing joystick raw datas-----
    *L_J.x_fixed_datas = fix_raw_datas(L_J.x_raw_datas, 1723/*L_J.rest_point_x_data*/, L_J.x_err, L_J.input_resolution, L_J.output_resolution);
    *L_J.y_fixed_datas = fix_raw_datas(L_J.y_raw_datas, 1813/*L_J.rest_point_y_data*/, L_J.y_err, L_J.input_resolution, L_J.output_resolution);

    //-----TOR output-----
    *L_J.tor_x = tor_joystick_output(*L_J.x_fixed_datas, L_J.output_resolution, L_J.x_err);
    *L_J.tor_y = tor_joystick_output(*L_J.y_fixed_datas, L_J.output_resolution, L_J.y_err);

    //-----print to serial-----
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

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
void CreateTasksForLeftJoystick(){
  xTaskCreatePinnedToCore(
    vTaskLeftJoystickInputs,"Getting Left Joystick inputs"
    ,
     2048
    ,
    NULL // args
    ,
    1 //Priority
    ,
    &xTask_left_joystick_Handle //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}

