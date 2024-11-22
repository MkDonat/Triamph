// NE COMPREND PAS LA PINCE

#include <ESP32Servo.h>

// Servo Configuration
Servo servodroite;
Servo servogauche;
const int BacCharge = 70;
const int BacDecharge = 110;

// Pushbutton for charge
const int ChargebuttonPin = 13;
int ChargebuttonState = LOW;
int lastChargebuttonState = LOW;
unsigned long lastDebounceTimeCharge = 0;
unsigned long debounceDelayCharge = 10;
int chargeordischarge = 0;

void charge_decharge(void *arg) {
  servodroite.attach(15); // Servo 1 (GPIO 15)
  servogauche.attach(2);  // Servo 2 (GPIO 2)
  pinMode(ChargebuttonPin, INPUT_PULLUP);
  ChargebuttonState = LOW;
  lastChargebuttonState = LOW;
  chargeordischarge = 0;
  while (1) {
    int readingCharge = digitalRead(ChargebuttonPin);
    if (readingCharge != lastChargebuttonState) {
      lastDebounceTimeCharge = millis();
    }
    if ((millis() - lastDebounceTimeCharge) > debounceDelayCharge) {
      if (readingCharge != ChargebuttonState) {
        ChargebuttonState = readingCharge;
        if (ChargebuttonState == LOW) {
          chargeordischarge = chargeordischarge + 1;
          if ((chargeordischarge) % 2 == 1) {
            Serial.println("Chargement");
            servodroite.write(BacCharge);
            servogauche.write(BacCharge);
          } else if ((chargeordischarge) % 2 == 0) {
            Serial.println("Déchargement");
            servodroite.write(BacDecharge);
            servogauche.write(BacDecharge);
          }
        }
      }
    }
    lastChargebuttonState = readingCharge;
  }
}

// Create task for handling charge and discharge operations
void CreateTaskForChargement(){
  if (xTaskCreatePinnedToCore(
        charge_decharge,      // Task function
        "Charge/Discharge",   // Task name
        2048,                 // Stack size
        NULL,                 // Parameters
        1,                    // Priority
        NULL,                 // Task handle
        CORE_2                 // Core
      ) != pdPASS) {
    Serial.println("Failed to create charge/discharge task");
  }
}