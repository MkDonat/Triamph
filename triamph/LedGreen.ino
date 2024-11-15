const uint8_t LED_GREEN = 15;
const ulong periode_led_verte = 200;

void blinkGreenLED(void *arg){
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);
  while(1){
    digitalWrite(LED_GREEN, 1);
    delay(periode_led_verte);
    digitalWrite(LED_GREEN, 0);
    delay(periode_led_verte);
  }
}
void CreateTasksForLedGreen(){
  xTaskCreatePinnedToCore(
    blinkGreenLED, "Blink GREEN Led", 2048
    ,
    NULL // Stack Depth
    ,
    2 //Priority
    ,
    NULL //Task handle
    ,
    CORE_1 // Core on which the task will run
  );
}