#include <Wire.h>

#include <math.h>

void waitForInput() {
  while (Serial.available() == 0);
  delay(10);
}

void setup() {
  Serial.begin(9600);
  float current_cap, current_lat, current_long, wanted_lat, wanted_long;

  Serial.println("Cap actuel : ");
  waitForInput();
  current_cap = Serial.parseFloat();
  Serial.read();

  Serial.println("Latitude actuelle : ");
  waitForInput();
  current_lat = Serial.parseFloat();
  Serial.read();

  Serial.println("Longitude actuelle : ");
  waitForInput();
  current_long = Serial.parseFloat();
  Serial.read();

  Serial.println("Latitude voulue : ");
  waitForInput();
  wanted_lat = Serial.parseFloat();
  Serial.read();

  Serial.println("Longitude voulue : ");
  waitForInput();
  wanted_long = Serial.parseFloat();
  Serial.read();

  // Calcul du cap voulu
  float delta_long = radians(wanted_long - current_long);
  float lat1 = radians(current_lat);
  float lat2 = radians(wanted_lat);

  float x = sin(delta_long) * cos(lat2);
  float y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(delta_long);

  float wanted_cap = atan2(x, y);
  float wanted_cap_deg = fmod((degrees(wanted_cap) + 360), 360); // Normalisation

  // Calcul de la différence de cap
  float delta_cap = fmod((current_cap - wanted_cap_deg + 360), 360);
  if (delta_cap > 180) {
      delta_cap -= 360;  // Ajustement pour [-180, 180]
  }
  // Calcul de la distance
  float delta_lat = radians(wanted_lat - current_lat);
  
  float a = pow(sin(delta_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(delta_long / 2), 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float distance_km = 6371 * c; // Distance en km

  // Affichage des résultats
  Serial.println("\n--- Résultats ---");
  Serial.print("Latitude actuelle : "); Serial.println(current_lat, 6);
  Serial.print("Longitude actuelle : "); Serial.println(current_long, 6);
  Serial.print("Cap actuel : "); Serial.println(current_cap, 1);
  Serial.print("Latitude voulue : "); Serial.println(wanted_lat, 6);
  Serial.print("Longitude voulue : "); Serial.println(wanted_long, 6);
  Serial.print("Cap voulu : "); Serial.println(wanted_cap_deg, 1);
  Serial.print("Delta cap : "); Serial.println(delta_cap, 1);
  Serial.print("Distance km : "); Serial.println(distance_km, 3);
  Serial.println("-----------------\n");
}

void loop() {

}