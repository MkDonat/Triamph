const uint8_t LED_RED = 23;
const ulong red_led_off_delay = 3000;
const ulong red_led_on_delay = 100;

void blinkRedLED(void *arg){
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, 0);
  while(1){
    digitalWrite(LED_RED, 1);
    delay(red_led_on_delay);
    digitalWrite(LED_RED, 0);
    delay(red_led_off_delay);
  }
}
void CreateTasksForLedRed(){
  xTaskCreatePinnedToCore(
    blinkRedLED, "Blink RED Led", 2048
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