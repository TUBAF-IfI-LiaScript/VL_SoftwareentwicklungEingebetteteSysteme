// Analoger Comparator des ATmega328P
//
// Vergleicht die Spannung an AIN0 (PD6) mit einer Referenz.
// Hier: interne Bandgap-Referenz (~1.1 V) gegen ADC3 (PC3) als
// negativen Eingang via Multiplexer (ACME=1, ADEN=0).
// Ergebnis liegt im ACO-Bit von ACSR.
//
// Datenblatt: ATmega48A/PA/88A/PA/168A/PA/328/P, Kapitel "Analog Comparator".

#define F_CPU 16000000UL

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

void Init(void) {
  DDRB  |=  (1 << PB5);   // LED-Ausgang (Arduino D13)
  DDRC  &= ~(1 << PC3);   // PC3 als Eingang (negativer Komparator-Input)
  PORTC &= ~(1 << PC3);   // Pull-Up aus

  ADCSRB |=  (1 << ACME); // Analog Comparator Multiplexer Enable
  ADCSRA &= ~(1 << ADEN); // ADC abschalten, sonst ignoriert ACME
  ADMUX  &= ~0x07;                                    // MUX-Bits löschen
  ADMUX  |=  (0 << MUX2) | (1 << MUX1) | (1 << MUX0); // ADC3 wählen

  ACSR = (0 << ACD)   |   // Comparator eingeschaltet
         (1 << ACBG)  |   // Bandgap-Referenz an AIN0
         (0 << ACIE)  |   // Interrupt deaktiviert (Polling im main)
         (0 << ACIC)  |   // kein Input Capture
         (0 << ACIS1) |   // Toggle-Modus (jede Flanke)
         (0 << ACIS0);

  Serial.println("Initialisierung abgeschlossen");
  Serial.flush();
}

int main(void) {
  Serial.begin(9600);
  Serial.println("Hello World");
  Serial.flush();

  Init();

  while (1) {
    if (ACSR & (1 << ACO)) {
      PORTB &= ~(1 << PB5); // LED aus
    } else {
      PORTB |=  (1 << PB5); // LED an
    }
  }
  return 0;
}
