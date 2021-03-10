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

### 8-Bit Datenbreite

Die Festlegung auf 8-Bit Operanden und Ausgabe bei den arithmetisch/logischen Operationen erfordert umfangreiche Berechnungen schon bei bescheidenen Größenordnungen.

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
               "movw %0, r0" "\n\t"
               "eor r1, r1" "\n\t"
              : "=a" (sample)
              :
              : "r16", "r17");

  Serial.print("Das Ergebnis ist ");
  Serial.println(sample);

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```
@AVR8js.sketch

> Warum scheitert das Ganze, wenn `r1` keine 0 enthält?

Mit dem 8-Bit Multiplikator decken wir aber nur Konstellationen hab, für die gilt das die Faktoren beide immer kleiner als 256 sein müssen. Um das Problem mit größeren Binärzahlen zu lösen, betrachten wir zunächst nur diese Kombination aus 16 und 8. Das Verständnis dieses Konzepts hilft, die Methode zu verstehen, so dass Sie später in der Lage sein werden, das 32-mal-64-Bit-Multiplikationsproblem zu lösen.

Die Mathematik dafür ist einfach, ein 16-Bit-Binär sind einfach zwei 8-Bit-Binäre, wobei der höchstwertige dieser beiden mit dezimal 256 oder hex 100 multipliziert wird. Das 16-Bit-Binär $m1$ ist also gleich $256*m1_H$ plus $m1_L$, wobei $m1_H$ das MSB und $m1_L$ das LSB ist. Die Multiplikation von $m1$ mit dem 8-Bit-Binär $m2$ ist also, mathematisch formuliert:

$$
m1 \cdot m2 = (256 \cdot m1_H + m1_L) \cdot m2 = 256 \cdot m1_H \cdot m2 + m1_L \cdot m2
$$

Wir brauchen also nur zwei Multiplikationen durchzuführen und beide Ergebnisse zu addieren. Die Multiplikation mit 256 erfordert keine Hardware, da es sich um einen  Sprung zum nächsthöheren Byte handelt. Lediglich der Übertrag bei der Additionsoperation muss beachtet werden.

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii

    +----------+
r16 |   0x10   | Faktor 1   low Byte  "$m1_L$"
    +----------+ 8208
r17 |   0x20   |            high Byte "$m1_H$"
    +----------+
r18 |   0xFF   | Faktor 2             "$m2$"
    +----------+ ........ -. .........................................                 
r19 |   0xF0   | Ergebnis  |       
    +----------+ 2093040   ⎬ "$m1_L\cdot m2$"-.
r20 |   0xEF   |           |                  |   
    +----------+          -.                  ⎬ "$m1_H \cdot m2$"
r21 |   0x1F   |                              |                 carry
    +----------+                             -.
r22 |   0x00   |      0
    +----------+
```

<div>
  <span id="simulation-time"></span>
</div>
```cpp       avrlibc.cpp
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void) {
  Serial.begin(9600);

  volatile long sample;

  asm volatile("ldi  r16, 0x10" "\n\t"
               "ldi  r17, 0x20" "\n\t"
               "ldi  r18, 0xFF" "\n\t"
               "eor  %D0, %D0" "\n\t"
               "mul  r16, r18" "\n\t"
               "mov  %A0, r0" "\n\t"
               "mov  %B0, r1" "\n\t"
               "mul  r17, r18" "\n\t"
               "add  %B0, r0" "\n\t"
               "mov  %C0, r1" "\n\t"
               "brcc NoInc" "\n\t"
               "inc  %D0" "\n\t"
               "NoInc:" "\n\t"
               "eor r1, r1" "\n\t"
                ""
              : "=r" (sample)
              :
              : "r16", "r17", "r18" );


  Serial.print("Das Ergebnis ist ");
  Serial.println(sample);

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```
@AVR8js.sketch

Für die Muliplikation von größeren Werten wird die Berechnung entsprechend aufwändiger.

### Fehlende Floating Point Unit




Alternative Fixed Point Implementierung


```cpp      
#include <avr/io.h>
#include <stdfix.h>

int main (void) {
  Serial.begin(9600);

  volatile accum fx1, fx2 = 2.33K, fx3 = 0.66K;
  volatile float fl1, flt2 = 2.33, fl3 = 0.66;

  fx1 = fx2 + fx3;
  fl1 = fl2 + fl3;

  fx1 = fx2 - fx3;
  fl1 = fl2 - fl3;

  fx1 = fx2 * fx3;
  fl1 = fl2 * fl3;

  fx3 = fx2 / fx3;
  fl3 = fl2 / fl3;

  return 0;
}
```
