#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // FreeRTOS-Funktionen fuer Mutexe/Semaphore (gemeinsamer Header)

// Mutex (kein Semaphor!): schuetzt die gemeinsame Ressource "serielle
// Schnittstelle". Take und Give erfolgen IMMER im selben Task (Besitz).
SemaphoreHandle_t xSerialMutex;

void TaskA( void *pvParameters );
void TaskB( void *pvParameters );

void setup() {

  Serial.begin(9600);
  
  if ( xSerialMutex == NULL )  
  {
    xSerialMutex = xSemaphoreCreateMutex();  
    if ( ( xSerialMutex ) != NULL )
      xSemaphoreGive( ( xSerialMutex ) );  
  }

  xTaskCreate(
    TaskA
    ,  "A"  
    ,  128  
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskB
    ,  "B"
    ,  128 
    ,  NULL
    ,  1  
    ,  NULL );

}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskA( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  for (;;) 
  {
    //Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    // Ist der Mutex belegt, bis zu 5 Ticks auf seine Freigabe warten.
    if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      xSemaphoreGive( xSerialMutex );
    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskB( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  for (;;)
  {
    //Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
      xSemaphoreGive( xSerialMutex ); 
    }

    vTaskDelay(1);
  }
}