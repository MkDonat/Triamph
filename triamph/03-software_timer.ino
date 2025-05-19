/*
  This sketch provide a function capable of creating
  then start a one shot software timer by providing few parameters.
  
  Example of use case
  
  TimerHandle_t TimerHandler = NULL;
  void TimerCallback(TimerHandle_t xTimer);

  one_shot_timer_start(
    "One Shot Test Timer",//Description 
    pdMS_TO_TICKS(3000),//Ticks To Wait 
    &TimerHandler,//Handler
    TimerCallback //Callback
  );
*/
void one_shot_timer_start(const char* const pcTimerName, const TickType_t xTimerPeriodInTicks, TimerHandle_t *xTimerHandle, TimerCallbackFunction_t pxCallbackFunction){
  *xTimerHandle = xTimerCreate( 
          /*const char * const pcTimerName*/pcTimerName, //short description
          /*const TickType_t xTimerPeriodInTicks*/xTimerPeriodInTicks, //period in ticks. *tips: use pdMS_TO_TICKS() to convert millis to ticks. 
          /*const BaseType_t xAutoReload*/pdFALSE, //Set xAutoReload to pdTRUE to create an auto-reload timer. Set xAutoReload to pdFALSE to create a one-shot timer.
          /*void * const pvTimerID*/ NULL, //Unique ID
          /*TimerCallbackFunction_t*/ pxCallbackFunction //callback function
        );
  Serial.println(pcTimerName);
  if(*xTimerHandle == NULL){
    Serial.println(" ->Couldn't create\n Insufficient heap in FreeRTOS!");
    return;
  }
  else Serial.println(" ->Created!");

  vTaskDelay(pdMS_TO_TICKS(5));// let the deamon task run the timer.

  BaseType_t TimerStartInfo = xTimerStart( 
    /*TimerHandle_t*/ *xTimerHandle, 
    /*TickType_t xTicksToWait*/10
  );
  if(TimerStartInfo == pdPASS) Serial.println(" ->Started!");
  else if(TimerStartInfo == pdFAIL){
    Serial.println(" ->Failed to start timer. Queue was full!");
    return;
  }
}