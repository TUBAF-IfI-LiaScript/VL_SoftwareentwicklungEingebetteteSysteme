#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskBlink_Handler;
TaskHandle_t TaskSerial_Handler;


void TaskBlink( void *pvParameters );
void TaskSerial(void* pvParameters);

void setup() {
  Serial.begin(9600);
  
   xTaskCreate(
    TaskBlink
    ,  "Blink"  
    ,  128  // Stack size
    ,  NULL //Parameters passed to the task function
    ,  2  // Priority
    ,  &TaskBlink_Handler );

   xTaskCreate(
    TaskSerial
    ,  "Serial"
    ,  128  
    ,  NULL 
    ,  1  
    ,  &TaskSerial_Handler );  //Task handle
}
    

void loop()
{
  // Empty. Things are done in Tasks.
}


void TaskSerial(void* pvParameters){
  (void) pvParameters;
   for (;;) // A Task shall never return or exit.
   {
    while(Serial.available()>0){
      switch(Serial.read()){
        case 's':
          vTaskSuspend(TaskBlink_Handler); 
          Serial.println("Suspend!");
          break;
        case 'r':
          vTaskResume(TaskBlink_Handler);
          Serial.println("Resume!");
          break;
      }
      vTaskDelay(1);
    }
   }
}

void TaskBlink(void *pvParameters)
{
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);
  for (;;) // A Task shall never return or exit.
  {
    //Serial.println(11);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}