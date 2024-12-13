#include <ESP32Servo.h>

// Servo Configuration
Servo Pincedroite;
Servo Pincegauche;
const int ouverturePince = 180; // Open position
const int fermeturePince = 0;   // Close position
// Push button for Pince
const int PincebuttonPin = 27;
int PincebuttonState = LOW;
int lastPincebuttonState = LOW;
unsigned long lastDebounceTimePince = 0;
unsigned long debounceDelayPince = 10;
int ouvertoufermee = 0;

void ouverture_fermeture_pince(void *arg){
  Pincedroite.attach(16); // Servo 1 (GPIO 16)
  Pincegauche.attach(22); // Servo 2 (GPIO 22)
  pinMode(PincebuttonPin, INPUT_PULLUP);
  PincebuttonState = LOW;
  lastPincebuttonState = LOW;
  ouvertoufermee = 0;
  while (1) {
    int readingPince = digitalRead(PincebuttonPin);
    if (readingPince != lastPincebuttonState) {
      lastDebounceTimePince = millis();
    }
    if ((millis() - lastDebounceTimePince) > debounceDelayPince) {
      if (readingPince != PincebuttonState) {
        PincebuttonState = readingPince;
        if (PincebuttonState == LOW) {
          ouvertoufermee = ouvertoufermee + 1;
          if ((ouvertoufermee) % 2 == 0) {
            Serial.println("Ouverture");
            Pincedroite.write(ouverturePince);
            Pincegauche.write(ouverturePince);
          } else if ((ouvertoufermee) % 2 == 1) {
            Serial.println("Fermeture");
            Pincedroite.write(fermeturePince);
            Pincegauche.write(fermeturePince);
          }
        }
      }
    }
    lastPincebuttonState = readingPince;
  }
}

// Create task for handling charge and discharge operations
void CreateTasksForPince(){
  if (xTaskCreatePinnedToCore(
        ouverture_fermeture_pince,      // Task function
        "Ouvrir/fermer",   // Task name
        4096,                 // Stack size
        NULL,                 // Parameters
        1,                    // Priority
        NULL,                 // Task handle
        CORE_2                 // Core
      ) != pdPASS) {
    Serial.println("Failed to create pince task");
  }
}