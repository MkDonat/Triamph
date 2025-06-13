#include "Adafruit_VL53L0X.h"

// I2C address
#define LOXWaste_ADDRESS 0x35

// XSHUT pin
#define SHT_LOXWaste 18

// Waste sensor
Adafruit_VL53L0X loxWaste;
VL53L0X_RangingMeasurementData_t measure_fill_value;

// Waste measurement variables
int captor_to_bottom = 260;
int bottom_to_top = 95;
int top_to_captor = captor_to_bottom - bottom_to_top;
float percentage_waste_max = 0.8;
float percentage_inverse = 1 - percentage_waste_max;
float threshold_to_top = percentage_inverse * bottom_to_top;
float threshold_to_captor = threshold_to_top + top_to_captor;

int measured_distance;
float fill_percentage = 0.0;
float max_fill_percentage = 0.0;
bool over_the_threshold = false;
bool has_been_over_the_threshold = false;
bool succesfull_sensor_waste = false;

void waste_sensor_setup(){
  Serial.println("Setting up sensors...");
  pinMode(SHT_LOXWaste, OUTPUT);
  setup_water_sensor();
  //activateWasteSensor();
}

// === Waste Sensor Functions ===
void activateWasteSensor() {
  digitalWrite(SHT_LOXWaste, HIGH);
  delay(10);

  if (loxWaste.begin(LOXWaste_ADDRESS)) {
    Serial.println("Waste sensor initialized successfully.");
    succesfull_sensor_waste = true;
  } else {
    Serial.println("Failed to initialize waste sensor.");
    succesfull_sensor_waste = false;
  }
}

void detectWasteForDuration(unsigned long durationMs) {
  if (!succesfull_sensor_waste) {
    Serial.println("Waste sensor not initialized. Cannot collect.");
    return;
  }

  unsigned long startTime = millis();
  int readingCount = 0;
  float fillSum = 0.0;

  while (millis() - startTime < durationMs) {
    loxWaste.rangingTest(&measure_fill_value, false);

    if (measure_fill_value.RangeStatus != 4) {
      measured_distance = measure_fill_value.RangeMilliMeter;

      float fill_distance = max(0, captor_to_bottom - measured_distance);
      fill_percentage = constrain((fill_distance / bottom_to_top) * 100.0, 0.0, 100.0);

      fillSum += fill_percentage;
      readingCount++;

      float average_fill_percentage = readingCount > 0 ? fillSum / readingCount : 0.0;
      over_the_threshold = average_fill_percentage >= (percentage_waste_max * 100);

      if (over_the_threshold) has_been_over_the_threshold = true;

      if (average_fill_percentage > max_fill_percentage)
        max_fill_percentage = average_fill_percentage;

      //Serial.print("Waste: ");
      //Serial.print(fill_percentage, 1);
      //Serial.println("% full");

    } else {
      Serial.println("Waste: Out of range");
    }

    delay(100);
  }

  // Use average instead of just max


  // Final logic
  Serial.print("Filled at: ");
  Serial.print(max_fill_percentage, 1);+
  Serial.println("%");

  if (!over_the_threshold) {
    if (!has_been_over_the_threshold) {
      Serial.println("Initiating collecting.");
    } else {
      Serial.println("has_been_over_the_threshold. Unable to collect.");
    }
  } else {
    has_been_over_the_threshold = true;
    Serial.println("over_the_threshold. Unable to collect.");
  }
}

void resetWasteAfterUnload() {
  max_fill_percentage = 0.0;
  has_been_over_the_threshold = false;
  over_the_threshold = false;
  Serial.println("Waste bin state reset after unloading.");
}


void Check_if_collecting_is_possible () {
    Serial.println("Starting waste collection simulation...");

    if (!succesfull_sensor_waste) {
      Serial.println("Waste sensor not initialized. Cannot collect.");
      return;
    }

    detectWasteForDuration(1000);  // 1 seconds of measurement
}

void Check_if_unloading_is_possible () {
  Serial.println("Attempting to unload...");

    if (*is_on_water) {
      Serial.println("Cannot unload while in water.");
    }
   else if (!sensorPresent){
      Serial.println("Cannot unload while water sensor is not connected");
    }
    else {
    Serial.print("Triamph was: ");
    Serial.print(max_fill_percentage, 1);
    Serial.println("% full"); 
    Serial.println("Unloading Triamph...");
    resetWasteAfterUnload();
    }
}