// Controller MAC Adress: CC:DB:A7:3F:5C:EC
// Triamph MAC Address : F8:B3:B7:22:41:CC
uint8_t receiverMacAddress[] = {0xF8,0xB3,0xB7,0x22,0x41,0xCC};
bool connected_to_peer = false;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t ");
  //Serial.println(status);
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
  if (status == ESP_NOW_SEND_SUCCESS) {
    connected_to_peer = true;
  } else {
    connected_to_peer = false;
  }
}

void setup_now_sender() {
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("Succes: Initialized ESP-NOW");
  }
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  // peerInfo.ifidx=WIFI_IF_AP;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("Succes: Added peer");
  }  
}
 
void loop_now_sender() {
  //Sendind Data
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &SendingData, sizeof(SendingData));
  if (result == ESP_OK) 
  {
    //Serial.println("Sent with success");
  }
  else 
  {
    //Serial.println("Error sending the data");
  }    
}