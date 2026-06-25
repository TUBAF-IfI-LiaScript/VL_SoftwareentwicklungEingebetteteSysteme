/*
 * Stufe 1 - Die naive Super-Loop
 * ==============================================================
 *
 * Aufgabenstellung:
 *   (A) Ein zeitdiskreter Regler MUSS exakt alle 10 ms einen neuen
 *       Stellwert berechnen. Die Reglerverstärkung wurde für T = 10 ms
 *       ausgelegt - jede Abweichung der Abtastzeit verfälscht das
 *       Regelverhalten.
 *   (B) Alle 250 ms soll der aktuelle Zustand auf einem grafischen
 *       OLED (SSD1306, 128x64) ueber I2C dargestellt werden.
 *
 * Diese erste Version löst beide Aufgaben "geradeaus" in einer
 * Schleife mit delay().
 *
 * Warum eine I2C-Display-Ausgabe und nicht UART?
 *   Auf dem Arduino erledigt die UART-Hardware das Senden per ISR im
 *   Hintergrund - Serial.print() blockiert kaum. Die Arduino-Wire-
 *   Bibliothek dagegen ist BLOCKIEREND: Wire.endTransmission() ruft
 *   twi_writeTo() auf, das in einer Schleife auf den Abschluss der
 *   Uebertragung wartet (busy-wait auf twi_state). Die TWI-Hardware
 *   taktet zwar die Bits, aber die CPU steckt in der Warteschleife.
 *
 * Das Problem (ehrliche Busrechnung):
 *   Ein 128x64-Frame = 1024 Byte. Pro Byte 9 Bit-Zeiten (8 Daten + ACK)
 *   auf dem I2C-Bus. Bei 400 kHz (Fast Mode):
 *       1024 * 9 / 400000 ~ 23 ms.
 *   Während dieser Zeit blockiert die Schleife komplett. Der Regler
 *   "verschläft" in jedem Display-Zyklus rund 2 Abtastschritte.
 *
 * Hinweis zum Code:
 *   Damit das Beispiel auch OHNE angeschlossenes OLED den Timing-Effekt
 *   zeigt, simuliert displaySchreiben() die Buszeit mit delayMicroseconds
 *   statt echte Wire-Transfers abzusetzen. Die Dauer ist exakt aus der
 *   Busrechnung abgeleitet (1024 Byte a ~22.5 us). Mit echtem SSD1306
 *   ersaetze man den Schleifenrumpf durch die Wire-/Display-Aufrufe -
 *   das Blockierverhalten bleibt identisch.
 *
 * Mess-Pin:
 *   An PIN_REGLER (D8) wird bei JEDEM Reglerschritt getoggelt. Auf
 *   dem Oszilloskop muss eine saubere 10-ms-Rechteckschwingung zu
 *   sehen sein - hier ist sie es NICHT.
 */

#include <Arduino.h>

const uint8_t PIN_REGLER = 8;   // Toggelt bei jedem Reglerschritt -> Messpunkt

// --- Platzhalter für die eigentliche Regelung -------------------
volatile int16_t stellwert = 0;

static void reglerSchritt()
{
    // Hier stünde ein PI-/PID-Algorithmus. Für die Demonstration
    // genügt eine triviale Berechnung - entscheidend ist, DASS der
    // Schritt exakt alle 10 ms passiert.
    stellwert = (stellwert + 1) & 0x03FF;
    digitalWrite(PIN_REGLER, !digitalRead(PIN_REGLER));   // Messpuls
}

// --- Simulierte I2C-Display-Ausgabe -----------------------------
// Modelliert das blockierende Schreiben eines 128x64-OLED-Frames ueber
// I2C: 1024 Byte a ~22.5 us Buszeit (400 kHz) = ~23 ms. Mit echter
// Hardware steckte die CPU genauso lange im Wire-busy-wait fest.
static void displaySchreiben(int16_t wert)
{
    (void) wert;   // in echt: Framebuffer rendern und per Wire senden
    for (uint16_t byteNr = 0; byteNr < 1024; ++byteNr) {
        delayMicroseconds(22);   // ~ ein I2C-Byte bei 400 kHz (9 Bit)
    }
}

void setup()
{
    pinMode(PIN_REGLER, OUTPUT);
    Serial.begin(9600);
    Serial.println(F("Stage 1: naive Super-Loop"));
}

void loop()
{
    static uint8_t diagZaehler = 0;

    // (A) Regler
    reglerSchritt();

    // (B) Display alle 25 Durchläufe (~ alle 250 ms)
    if (++diagZaehler >= 25) {
        diagZaehler = 0;
        // ACHTUNG: blockiert ~23 ms -> der 10-ms-Takt bricht zusammen!
        displaySchreiben(stellwert);
    }

    // Warten, damit der nächste Reglerschritt bei 10 ms liegt.
    delay(10);   // <-- Annahme: Schleife kostet 0 ms. Stimmt nicht!
}
