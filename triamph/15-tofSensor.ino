#include <VL53L0X.h>
#include "Wire.h"
typedef struct{
  const uint8_t sda_pin = 4;
  const uint8_t scl_pin = 17;
  uint16_t *singleMillimetersValue = &SendingData.tofSensorData_singleMillimetersValue;
}tofSensorDatas;
tofSensorDatas tofSensorData;

VL53L0X tofSensor;

void vTaskToFSensor(void *arg){
  Wire.begin(
    tofSensorData.sda_pin,//SDA
    tofSensorData.scl_pin //SCL
    ); 
  //Serial.begin(115200);
  if(tofSensor.init()!= true){
    Serial.print("Failed init");
  }
  for(;;){
    uint16_t sensorValue;
    *tofSensorData.singleMillimetersValue = tofSensor.readRangeSingleMillimeters();
    //tofSensor.timeoutOccurred();
    //Serial.printf("Value: %d \n",sensorValue);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void CreateTasksForTofSensor(){
  xTaskCreatePinnedToCore( 
    vTaskToFSensor
    ,
    "TOFSENSOR"
    ,
    2048
    ,
    NULL
    ,
    2
    ,
    NULL
    ,
    CORE_1
  );
}

  
