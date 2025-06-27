//0x40 

void ina219_setup(){

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  //Serial.println("Measuring voltage and current with INA219 ...");

}

void ina219_task(void *args){

  for(;;){
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;
    float battery_percentage = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    battery_percentage = nine_volt_battery_percentage(busvoltage);

    //Update global var
    if(power_mW >= 980 && power_mW <= 2000){
      if(current_mA >= 111 && current_mA < 130){
        if(battery_percentage>0){
          battery_level_percentage = battery_percentage;
        }
      }
    }
      
    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.print("Percentage:    "); Serial.print(battery_percentage); Serial.println(" %");
    Serial.println("");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

}

float nine_volt_battery_percentage(float voltage){

  const int dataSize = 9;
  float voltages[dataSize] =     {  9.0 , 8.5  , 8.0   , 7.5   , 7.0  , 6.5   , 6.0  , 5.5  , 5.0 };
  float serviceHours[dataSize] = {  0   , 0.25 , 0.56  , 1.03  , 1.76 , 2.70  , 3.11 , 3.41 , 3.45 };

  float voltage_service_hour = 0.0;
  float low_voltage_service_hour = serviceHours[5];// corresponding voltage 7.0 volts

  if (voltage >= voltages[0]) return 100.0;
  if (voltage <= voltages[dataSize - 1]) return 0.0;

  for (int i = 0; i < dataSize - 1; i++) {
    if (voltage <= voltages[i] && voltage > voltages[i + 1]){
      float v1 = voltages[i];
      float v2 = voltages[i+1];
      float t1 = serviceHours[i];
      float t2 = serviceHours[i+1];
      float voltage_service_hour = t1 + (voltage - v1) * (t2 - t1) / (v2 - v1);
      // battery percentage
      return (((low_voltage_service_hour-voltage_service_hour)/low_voltage_service_hour)*100.0);
    }
  }

}

void create_task_for_ina219(){

  xTaskCreatePinnedToCore(
    ina219_task // task function
    ,
    "ina219_task" //task name
    ,
    2048 // stack in word (not bytes)
    ,
    NULL // params
    ,
    1 // priority
    ,
    NULL // Task Handle
    ,
    CORE_2 // Core on which the task will run
  );

}