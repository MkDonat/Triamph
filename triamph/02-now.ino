uint16_t SIGNAL_TIMEOUT = 1000;  // This is signal timeout in milli seconds. We will reset the data if no signal
unsigned long lastRecvTime = 0;

// callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == 0)
  {
    return;
  }
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  lastRecvTime = millis();
  Serial.printf("gaz: %d/4095\n",receivedData.gaz_lecture);
}

void setup_now() { 
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop_now() {
  //Check Signal lost.
  unsigned long now = millis();
  if ( now - lastRecvTime > SIGNAL_TIMEOUT ) {
    return; //réinitialiser certain paramètres
  }
}