const uint8_t LED_ORANGE = 16;
const ulong periode_led_orange = 500;

void blinkOrangeLED(void *arg){
  pinMode(LED_ORANGE, OUTPUT);
  digitalWrite(LED_ORANGE, 0);
  while(1){
    digitalWrite(LED_ORANGE, 1);
    delay(periode_led_orange);
    digitalWrite(LED_ORANGE, 0);
    delay(periode_led_orange);
  }
}
void CreateTasksForLedOrange(){
  xTaskCreatePinnedToCore(
    blinkOrangeLED, "Blink Orange Led", 2048
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