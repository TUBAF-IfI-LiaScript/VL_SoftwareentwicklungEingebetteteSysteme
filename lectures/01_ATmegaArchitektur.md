<!--
author:   Sebastian Zug
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.1
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md#10

comment:  Try to write a short comment about
          your course, multiline is also okay.

translation: Deutsch  translations/German.md
translation: Français translations/French.md
-->

# ATmega Architektur

Die interaktive Version des Kurses ist unter diesem [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/SebastianZug/VL_ProzeduraleProgrammierung/master/01_ATmegaArchitektur.md#1) zu finden.

**Lernziele**

*


**Fragen an die heutige Veranstaltung ...**

*

## Was ist eine Rechnerarchitektur?

Typischerweise fehlen beim  Mikroprozessor im Chip viele Dinge, die
einen Rechner ausmachen, wie zum Beispiel der Arbeitsspeicher
(RAM),Programmspeicher usw.. Entsprechend sind dann der Daten- und Adress-Bus zur
Ansteuerung von externem Speicher sowie Leitungen für
Interrupts aus dem Chip herausgeführt.

Beim Mikrocontroller sind die Komponenten wie der Programmspeicher im Chip
integriert:

+ Timerbausteine
+ Analoge Eingänge
+ Digitale Eingänge
+ Interruptsystem

Ziel ist es, möglichst alle benötigten Elemente in einem Chip zu
vereinen.

```text @plantUML.png
@startuml
ditaa

+---------------------------------------------------------------------+
| Mikrocontroller                                                     |
| +-------------------+  +-------------------+  +-------------------+
| | Interrupt         |  | Floating Point    |  | Debug Interface   | |
| | Controller        |  | Unit (FPU)        |  |                   | |
| +-------------------+  +-------------------+  +-------------------+ |
|                                                                     |
| +-------------------+  +-------------------+  +-------------------+ |
| | Analog-Digital    |  | Mikroprozessor    |  | Timer und Zähler  | |
| | Wandler           |  |             c8F8  |  |                   | |
| +-------------------+  +-------------------+  +-------------------+ |
|                                                                     |
| +-------------------+  +-------------------+  +-------------------+ |
| | Takterzeugung     |  | Speicher          |  | Schnittstellen    | |
| |                   |  |                   |  | z.B. Seriell      | |
| +-------------------+  +-------------------+  +-------------------+ |
+---------------------------------------------------------------------+
@enduml
```
> **Merke:** Der Übergang zwischen Mikrocontrollern und Mikroprozessoren ist fließend!

> Die **Architektur** eines Rechners beschreibt dessen grundsätzlichen Aufbau und das Zusammenspiel der Komponenten.

```text @plantUML.png
@startuml
ditaa
  ^  |
  |  V
+-+----+    +----------------------------+
|c8F8  |    |c88F   Speicherwerk         |
|      |<-->|   +--------+  +--------+   |
|      |    |   |Programm|  | Daten  |   |
|      |    |   +--------+  +--------+   |
|      |    +------+---------------------+
|      |        ^  |             ^
| E/A- |        :  |             |
| Werk |        |  V             V
|      |    +---+------+    +------------+
|      |    |cF88      |    | Rechenwerk |
|      |    |          |    |cFF4        |
|      |    |          |<-=-+ +--------+ |
|      |<-=-+Steuerwerk|    | |Register| |
|      |    |          +-=->| +--------+ |
|      |    |          |    | |  ALU   | |
|      |    |          |    | +--------+ |
+------+    +----------+    +------------+
@enduml
```

Eine von-Neumann-Architektur hat einen einzigen Bus, über den sie auf alle ihre Komponenten zugreift.

## Befehlsabarbeitung und Instruction Set

RISC (Reduced Instruction Set) stellt tatsächlich eine “Entwurfs-Philosophie“ dar, in der das Ziel höchste
Leistung ist, die im Zusammenspiel von Hardware und einem optimierenden
Compiler erreicht wird. Im RISC-Ansatz wird in der der Instruktionssatz in
Hinblick auf Einfachheit und Regularität entworfen, so daß die Verwendung der
Instruktionen durch den Compiler einfach und überschaubar ist.

| Nachteile                                 | Vorteile                             |
| ----------------------------------------- | ------------------------------------ |
| Mehr Speicherplatz für Programme          | Einfachheit der Hardwarerealisierung |
| Aufwändigere Implementierung IM ASSEMBLER | höhere Taktraten                     |
|                                           | Ausnutzung von Pipelining Techniken  |

|                                    | CISC          | RISC                 |
| ---------------------------------- | ------------- | -------------------- |
| Ausführungszeit einer Instruktion  | $geq 1$       | meist 1              |
| Instruktuionssatz                  | groß          | klein                |
| Instruktionsformat                 | variabel      | strikt               |
| unterstützte Adressierungsschemata | komplex       | einfach (Load/Store) |
| CPU-Abarbeitungslogik              | Mikroprogramm | Hardware             |
| Komplexität                        | Hardware      | Compiler             |

> **Merke: ** Die Abgrenzung zwischen Complex Instruction Set (CISC) vs Reduced Instruction Set (RISC) ist heute kaum noch präsent. Vielmehr verschmelzen die Konzepte in aktuellen Prozessoren.

## ATmega Überblick

Schema

Register

+ General purpose registers (32)

   `R0 - R31`, dabei sind die Register `X` (26+27), `Y` (28+29) und `Z` (30+31) 16-Bit breit und werden für die indirekte Adressierung verwendet.

+ I/O Register

  CPU Status Register `SREG`, Stack Pointer `SP` und andere Konfigurationsregister für die Perepherie `TCCRx` oder `ADMUX`

+ nicht direkt zugreifbare Register

  Programmcounter `PC`, Transmit Shift Register `USART`

## Welche Einschränkungen ergeben sich aus der Architektur?

Fehlende Breite bei den aritmetischen Operationen

http://www.avr-asm-tutorial.net/avr_en/calc/HARDMULT.html


<div>
  <span id="simulation-time"></span>
</div>
```cpp       avrlibc.cpp
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void) {
  Serial.begin(9600);

  volatile int sample;

  asm volatile("ldi  r16, 250" "\n\t"
               "ldi  r17, 100" "\n\t"
               "mul  r16, r17" "\n\t"
               "movw %A0:%B0, r0:r1" "\n\t"
               "eor r1, r1" "\n\t"
              : "=a" (sample)
              :
              : "16", "r17");

  Serial.print("Antwort auf alle Fragen ist:");
  Serial.println(sample);

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```
@AVR8js.sketch

Fehlende Floating Point Unit
