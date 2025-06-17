bool is_on_water = false;

// === Water Sensor Functions ===
void setup_water_sensor() {
  pinMode(sensorPin, INPUT);
  delay(100);
  sensorValue = analogRead(sensorPin);

  if (sensorValue == max_adc_value) {
    sensorPresent = false;
    Serial.print("Water sensor not detected (value = ");
    Serial.print(sensorValue);
    Serial.println(").");
  } else {
    sensorPresent = true;
    Serial.print("Water sensor initialized (value = ");
    Serial.print(sensorValue);
    Serial.println(").");
  }
}

void operating_water_sensor(){
  if (!sensorPresent){
    waterSensorError = true;
    return;
  }

  sensorValue = analogRead(sensorPin);

  //Serial.println(SendingData.is_on_water);

  if (sensorValue == max_adc_value) {
    is_on_water = false;
    waterSensorError = true;
  } else if (sensorValue >= water_threshold) {
    is_on_water = true;
    waterSensorError = false;
  } else {
    is_on_water = false;
    waterSensorError = false;
  }

}