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

# ATmega Controller

Die interaktive Version des Kurses ist unter diesem [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/SebastianZug/VL_ProzeduraleProgrammierung/master/02_ATmegaFamilie.md#1) zu finden.

**Lernziele**

*


**Fragen an die heutige Veranstaltung ...**

*


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
               "inc  %C0" "\n\t"
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





### Fehlende Fließkommaeinheit




###  Fehlende Festkommaeinheit
https://www.embedded-software-engineering.de/festkomma-arithmetik-einsatz-in-eigenen-algorithmen-und-bibliotheken-a-859213/

Neben den Fließkommadarstellungen lassen sich auch Festkommakonzepte für die Darstellung gebrochener Zahlen in Hardware/Software umsetzen. Dabei wird die Speicherbreite in den Anteil vor und nach einer spezifischen und unveränderlichen Kommaposition eingeteilt.

Ein Beschreibungsformat dafür ist die Q-Notation bei der die Anzahl der Nachkommastellen (und optional die Anzahl der ganzzahligen Bits) angegeben wird. Eine Q15-Zahl hat z. B. 15 Nachkommastellen; eine Q1.14-Zahl hat 1 ganzzahliges Bit und 14 Nachkommastellen.

> **Achtung:** Für vorzeichenbehaftete Festkommazahlen gibt es zwei widersprüchliche Verwendungen des Q-Formats. Bei der einen Verwendung wird das Vorzeichenbit als Ganzzahlbit gezählt, in der anderen Variante jedoch nicht. Zum Beispiel könnte eine vorzeichenbehaftete 16-Bit-Ganzzahl als Q16.0 oder Q15.0 bezeichnet werden. Um diese Unklarheit zu beseitigen wird teilweise ein U für `unsigned` eingefügt.


| Konfiguration | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Wert  |
| ------------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| UQ0.8         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 0.875 |
| UQ1.7         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 1.75  |
| UQ2.6         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 3.5   |


| Konfiguration | Auflösung | größte Zahl | kleinste Zahl |
| ------------- | --------- | ----------- | ------------- |
| `Qm.n`          | $2^{-n}$  | $2^{m-1}-2^{-n}$            | $-2^{m-1}$              |
| `UQm.n`          | $2^{-n}$  | $2^{m}-2^{-n}$            | $0$              |

Eine 16 Bit breite, vorzeichenbehaftete Festkommazahl `Q15.1` kann also Zahlenwerte im Bereich $[-16384.0, +16383.5]$ abbilden. Die Auflösung der Darstellung ist $2^{-n} = 0.5$

> **Merke:** Anders als eine Fließkommazahl ist die Auflösung der Festkommazahl konstant!

Bei der Rechnung mit Festkommazahlen werden die binären Muster prinzipiell so verarbeitet wie bei der Rechnung mit ganzen Zahlen. Festkomma-Arithmetik kann daher von jedem digitalen Prozessor durchgeführt werden, der arithmetische Operationen mit ganzen Zahlen unterstützt. Dennoch sind einige Regeln zu beachten, die sich auf die Position des Kommas vor und nach der Rechenoperation beziehen:

+ Bei Addition und Subtraktion muss die Position des Kommas für alle Operanden identisch sein. Ist dies nicht der Fall, sind die Operanden durch Schiebeoperationen entsprechend anzugleichen. Die Kommaposition des Ergebnisses entspricht dann der Kommaposition der Operanden.
+ Bei Multiplikation entspricht die Anzahl der Nachkommastellen des Ergebnisses der Summe der Anzahlen der Nachkommastellen aller Operanden.
+ Die Wortbreite des Endergebnisses wird auf die gewünschte Breite reduziert. Dabei wird häufig Sättigungsarithmetik und Rundung verwendet.

```c
int16_t q_add(int16_t a, int16_t b)
{
    return a + b;
}

int16_t q_add_sat(int16_t a, int16_t b)
{
    int16_t result;
    int32_t tmp;

    tmp = (int32_t)a + (int32_t)b;
    if (tmp > 0x7FFF)
        tmp = 0x7FFF;
    if (tmp < -1 * 0x8000)
        tmp = -1 * 0x8000;
    result = (int16_t)tmp;

    return result;
}
```

Der Dynamikbereich von Festkommawerten ist zwar wesentlich geringer als der von Fließkommawerten mit gleicher Wortgröße. Warum sollte man dann einen Mikrocontroller oder Prozessor mit Festkomma-Hardwareunterstützung verwenden?

+ **Größe und Stromverbrauch** - Die logischen Schaltungen der Festkomma-Hardware sind viel weniger kompliziert als die der Fließkomma-Hardware. Das bedeutet, dass die Festkomma-Chipgröße im Vergleich zur Fließkomma-Hardware kleiner ist und weniger Strom verbraucht.

+ **Speicherverbrauch und Geschwindigkeit** - Im Allgemeinen benötigen Festkommaberechnungen weniger Speicher und weniger Prozessorzeit.

+ **Kosten** - Festkomma-Hardware ist kostengünstiger, wenn Preis/Kosten eine wichtige Rolle spielen. Wenn digitale Hardware in einem Produkt verwendet wird, insbesondere bei Massenprodukten, kostet Festkomma-Hardware viel weniger als Fließkomma-Hardware.

Wie ist das Ganze implementiert? Seit der Version 4.8 integriert der [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc) eine entsprechende Bibliothek `stdfix.h`, die vordefinierte Typen integriert:

| Typname | Typ       | Größe in Byte | QU    | Q          |
| ------- | --------- | ------------- | ----- | ---------- |
| _Fract  | short     | 1             | 0.8   | $\pm$0.7   |
|         | long      | 4             | 0.32  | $\pm$0.31  |
|         | long long | 8             | 0.64  | $\pm$0.63  |
| _Accum  | short     | 1             | 8.8   | $\pm$8.7   |
|         | long      | 4             | 32.32 | $\pm$32.31 |
|         | long long | 8             | 16.48 | $\pm$16.47 |

> **Merke:** Daneben existieren verschiedene andere Festkommabibliotheken, die andere Konfigurationen unterstützen und verschiedene Implementierungen aufzeigen.

Lassen Sie uns einen genaueren Blick auf die Implementierung werfen. Im Codebeispiel, dass Sie im Projektordner XXX finden, addieren wir zwei Variablen unterschiedlichen Formates.

```c    FixedPoint.c
#include <avr/io.h>
#include <util/delay.h>
#include <stdfix.h>

int main (void) {

  unsigned short _Accum variableA = 0.75K;   
  short _Accum variableB = -1.875K;

  long _Accum result = variableA * variableB;

  while(1);
  return 0;
}
```

Für die `variableA` ergibt sich dabei folgender Auszug des Programmspeichers, sofern das Beispielprogramm ohne Optimierung übersetzt wird.
```
short _Accum variableB = -1.875K;
11c:	80 e1       	ldi	r24, 0x10	; 16
11e:	9f ef       	ldi	r25, 0xFF	; 255
120:	9c 83       	std	Y+4, r25	; 0x04
122:	8b 83       	std	Y+3, r24	; 0x03

     +--------+
r24  |00010000|
     +--------+
r25  |11111111|
     +--------+
```

### Vergleich der Softwarelösungen auf dem AVR

```cpp      

```
