/*
 * Stufe 3 - Die Lösung mit FreeRTOS
 * ==============================================================
 *
 * Jetzt bekommt jede Aufgabe ihren eigenen Task mit eigener
 * Priorität. Der entscheidende Unterschied zur Super-Loop:
 *
 *   1. PRÄEMPTION
 *      Der Regler-Task hat die HÖHERE Priorität. Sobald sein
 *      10-ms-Tick fällig ist, entzieht der Scheduler dem laufenden
 *      Display-Task SOFORT die CPU - mitten im I2C-Transfer.
 *      Der Reglertakt bleibt dadurch stabil, OBWOHL die Display-
 *      Ausgabe weiterhin ~23 ms blockierend sendet. Die laufende
 *      I2C-Uebertragung wird dabei nicht beschaedigt: Die TWI-
 *      Hardware taktet das aktuelle Byte autonom fertig, nur die
 *      CPU wechselt den Task.
 *
 *   2. SAUBERE ZEITBASIS
 *      vTaskDelayUntil() rechnet den nächsten Weckzeitpunkt relativ
 *      zum LETZTEN, nicht zum aktuellen. Laufzeit-Jitter der Task-
 *      Funktion akkumuliert sich dadurch nicht - der mittlere
 *      Abtastabstand bleibt exakt 10 ms.
 *
 *   3. TRENNUNG VON FUNKTIONALITÄT UND TIMING
 *      Keine if-(jetzt >= ...)-Buchhaltung mehr. Jeder Task
 *      beschreibt nur seine Aufgabe und seine Periode - das Timing
 *      übernimmt der Kernel.
 *
 * Auf dem Oszilloskop an PIN_REGLER (D8): saubere 10-ms-Schwingung,
 * auch während der Display-Ausgabe.
 */

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

const uint8_t PIN_REGLER = 8;

volatile int16_t stellwert = 0;

// Simulierte I2C-Display-Ausgabe (siehe stage1_naiv.cpp):
// 1024 Byte a ~22 us Buszeit (400 kHz) = ~23 ms blockierend.
static void displaySchreiben(int16_t wert)
{
    (void) wert;
    for (uint16_t byteNr = 0; byteNr < 1024; ++byteNr) {
        delayMicroseconds(22);
    }
}

// ---- Zeitkritischer Regler-Task (hohe Priorität) ---------------
static void TaskRegler(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriode = pdMS_TO_TICKS(10);   // 10 ms

    for (;;) {
        stellwert = (stellwert + 1) & 0x03FF;
        digitalWrite(PIN_REGLER, !digitalRead(PIN_REGLER));

        // Exakt 10 ms relativ zum letzten Weckzeitpunkt warten.
        vTaskDelayUntil(&xLastWakeTime, xPeriode);
    }
}

// ---- Unkritischer Display-Task (niedrige Priorität) ------------
static void TaskDisplay(void *pvParameters)
{
    (void) pvParameters;
    const TickType_t xPeriode = pdMS_TO_TICKS(250);  // 250 ms

    for (;;) {
        // Blockiert ~23 ms - wird aber jederzeit vom Regler-Task
        // präemptiert. Der FreeRTOS-Tick ist selbst ein Interrupt und
        // feuert auch während des Wire-busy-waits (twi_writeTo sperrt
        // die Interrupts nicht), daher bleibt der 10-ms-Takt unberührt.
        displaySchreiben(stellwert);
        vTaskDelay(xPeriode);
    }
}

void setup()
{
    pinMode(PIN_REGLER, OUTPUT);
    Serial.begin(9600);
    Serial.println(F("Stage 3: FreeRTOS mit Praemption"));

    // Hoehere Zahl = hoehere Prioritaet.
    xTaskCreate(TaskRegler,  "Regler",  128, NULL, 2, NULL);
    xTaskCreate(TaskDisplay, "Display", 128, NULL, 1, NULL);

    vTaskStartScheduler();
}

void loop()
{
    // Leer - die Tasks uebernehmen die Kontrolle.
}
