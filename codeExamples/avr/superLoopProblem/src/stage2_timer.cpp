/*
 * Stufe 2 - Super-Loop mit eigener Zeitbasis (millis())
 * ==============================================================
 *
 * Verbesserung gegenüber Stufe 1:
 *   Wir warten nicht mehr blind mit delay(), sondern vergleichen
 *   millis() gegen einen Sollzeitpunkt. Jede Aufgabe bekommt ihre
 *   eigene "Fälligkeit". Das ist das klassische kooperative
 *   Zeitscheiben-Muster (auch "Bare-Metal-Scheduler" genannt).
 *
 * Was jetzt besser ist:
 *   - Der Regler wird nicht mehr von delay() ausgebremst, solange
 *     keine andere Aufgabe läuft.
 *
 * Was IMMER NOCH nicht funktioniert:
 *   - Die I2C-Display-Ausgabe ist weiterhin blockierend. Ist sie fällig,
 *     steckt die CPU ~23 ms im Wire-busy-wait des Frame-Transfers. In
 *     dieser Zeit kann die loop() den Reglerzeitpunkt NICHT prüfen - der
 *     10-ms-Takt reißt erneut auf, nur seltener und schwerer zu finden.
 *   - Es gibt keine Priorität: Die zeitkritische Regelung kann die
 *     unkritische Display-Ausgabe nicht unterbrechen. Kooperatives
 *     Multitasking heißt: Jede Aufgabe muss FREIWILLIG schnell zurück-
 *     kehren. Tut sie es nicht (langsame Peripherie, langer Rechen-
 *     schritt), leiden ALLE anderen.
 *
 * Genau hier endet das, was man "von Hand" sauber bekommt: Sobald
 * eine Aufgabe blockiert oder lange rechnet, bräuchte man Präemption
 * - und die muss man selbst nicht mehr bauen. Das ist die Aufgabe
 * eines RTOS (siehe stage3_freertos.cpp).
 */

#include <Arduino.h>

const uint8_t PIN_REGLER = 8;

volatile int16_t stellwert = 0;

static void reglerSchritt()
{
    stellwert = (stellwert + 1) & 0x03FF;
    digitalWrite(PIN_REGLER, !digitalRead(PIN_REGLER));
}

// Simulierte I2C-Display-Ausgabe (siehe stage1_naiv.cpp):
// 1024 Byte a ~22 us Buszeit (400 kHz) = ~23 ms blockierend.
static void displaySchreiben(int16_t wert)
{
    (void) wert;
    for (uint16_t byteNr = 0; byteNr < 1024; ++byteNr) {
        delayMicroseconds(22);
    }
}

void setup()
{
    pinMode(PIN_REGLER, OUTPUT);
    Serial.begin(9600);
    Serial.println(F("Stage 2: Super-Loop mit millis()-Zeitbasen"));
}

void loop()
{
    static uint32_t naechsterRegler = 0;
    static uint32_t naechstesDisplay = 0;
    uint32_t jetzt = millis();

    // (A) Regler - fällig alle 10 ms
    if (jetzt >= naechsterRegler) {
        naechsterRegler += 10;
        reglerSchritt();
    }

    // (B) Display - fällig alle 250 ms
    if (jetzt >= naechstesDisplay) {
        naechstesDisplay += 250;
        // Blockiert ~23 ms! Während dieser Zeit wird der obere
        // if-Zweig nicht erreicht -> Regler verpasst Takte.
        displaySchreiben(stellwert);
    }
}
