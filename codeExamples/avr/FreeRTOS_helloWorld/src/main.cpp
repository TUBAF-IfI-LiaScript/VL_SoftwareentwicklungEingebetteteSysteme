#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// Variablen zur Frequenzmessung 
static uint32_t task_counter = 0;
static uint32_t value_sum = 0;
static TickType_t last_wake_time = 0;
static TickType_t current_time = 0;

void setup() {
  

  Serial.begin(9600);
  while (!Serial) {
        ; // Warten bis die serielle Verbindung bereit ist (nur relevant für native USB-Ports)
    }
    
  Serial.println(F("FreeRTOS Task Frequency Measurement"));
  
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialisieren der Last Wake Time
  last_wake_time = xTaskGetTickCount();
    
  for (;;) // A Task shall never return or exit.
  {
    // LED umschalten
        PORTB ^= _BV(PORTB5);
        
        // Aktuelle Zeit holen
        current_time = xTaskGetTickCount();
        
        // Alle 10 Ausführungen messen wir die Frequenz
        if (task_counter % 10 == 0) {
            // Frequenz berechnen (in Hz) - ticks pro Sekunde geteilt durch vergangene Ticks
            float frequency = (10.000f * configTICK_RATE_HZ) / (current_time - last_wake_time);
            
            // Ausgabe über serielle Schnittstelle
            Serial.println();
            Serial.print(current_time);
            Serial.print(F("Task Blink Frequenz: "));
            Serial.print(frequency, 4);
            Serial.print(F(" Hz"));
            
            // Zeit zurücksetzen
            last_wake_time = current_time;
        }
        
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  for (;;)
  {
    int sensorValue = analogRead(A0);
    value_sum += sensorValue;
    //Serial.print(value_sum);
    vTaskDelay( 100 / portTICK_PERIOD_MS ); 
  }
} 