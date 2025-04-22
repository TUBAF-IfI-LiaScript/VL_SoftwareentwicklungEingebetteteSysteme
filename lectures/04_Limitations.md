<!--

author:   Sebastian Zug, Karl Fessel & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de
version:  0.0.5
language: de
narrator: Deutsch Female
comment:  Darstellung der grundlegenden Eigenschaften der atmega Mikroncontroler-Familie, Abstraktionsebenen der Programmierung
icon:     https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md

-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/lectures/04_Limitations.md#1)


# Aritmethik und Analog-Digital-Wandler

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Vorlesung Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2025`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Beschränkungen des 8-Bit Controllers und Rekapitulation der Analog-Digital-Wandler Begriffe`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/04_Limitations.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/04_Limitations.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/26gR2qGRnxxXAvhBu/giphy.gif)

---

## Einschub - Inline Assembler 

Inline-Assembler erlaubt die Einbettung von kleinen Assembler-Codes direkt in den C/C++-Code. Folglich muss keine Assembler-Funktion aufgerufen werden, was die Unterstützung durch den Compiler bei der Registerverwendung ermöglicht.

> Eine sehr gute Übersicht zur Konfiguration von Inlineassambler bietet [Link](https://rn-wissen.de/wiki/index.php/Inline-Assembler_in_avr-gcc).

```
asm volatile (asm-template 
             : output-operand-list 
             : input-operand-list 
             : clobber-list);
```

| Elemente              | Bedeutung                                                                         |
| --------------------- | --------------------------------------------------------------------------------- |
| `asm-template`        | ... String mit Assembler-Befehlen und Platzhaltern                                |
| `output-operand-list` | ... Constraints und C-Ausdruck                                             |
| `input-operand-list`  |                                                                                   |
| `clobber-list`        | ... Liste von Registern, deren Inhalt durch den Inline-Assembler manipuliert wird |

> Warum die Frage nach den verwendeten Registern?

| Kategorie           | Register          | Bedeutung                                                                                                                                                                                                           |
| ------------------- | ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Feste Register      | R0                | ... wird als Scratch-Register verwendet, das nach seiner Verwendung nicht wiederhergestellt werden muss. Es muss im Prolog und Epilog der Interrupt Service Routine (ISR) gespeichert und wiederhergestellt werden. |
|                     | R1                | ...  enthält immer Null. Dieses Register muss in ISR-Prologs gespeichert und dann auf Null gesetzt werden, da R1 andere Werte als Null enthalten  kann.                                                             |
| Call-Used-Register  | R18–R27, R30, R31 | Diese Register sind Call-Clobbered. Eine normale Funktion kann sie verwenden, ohne ihren Inhalt wiederherzustellen. ISRs müssen jedes von ihnen verwendete Register speichern und wiederherstellen.                 |
| Call-Saved-Register | R2-R17, R28, R29  | Die genannten Register sind aufrufgesichert, d.h. eine Funktion, die ein solches Register verwendet, muss dessen ursprünglichen Inhalt wiederherstellen.                                                           |


## Welche Einschränkungen ergeben sich aus der Architektur?

> Warum sprechen wir im Zusammenhang mit den Controllern von fehlender Performance verglichen mit anderen Systemen?

<!--data-type="none"-->
| Arduino Uno Board                                                                                                 | Nucleo 64                                                                                          |
| ----------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| ![ArduinoUno](../images/00_Einfuehrung/arduino-uno-rev3.jpg)<!-- style="width: 100%; auto; max-width: 415px;" --> | ![ArduinoUno](../images/00_Einfuehrung/Nucleo64.jpg)<!-- style="width: 100%; max-width=315px;" --> |
| Microchip ATmega 328p                                                                                             | STM32F401                                                                                          |
| 8-Bit AVR Familie                                                                                                 | 32-Bit Cortex M4 Prozessor                                                                         |
| C, Assembler                                                                                                      | C, C++                                                                                             |
| avrlibc, FreeRTOS                                                                                                 | CMSIS, mbedOS, FreeRTOS                                                                            |
| 10 Bit Analog-Digital-Wandler, 16 Bit Timer,                                                                      | 10 timers, 16- and 32-bit (84 MHz), 12-bit ADC                                                     |

### 8-Bit Datenbreite


                            {{0-1}}
*******************************************************************

![Bild](../images/02_ATmegaFamilie/AtTiny.png "ATtiny Architektur [^AtTinyArchitecture]")

Die Festlegung auf 8-Bit Operanden und Ausgabe bei den arithmetisch/logischen Operationen erfordert umfangreiche Berechnungen schon bei bescheidenen Größenordnungen.

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
  Serial.flush();

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```

> Wie lange dauert die Berechnung für die in Zeile 11 - 13 genannten Befehle?

> Warum scheitert das Ganze, wenn `r1` keine 0 enthält?

Das Beispiel findet sich im Projektordner unter `../codeExamples/avr/inlineAssembly/`.

*******************************************************************

                            {{1-2}}
*******************************************************************

Mit dem 8-Bit Multiplikator decken wir aber nur Konstellationen hab, für die gilt das die Faktoren beide immer kleiner als 256 sein müssen. Um das Problem mit größeren Binärzahlen zu lösen, betrachten wir zunächst nur diese Kombination aus 16 und 8. Das Verständnis dieses Konzepts hilft, die Methode zu verstehen, so dass Sie später in der Lage sein werden, das 32-mal-64-Bit-Multiplikationsproblem zu lösen.

     --{{2}}--
Die Mathematik dafür ist einfach, ein 16-Bit-Binär sind einfach zwei 8-Bit-Binäre, wobei der höchstwertige dieser beiden mit dezimal 256 oder hex 100 multipliziert wird. Das 16-Bit-Binär $m1$ ist also gleich $256*m1_H$ plus $m1_L$, wobei $m1_H$ das MSB und $m1_L$ das LSB ist. Die Multiplikation von $m1$ mit dem 8-Bit-Binär $m2$ ist also, mathematisch formuliert:

$$
m1 \cdot m2 = (256 \cdot m1_H + m1_L) \cdot m2 = 256 \cdot m1_H \cdot m2 + m1_L \cdot m2
$$

Welche Abschnitte sind in der Brechnung notwendig?

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii
    +--------+          +--------+--------+
    | "$m2$" | "$\cdot$"|"$m1_H$"|"$m1_L$"|
    +--------+          +--------+--------+
    ---------------------------------------
                        +-----------------+
                        |"$m2 \cdot m1_L$"|     
                        +-----------------+
               +-----------------+
               |"$m2 \cdot m1_H$"|     
    +          +-----------------+
    ---------------------------------------
               +--------------------------+  
               |      "$m2 \cdot m1$"     |   <- 24 Bit Ergebnis
               +--------------------------+                                    .
```


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
%A0  r19 |   0xF0   | Ergebnis  |       
         +----------+ 2093040   ⎬ "$m1_L\cdot m2$"-.
%B0  r20 |   0xEF   |           |                  |   
         +----------+          -.                  ⎬ "$m1_H \cdot m2$"
%C0  r21 |   0x1F   |                              |                 carry
         +----------+                             -.
%D0  r22 |   0x00   |      0
         +----------+                                                               .
```

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
  Serial.flush();

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```

Für die Muliplikation von größeren Werten wird die Berechnung entsprechend aufwändiger.

*******************************************************************

[^AtTinyArchitecture]: Firma Microchip, Handbuch AtTiny Family, https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf

### Fehlende Fließkommaeinheit

Die Gleitkommadarstellung besteht dann aus dem Vorzeichen, der Mantisse und dem Exponenten. Für binäre Zahlen ist diese Darstellung in der [IEEE 754](https://de.wikipedia.org/wiki/IEEE_754) genormt.

<!--
style="width: 100%; max-width: 560px; display: block; margin-left: auto; margin-right: auto;"
-->
```ascii
  +-+---- ~ -----+----- ~ ----+
  |V|  Mantisse  |  Exponent  |   V=Vorzeichenbit
  +-+---- ~ -----+----- ~ ----+

   1      23           8          = 32 Bit (float)
   1      52          11          = 64 Bit (double)                            .
```

> **Merke:** Die Verrechnung von Gleitkommazahlen ist entsprechend aufwändig:
>
> 1. Homogenisierung der Exponenten und Mantissen
> 2. Berechnung des Ergebnisses
> 3. Normierung des Resultats

![](https://www.researchgate.net/profile/Peter-Wilson-14/publication/224698133/figure/fig3/AS:669064468893710@1536528808587/Floating-Point-Adder-Architecture_W640.jpg "Floating Point Adder Architecture - Research Gate - Color Edge Detection Hardware based on Geometric Algebra. https://www.researchgate.net/publication/224698133_Color_Edge_Detection_Hardware_based_on_Geometric_Algebra")<!-- style="width: 100%; max-width: 560px" -->


<iframe width="100%" height="60%" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:2,endLineNumber:22,positionColumn:2,positionLineNumber:22,selectionStartColumn:2,selectionStartLineNumber:22,startColumn:2,startLineNumber:22),source:'%23define+F_CPU+16000000UL%0A%0A%23include+%3Cavr/io.h%3E%0A%0Aint+main+(void)+%7B%0A%0A++char+a+%3D+5%3B%0A++char+b+%3D+6%3B%0A++char+c+%3D+0%3B%0A%0A++//float+a+%3D+5.1%3B%0A++//float+b+%3D+6.3%3B%0A++//float+c+%3D+0%3B%0A%0A++for+(int+i+%3D+1%3B+i+%3C+11%3B+%2B%2Bi)%0A++%7B%0A++++c+%3D+a+%2B+b%3B%0A++++if+(c%3E15)+break%3B%0A++%7D%0A%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:avrg930,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+AVR+gcc+9.3.0+(Editor+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

> Ändern Sie die Operation in Zeile 17 von einer Addition in eine Mulitplikation. Was beobachten Sie und warum?
> Welche Änderungen beobachten Sie, wenn Sie den Datentyp auf `float` setzen?


###  Fehlende Festkommaeinheit

Neben den Fließkommadarstellungen lassen sich auch Festkommakonzepte für die Darstellung gebrochener Zahlen in Hardware/Software umsetzen. Dabei wird die Speicherbreite in den Anteil vor und nach einer spezifischen und unveränderlichen Kommaposition eingeteilt.

Ein Beschreibungsformat dafür ist die Q-Notation bei der die Anzahl der Nachkommastellen (und optional die Anzahl der ganzzahligen Bits) angegeben wird. Eine Q15-Zahl hat z. B. 15 Nachkommastellen; eine Q1.14-Zahl hat 1 ganzzahliges Bit und 14 Nachkommastellen.

> **Achtung:** Für vorzeichenbehaftete Festkommazahlen gibt es zwei widersprüchliche Verwendungen des Q-Formats. Bei der einen Verwendung wird das Vorzeichenbit als Ganzzahlbit gezählt, in der anderen Variante jedoch nicht. Zum Beispiel könnte eine vorzeichenbehaftete 16-Bit-Ganzzahl als Q16.0 oder Q15.0 bezeichnet werden. Um diese Unklarheit zu beseitigen wird teilweise ein U für `unsigned` eingefügt.

<!-- data-type="none" -->
| Konfiguration | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Wert  |
| ------------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| UQ0.8         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 0.875 |
| UQ1.7         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 1.75  |
| UQ2.6         | 1     | 1     | 1     | 0     | 0     | 0     | 0     | 0     | 3.5   |

<!-- data-type="none" -->
| Konfiguration | Auflösung | größte Zahl      | kleinste Zahl |
| ------------- | --------- | ---------------- | ------------- |
| `Qm.n`        | $2^{-n}$  | $2^{m-1}-2^{-n}$ | $-2^{m-1}$    |
| `UQm.n`       | $2^{-n}$  | $2^{m}-2^{-n}$   | $0$           |

Eine 16 Bit breite, vorzeichenbehaftete Festkommazahl `Q15.1` kann also Zahlenwerte im Bereich $[-16384.0, +16383.5]$ abbilden. Die Auflösung der Darstellung ist $2^{-n} = 0.5$

> **Merke:** Anders als für eine Fließkommazahl, ist die Auflösung der Festkommazahl konstant!

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

<!-- data-type="none" -->
| Typname | Typ       | Größe in Byte | QU    | Q          |
| ------- | --------- | ------------- | ----- | ---------- |
| _Fract  | short     | 1             | 0.8   | $\pm$0.7   |
|         | long      | 4             | 0.32  | $\pm$0.31  |
|         | long long | 8             | 0.64  | $\pm$0.63  |
| _Accum  | short     | 2             | 8.8   | $\pm$8.7   |
|         | long      | 4             | 32.32 | $\pm$32.31 |
|         | long long | 8             | 16.48 | $\pm$16.47 |

> **Merke:** Daneben existieren verschiedene andere Festkommabibliotheken, die andere Konfigurationen unterstützen und verschiedene Implementierungen aufzeigen.

Lassen Sie uns einen genaueren Blick auf die Implementierung werfen. Im Codebeispiel, dass Sie im Projektordner finden, addieren wir zwei Variablen unterschiedlichen Formates.

```c    FixedPoint.c
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdfix.h>

int main (void) {

  unsigned short _Accum fixVarA = 1.5K;
  short _Accum fixVarB =  -1.5K;
  long _Accum fixResult = fixVarA * fixVarB;

  while(1);
  return 0;
}
```

Für die `variableA` ergibt sich dabei folgender Auszug des Programmspeichers, sofern das Beispielprogramm ohne Optimierung übersetzt wird.

```
short _Accum fixVarB =  -1.5K;
11c:	80 e4       	ldi	r24, 0x40	; 64
11e:	9f ef       	ldi	r25, 0xFF	; 255

     +--------+
r24  |01000000|     r25
     +--------+   --------
r25  |11111111| = 111111110.1000000
     +--------+           ---------
                             r24

 1.5 = (3 >> 1)

  0011 = 3
  1100 invertiert  
  0001 +1
  ----
  1101 = -3   --> 110.1 == -1.5                           
```

### Vergleich der Softwarelösungen auf dem AVR

Um eine Evaluation durchzuführen wurde der Python Wrapper `pysimavr` für die AVR Core Simulation genutzt.

https://github.com/buserror/simavr

Im Projektordern finden Sie unter `../codeExamples/avr/fixedPoint/pySimAVR` das Miniprojekt. Dabei sind zwei Beispiele vorgesehen:

+ Evaluation der Laufzeit mittels UART Ausgaben
+ Evaluation der Laufzeit über togglende Pins

Im Ergebnis zeigt sich folgendes Bild:

<!-- data-type="none" -->
| Variable                | Dauer           |
| ----------------------- | --------------- |
| `_delay_ms (100);`      | 100000.12500 us |
| `unsigned short _Accum` | 2771.68700 us   |
| `unsigned long _Accum`  | 45760.37500 us  |
| `long _Accum`           | 50463.25000 us  |

## Analog-Digital-Wandler Wiederholung

### Motivation

In der vorangegangenen Vorlesung sprachen wir insbesondere über die Erfassung
von digitalen Signalen. Eine Erfassung von analogen Werten ist allerdings notwendig, um Phänomene der Umgebung mit dem notwendigen Detailgrad beobachten zu können.  

![Bild](../images/04_ADC/Herausforderung.png)

Dabei wird das zeit- und wertkontinuierliche Eingangssignal in eine zeit- und wertdiskrete Darstellung überführt.

### Analog Komperator

Ein Komparator ist eine elektronische Schaltung, die zwei Spannungen vergleicht. Der Ausgang zeigt in binärer/digitaler Form an, welche der beiden Eingangsspannungen höher ist. Damit handelt es sich praktisch um einen 1-Bit-Analog-Digital-Umsetzer.

![Bild](../images/04_ADC/Op-amp_symbol.svg.png "Symbol eines Operationsverstärkers")<!-- style="width: 35%; max-width: 600px" -->

![Bild](../images/04_ADC/Comperator.png "Grundsätzliche Beschaltung eines Operationsversträrkers bei der Verwendung als ADC [^WikipediaOmegatron]" )<!-- style="width: 55%; max-width: 600px" -->

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii
                   ^
 Ideales       U_a |------------+
 Verhalten         |            |
                   |            |
                   |            |             U_i
                   +------------+------------->
                   |            | U_ref
                   |            |
                   |            |
                   |............+------------                                                .

                   ^
 Reales        U_a |--------+
 Verhalten         |        !\  
                   |        ! \  
                   |        !  \ U_ref        U_i
                   +--------!---+------------->
                   |        !    \  !
                   |        !     \ !  
                   |        !      \!
                   |........!.......+-----------                                                .

                            Hysterese
```

Im AVR findet sich ein Komperator, der unterschiedliche Eingänge miteinander vergleichen kann:
Für "+" sind dies die `BANDGAP Reference` und der Eingang `AIN0` und für "-" der Pin `AIN1` sowie alle analogen Eingänge.  

![Bild](../images/04_ADC/AnalogCompAVR.png "Comperator Konfiguration im ATmega [^AtmelHandbuch]")<!-- style="width: 85%; max-width: 1000px" -->

Die grundlegende Konfiguration erfolgt über die Konfiguration der Bits / Register :

| Bits                   | Register | Bedeutung                                              |
| ---------------------- | -------- | ------------------------------------------------------ |
| `ACBG`                 | `ACSR`   | Analog Comparator Bandgap Select                       |
| `ACME`                 | `ADCSRB` | Analog Comparator Multiplexer Enable - Bit im Register |
| `ADEN`                 | `ADCSRA` | Analog Digital Enable                                  |
| `MUX2`, `MUX1`, `MUX0` | `ADMUX`  | Mulitiplexer Analog input                              |

Dazu kommen noch weitere Parameterisierungen bezüglich der Interrupts, der Aktivitierung von Timerfunktionalität oder der Synchronisierung.

Weitere Erläuterungen finden Sie im Handbuch auf Seite

> **Aufgabe:** An welchen Pins eines Arduino Uno Boards müssen Analoge Eingänge angeschlossen werden, um die zwei Signale mit dem Komperator zu vergleichen. Nutzen Sie den Belegungsplan (Schematics) des Kontrollers, der unter [Link](https://store.arduino.cc/arduino-uno-rev3) zu finden ist.

Ein Beispiel für den Vergleich eines Infrarot Distanzsensors mit einem fest vorgegebenen Spannungswert findet sich im _Example_ Ordner der Veranstaltung.

![Bild](../images/04_ADC/ComperatorBeispiel.png)<!-- style="width: 85%; max-width: 1000px" -->

```cpp
#define F_CPU 16000000UL
#include <avr/io.h>

int main()
{
  ADCSRB = (1<<ACME);
  DDRB = (1<<PB5);

  while(1)
  {
    if (ACSR & (1<<ACO))/* Check ACO bit of ACSR register */
       PORTB &= ~(1 << PB5); /* Then turn OFF PB5 pin */
    else    /* If ACO bit is zero */
        PORTB = (1<<PB5); /* Turn ON PB5 pin */
  }
}
```

[^AtmelHandbuch]: Firma Microchip, megaAVR® Data Sheet, Seite 243, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf)

[^WikipediaOmegatron]: Wikipedia, Autor Omegatron - Eigenes Werk, CC BY-SA 3.0, https://commons.wikimedia.org/w/index.php?curid=983276

### Analog Digital Wandler

__Flashwandler__

![Bild](../images/04_ADC/FlashWandler.png)<!-- style="width: 85%; max-width: 1000px" -->

Vorteil

+ Hohe Geschwindigkeit

Nachteil

+ Energieverbrauch größer
+ Hardwareaufwand für höhere Auflösungen

https://www.youtube.com/watch?v=x7oPVWLD59Y

__Sequenzielle Wandler__

Sequenzielle Wandler umgehen die Notwendigkeit mehrerer Komperatoren, in dem das Referenzsignal variabel erzeugt wird.

![Bild](../images/04_ADC/SequenziellerWandler.png)<!-- style="width: 85%; max-width: 1000px" -->

Vorteil

+ sehr hohe Auflösungen möglich
+ Schaltung einfach umsetzbar – kritisches Element DAC/Komperator

Nachteil

+ Variierende Wandlungsdauer
+ langsam (verglichen mit dem Flashwandler)


### Herausforderungen bei der Wandlung

**Fehlertypen**

![Bild](../images/04_ADC/Fehler.png)<!-- style="width: 85%; max-width: 1000px" -->

+ Quantisierungsfehler sind bedingt durch die Auflösung des Wandlers
+ Offsetfehler ergeben sich aus einer Abweichung der Referenzspannung und führen zu einem konstanten Fehler.
+ Verstärkungsfehler im Analog-Digitalwandler wirken einen wertabhängigen Fehler.
+ Der Linearitätsfehler ist die Abweichung von der Geraden. Linearitätsfehler lassen sich nicht abgleichen.

> **Merke:** Die Fehlerparameter hängen in starkem Maße von der Konfiguration des Wandlers (Sample Frequenz, Arbeitsbreite, Umgebungstemperatur) ab!

![Bild](../images/04_ADC/Linearitätsfehler.png "Datenblatt eines 8 Bit Wandlers der TLC0831 [^TIDatenblatt]")

**Referenzspannung**

Eine Herausforderung liegt in der stabilen Bereitstellung der Referenzspannung für den Analog-Digital-Wandler.

### Parameter eines Analog-Digital-Wandlers

+ Auflösung
+ Messdauer
+ Leistungsaufnahme
+ Stabilität der Referenzspannung
+ Unipolare/Bipolare Messungen
+ Zahl der Eingänge
+ Ausgangsinterfaces (parallele Pins, Bus)
+ Temperaturabhängigkeit und Rauschverhalten (Gain, Nicht-Linearität, Offset)

[^TIDatenblatt]: Firma Texas Instruments, Datenblatt AD-Wandler 8 Bit DIL-8, TLC0831, TLC0831IP

### Umsetzung im AVR

| Handbuch des Atmega328p                             | Bedeutung                                                                                       |
| --------------------------------------------------- | ----------------------------------------------------------------------------------------------- |
| 10-Bit Auflösung                                    |                                                                                                 |
| 0.5 LSB Integral Non-Linearity                      | maximale Abweichung zwischen der idealen und der eigentlichen analogen Signalverlauf am Wandler |
| +/- 2 LSB Absolute Genauigkeit                      | Summe der Fehler inklusive Quantisierungsfehler, Offset Fehler etc.  (worst case Situation)     |
| 13 - 260μs Conversion Time                          | Die Dauer der Wandlung hängt von der Auflösung und der der vorgegebenen Taktrate  ab.                                                                                               |
| Up to 76.9kSPS (Up to 15kSPS at Maximum Resolution) |                                                                                                 |
| 0 - V CC ADC Input Voltage Range                    |  Es sind keine negativen Spannungen möglich.                                                                                               |
| Temperature Sensor Input Channel                    |                                                                                                 |
| Sleep Mode Noise Canceler                                                    |    Reduzierung des Steuquellen durch einen "Sleepmode" für die CPU                                                                                             |

![Bild](../images/04_ADC/AVR_ADC.png "Strukturdarstellung des AD-Wandlers im ATmega [^HandbuchAtmega]")

**Trigger für den Wandlung**

Grundsätzlich sind 3 Modi für die Wandlung möglich:

+ Programmgetriggerte Ausführung der Wandlung
+ Kontinuierliche Wandlung
+ ereignisgetriebener Start

![Bild](../images/04_ADC/ADCTrigger.png "Trigger des AD-Wandlers im ATmega [^HandbuchAtmega](Seite 247)")

![Bild](../images/04_ADC/TimeLineADC.png "Zeitlicher Verlauf einer AD-Wandlung  [^HandbuchAtmega](Seite 250)")

**Ergebnisregister**

Die Atmega Prozessoren bieten eine Auflösung von 10Bit oder 8Bit für die analogen Wandlungen. Entsprechend stehen zwei Register `ADCL` und `ADCH` für die Speicherung bereit. Standardmäßig (d.h. `ADLAR == 0`) werden die niederwertigsten 8 im Register `ADCL` bereitgehalten und die zwei höherwertigsten im Register `ADCH`.

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii
             ADCH                                   ADCL
  +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
                            9   8       7   6   5   4   3   2   1   0
```

Das Ergebnis ergibt sich dann zu

```c
uint8_t theLowADC = ADCL
uint16_t theTenBitResults = ADCH<<8 | theLowADC;
```

Ist keine 10-bit Genauigkeit erforderlich, wird diese Zuordnung durch das Setzen des `ADLAR` Bits im `ADMUX` Register angepasst. Auf diese Weise kann das ADC Ergebnis direkt als 8 Bit Zahl aus `ADCH` ausgelesen werden.

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii
             ADCH                                   ADCL
  +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
    9   8   7   6   5   4   3   2       1   0
```

> **Merke: ** Immer zuerst ADCL und erst dann ADCH auslesen.

Beim Zugriff auf ADCL wird das ADCH Register gegenüber Veränderungen vom ADC gesperrt. Erst beim nächsten Auslesen des ADCH-Registers wird diese Sperre wieder aufgehoben. Dadurch ist sichergestellt, dass die Inhalte von ADCL und ADCH immer aus demselben Wandlungsergebnis stammen, selbst wenn der ADC im Hintergrund im Free-Conversion-Mode arbeitet.

[^HandbuchAtmega]: Firma Microchip, megaAVR® Data Sheet, Seite 247, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf)

### Beispiele


**Beispiel 1 - Lesen eines Analogen Distanzsensors**

Für das Beispiel wird der AtMega2560 verwendet, der eine interne Referenzspannung von 2.56 V anstatt der des AtMega328 von 1.1 V bereit stellt.

![Bild](../images/04_ADC/AtMega2560Refs.png "Referenzsystem des Analog-Digitalwandlers [^HandbuchAtmega](Seite 281)")

Die Bedeutung ergibt sich beim Blick ins Datenblatt des Sensors GP2D, dessen Maximalwertausgabewert liegt bei etwa 2.55V

```c
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

int readADC(int channel) {
  int i; int result = 0;
  // Den ADC aktivieren und Teilungsfaktor auf 64 stellen
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
  // Kanal des Multiplexers & Interne Referenzspannung (2,56 V)
  ADMUX = channel | (1<<REFS1) | (1<<REFS0);
  // Den ADC initialisieren und einen sog. Dummyreadout machen
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC));
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC)); // Auf Ergebnis warten...
  // Lesen des egisters "ADCW" takes care of how to read ADCL and ADCH.
  result = ADCW;
  // ADC wieder deaktivieren
  ADCSRA = 0;
  return result;
}

int main(void)
{
  Serial.begin(9600);
  while (1) //infinite loop
  {
    int result_individual = readADC(0);
    Serial.println(result_individual);
    Serial.flush();
    _delay_ms(10); //1 second delay
  }
  return  0; // wird nie erreicht
}
```

> _The first ADC conversion result after switching reference voltage source may be inaccurate, and the user is advised to discard this result._ Handbuch Seite 252

**Beispiel 2 - Temperaturüberwachung des Controllers**

> _The temperature measurement is based on an on-chip temperature sensor that is coupled to a single ended ADC8 channel. Selecting the ADC8 channel by writing the MUX3...0 bits in ADMUX register to "1000" enables the temperature sensor. The internal 1.1V voltage reference must also be selected for the ADC voltage reference source in the temperature sensor measurement. When the temperature sensor is enabled, the ADC converter can be used in single conversion mode to measure the voltage over the temperature sensor._ Handbuch Seite 256

```c
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

double getTemp(void)
{
    unsigned int wADC;
    double t;

    // Set the internal reference and mux.
    ADMUX = (1<<REFS1) | (1<<REFS0) | (1<<MUX3);
    ADCSRA |= (1<<ADEN);  // enable the ADC

    // wait for voltages to become stable.
    delay(20);

    // Start the ADC
    ADCSRA |= (1<<ADSC);

    // Detect end-of-conversion
    while (ADCSRA & (1<<ADSC));
    wADC = ADCW;

    // The offset of 324.31 could be wrong. It is just an indication.
    t = (wADC - 324.31 ) / 1.22;

    // The returned temperature is in degrees Celsius.
    return (t);
}

int main(void)
{
  Serial.begin(9600);
  while (1)
  {
    Serial.println(getTemp());
    Serial.flush();
    _delay_ms(10); //1 second delay
  }
  return  0; // wird nie erreicht
}
```

## Aufgaben

- [ ] Integrieren Sie die Berechnung im Beispiel vcdBased auf Basis von `float` und `double` Werten
- [ ] Reimplementieren Sie die Nutzung des Temperatursensors aus Ihrer "Bastelbox", so dass bis auf die Serielle Kommunikation keine Arduino-Bibliotheken mehr genutzt werden.
- [ ] Nutzen Sie die Infrarot-Distanzsensoren aus Ihrer Box, um eine Entfernungsmessung zu Implementieren. Für die Ausgabe sollten Sie beide Varianten der Linearsierung nutzen - Look-up-Table und Funktionsdarstellung.
