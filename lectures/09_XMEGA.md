<!--
author:   Sebastian Zug, Karl Fessel & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de

version:  1.0.0
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md
         https://github.com/LiaTemplates/Pyodide

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg

mark: <span style="background-color: @0;
                                  display: flex;
                                  width: calc(100% + 32px);
                                  margin: -16px;
                                  padding: 6px 16px 6px 16px;
                                  ">@1</span>
red:  @mark(#FF888888,@0)
blue: @mark(lightblue,@0)
gray: @mark(gray,@0)

-->


[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/lectures/09_XMEGA.md#1)


# Erweiterte AVR Architekturen

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Vorlesung Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2021`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Erweiternde Architekturkonzepte der XMEGA Architektur`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/09_XMEGA.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/09_XMEGA.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/26gR2qGRnxxXAvhBu/giphy.gif)

---

## Ausgangspunkt

![alt-text](../images/09_megaAVR_0/Controller8Bit.png "Speicherstruktur des ATMega2560 [^ATmega640] Seite 20")

Welche Erweiterungen ergeben sich dabei:

+ alle GPIOs können als externe Interrupts genutzt werden
+ die Interruptvektortabelle ist nicht mehr fest vordefiniert sondern kann an die Anwendung angepasst werden.
+ ein Eventsystem erlaubt die Verknüpfung von peripheren Elementen untereinander, ohne dass die CPU eingreifen muss
+ die Taktgeber werden nicht mehr über Fuse-Bits gesetzt sondern können über entsprechende Register konfiguriert werden.
+ eine konfigurierbare Logik (CCL) verbindet Eingänge, Perepheriebauteile und Ausgänge mit sequenziellen Schaltwerken
+ für den Analog-Digitalwandler stehen 5 interne Referenzspannungen bereit.
+ der Controller integriert einen internen Real-Time Oszillator mit 32.768 Hz
+ Softwareresets sind über ein eigenes Register möglich.
+ UPDI ersetzt die bisherige OneWire Debug Schnittstelle

![alt-text](../images/09_megaAVR_0/Blockdiagram.png "Speicherstruktur des ATMega2560 [^ATmega640] Seite 20")




![alt-text](../images/09_megaAVR_0/Arduino_UNO_WIFI.jpg "Atmel ATmega4809 auf dem Arduino Uno Wifi Rev. 2" )


[^Microchip4809]: Firma Microchip, ATmega4808/4809 Data Sheet, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)

## Hardware

![alt-text](../images/09_megaAVR_0/ClockSystem.png "Clock-System des ATmega4809 [^Microchip4809] Seite 85")

Der Haupttakt versorgt die CPU, den RAM, den Flash, den I/O-Bus und allen am I/O-Bus angeschlossenen Peripheriegeräten und wird vom Taktcontroller vorskaliert und verteilt.
Dem asynchronen Takt folgen die Realtime-Clock (RTC), der Watchdog-Timer (WDT), die Brown-out-Detection (BOD) und asynchrone Timer Counter (TCD). Die asynchronen Taktquellen werden über Register in der jeweiligen Peripherie konfiguriert.

Als Taktquellen sind vorgesehen:

+ Interne Oszillatoren (16/20 MHz Oszillator, 32KHz Oszillator)
+ Externe Ozillatoren (via External Clock Pin, 32.768 kHz Quarz Oszillator)

Die Konfiguration wird über zwei Register `MCLKCTRLA` (Taktresource) und `MCLKCTRLB` (Prescaler) vorgenommen.

[^Microchip4809]: Firma Microchip, ATmega4808/4809 Data Sheet, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)

### Variable Konfiguration von Pin-Belegungen

Ein zentraler Unterschied des bishering ATmega328 zum ATmega4809 ist die möglichkeit der Variablen Zuordnung bestimmter Funktionalitäten zu einzelnen Pins. Der Port-Multiplexer (`PORTMUX`) kann entweder die Funktionalität von Pins aktivieren oder deaktivieren, oder zwischen Standard- und alternativen Pin-Positionen.

![alt-text](../images/09_megaAVR_0/IOMultiplexing.png "IO-Multiplexing des ATmega4809 [^Microchip4809] Seite 18" )

Die Grafik zeigt, das zum Beispiel das der USART3 standardmäßig mit PB0 und PB1 verknüpft ist. Durch die Rekonfiguration im `PORTMUX` Register kann diese Zuordnung nach PB4 und PB5 verschoben werden.

> Recherchieren Sie die Beschaltung der Seriellen Schnittstelle des aktuellen Boards. Werden hier die alternativen Pins benutzt? Welche Konfigurationen sind entsprechend zu treffen?

![alt-text](../images/09_megaAVR_0/PORTMUX_Register.png "PORTMUX_Register [^Microchip4809] Seite 134" )

| Bezeichnung | Bedeutung                        |
| ----------- | -------------------------------- |
| `EVOUTx`    | Event Output Pin                 |
| `LUTn`      | Look-Up Tables Output Pins       |
| `USARTn`    |                                  |
| `TWIn`      | I2C Schnittstellen Pins          |
| `SPIn`      | Serial Peripheral Interface Pins |
| `TCAn`      | Timer Counter A Output           |
| `TCBn`      | Timer Counter B Output           |

> **Frage:** Warum brauchen wir 6 Ausgabekanäle für lediglich 3 Compare-Einheiten?

![alt-text](../images/09_megaAVR_0/TimerCounterA_16Bit.png "Timer-Counter Modul A des ATmega4809 [^Microchip4809] Seite 187" )

Die 16 Bit Counter des 4809 können in einen 8bit Modus umgeschalten werden.

![alt-text](../images/09_megaAVR_0/TimerCounterA_8Bit.png "8 Bit Modus des Timer-Counter Modul A [^Microchip4809] Seite 187" )

[^Microchip4809]: Firma Microchip, ATmega4808/4809 Data Sheet, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)

### Interrupts

> **Achtung:** Das Interruptsystem des 4809 unterscheidet sich deutlich von dem des ATmega328. Bisher waren wir dort auf eine statisch konfigurierte Priorisierung angewiesen.

![alt-text](../images/09_megaAVR_0/InterruptSystem.png "Interruptsystem des 4809 [^Microchip4809] Seite 111" )
Die Interrupt-Erzeugung muss global aktiviert werden, indem eine '1' in das Global Interrupt Enable Bit (I) im CPU-Statusregister `CPU.SREG` geschrieben wird. Dieses Bit wird nicht gelöscht, wenn ein Interrupt quittiert wird.

![alt-text](../images/09_megaAVR_0/InterruptSequence.png "Ablauf der Interruptverarbeitung [^Microchip4809] Seite 114" )
Wenn ein Interrupt aktiviert ist und die Interrupt-Bedingung eintritt, empfängt die CPUINT die Interrupt-Anforderung. Wenn eine Interrupt-Anforderung von der CPUINT bestätigt wird, wird der Programmzähler so gesetzt, dass er auf den Interrupt-Vektor zeigt. Der Interrupt-Vektor ist ein Sprung zum Interrupt-Handler. Nach der Rückkehr vom Interrupt-Handler wird die Programmausführung an der Stelle fortgesetzt, an der sie vor dem Auftreten der Unterbrechung war.

Standardmäßig haben alle Peripheriegeräte die Prioritätsstufe 0. Es ist möglich, eine Interrupt-Anforderung der Stufe 1 (hohe Priorität) zuzuordnen, indem Sie ihre Interrupt-Vektornummer in das `CPUINT.LVL1VEC`-Register schreibt. Diese Interrupt-Anforderung hat dann eine höhere Priorität als die anderen (normal priorisierten) Interrupt-Anforderungen Anforderungen.

| Priorität | Level                          | Quelle                 |
| --------- | ------------------------------ | ---------------------- |
| Höchste   | _Non Maskable Interrupt_ (NMI) | für 4809 nur CRC check |
| Hohe      | Level 1                        |                        |
| Niedrige  | Level 0                        |                        |   

Interrupts werden entsprechend ihrer Prioritätsstufe UND ihrer Interruptvektoradresse (vgl. Datenblatt Seite 66) priorisiert. Interrupts der Prioritätsstufe 1 unterbrechen Interrupthandler der Prioritätsstufe 0. Bei Interrupts der Prioritätsstufe 0 wird die Priorität anhand der Interruptvektoradresse ermittelt, wobei die niedrigste Interruptvektoradresse die höchste Interruptpriorität hat.

![alt-text](../images/09_megaAVR_0/InterruptScheduling.png "Interruptsystem des 4809 [^Microchip4809] Seite 116" )

Optional kann für Interrupts der Prioritätsstufe 0 ein Round-Robin-Schema aktiviert werden. Dadurch wird sichergestellt, dass alle Interrupts innerhalb einer bestimmten Zeitspanne bearbeitet werden.

### Konfigurierbare Logik

Die konfigurierbare benutzerdefinierte Logik (CCL) ist eine programmierbare Logik-Peripherie, die mit den Geräte Pins, an Ereignisse oder an andere interne Peripherie angeschlossen werden kann. Die CCL kann als "Klebelogik" zwischen der Geräteperipherie und externen Geräten dienen.

#### Exkurs - Schaltwerke

Sequentieller Binärzahlenvergleicher - zwei Zahlenwerte werden sequenziell entsprechend ihren Stellen durch den Vergleicher bewegt und verglichen. Das Schaltwerk speichert das Resultat sobald ein Wert größer als der andere .

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii

           <------ Linksshift
         +---+---+---+---+---+---+---+
  +----- | 1 | 1 | 0 | 0 | 1 | 0 | 1 |   Shiftregister A
  |      +---+---+---+---+---+---+---+
  |
  |
  |      +---+---+---+---+---+---+---+
  |  +-- | 1 | 1 | 0 | 1 | 1 | 1 | 0 |   Shiftregister B
  |  |   +---+---+---+---+---+---+---+
  |  |
  |  |   +-------------+               +--------------------+----+----+
  |  |   |             |               |                    | R1 | R2 |
  |  +-> | Vergleicher | --> R1        +--------------------+----+----+
  |      |             |               | Equal   E: A==B    | 0  | 0  |
  |      |             |               | Greater G: A"$>$"B | 1  | 0  |
  +----> |             | --> R2        | Less    L: A"$<$"B | 0  | 1  |
         |             |               +--------------------+----+----+
         +-------------+                                                       .
```

**1. Schritt: Aufgabenspezifikation, Erstellen eines Zustandsdiagramms**

Für die Aufgabe ergibt sich folgender Graph:

```text @plantUML
@startuml
digraph finite_state_machine {
    rankdir=LR;

    node [shape = point ]; qi
    node[shape=circle]
    E[label="E"];
    G[label="G"];
    L[label="L"];


    qi -> E;
    E  -> E  [ label = "AB={00, 11}" ];
    E  -> G  [ label = "AB=10" ];
    G  -> G  [ label = "AB={00, 01, \n10, 11}"];
    E  -> L  [ label = "AB=01" ];
    L  -> L  [ label = "AB={00, 01, \n10, 11}"];
}
@enduml
```
@plantUML

Im Beispiel liegt ein Medwedew-Automat vor. Die Zustände werden direkt auf den Ausgang abgebildet.

**2. Schritt: Erstellen der Zustandstabelle**

Hier wäre eine Zustandstabelle denkbar, die alle Eingangskombinationen mit allen Zuständen zeilenweise verknüpft.

<!-- data-type="none" -->
| aktueller Zustand | A   | B   | Folgezustand |
| ----------------- | --- | --- | ------------ |
| E                 | 0   | 0   | E            |
| E                 | 0   | 1   | L            |
| E                 | 1   | 0   | G            |
| ...               |     |     |              |

Eine kompaktere Darstellung fasst die Kombinationen der Eingänge zusammen und ordnet sie den Folgezuständen zu.

<!-- data-type="none" -->
| aktueller Zustand | AB==00 | AB==01 | AB==10 | AB==11 |
| ----------------- | ------ | ------ | ------ | ------ |
| E                 | E      | L      | G      | E      |
| G                 | G      | G      | G      | G      |
| L                 | L      | L      | L      | L      |

**Schritt 3: Auswahl einer binären Zustandskodierung und Generierung einer binären Zustandstabelle**

Insgesamt sind 3 Zustände zu kodieren, entsprechend werden wiederum 2 Flip-Flops benötigt. Dabei wird die Kodierung wie folgt vorgenommen:

<!-- data-type="none" -->
| Zustand | F   | G   |
| ------- | --- | --- |
| E       | 0   | 0   |
| G       | 0   | 1   |
| L       | 1   | 0   |

Damit ergibt sich folgende Binäre Zustandstabelle

<!-- data-type="none" -->
| aktueller Zustand | AB==00 | AB==01 | AB==10 | AB==11 |
| ----------------- | ------ | ------ | ------ | ------ |
| 00                | 00     | 10     | 01     | 00     |
| 01                | 01     | 01     | 01     | 01     |
| 10                | 10     | 10     | 10     | 10     |

In der traditionellen Darstellung zeigt sich diese wie folgt:

<!-- data-type="none" -->
| $F_{t}$ | $G_{t}$ | $A_{t}$ | $B_{t}$ | @gray($F_{t+1}$) | @gray($G_{t+1}$) |
| ------- | ------- | ------- | ------- | ---------------- | ---------------- |
| 0       | 0       | 0       | 0       | @gray(0)         | @gray(0)         |
| 0       | 0       | 0       | 1       | @gray(1)         | @gray(0)         |
| 0       | 0       | 1       | 0       | @gray(0)         | @gray(1)         |
| 0       | 0       | 1       | 1       | @gray(0)         | @gray(0)         |
| 0       | 1       | 0       | 0       | @gray(0)         | @gray(1)         |
| 0       | 1       | 0       | 1       | @gray(0)         | @gray(1)         |
| 0       | 1       | 1       | 0       | @gray(0)         | @gray(1)         |
| 0       | 1       | 1       | 1       | @gray(0)         | @gray(1)         |
| 1       | 0       | 0       | 0       | @gray(1)         | @gray(0)         |
| 1       | 0       | 0       | 1       | @gray(1)         | @gray(0)         |
| 1       | 0       | 1       | 0       | @gray(1)         | @gray(0)         |
| 1       | 0       | 1       | 1       | @gray(1)         | @gray(0)         |
| 1       | 1       | 0       | 0       | @gray(D)         | @gray(D)         |
| 1       | 1       | 0       | 1       | @gray(D)         | @gray(D)         |
| 1       | 1       | 1       | 0       | @gray(D)         | @gray(D)         |
| 1       | 1       | 1       | 1       | @gray(D)         | @gray(D)         |

**Schritt 4: Auswahl eines Flip-Flop Typs und Ermittlung der für jeden Zustandsübergang benötigten Flip-Flop Ansteuerungen**

Wir entscheiden uns für einen D Flip-Flop für die Realisierung. Die entsprechende invertierte Wahrheitstafel haben Sie zwischenzeitlich im Kopf:

<!-- data-type="none" -->
| $Q(t)$ | $Q(t+1)$ | $D$ |
| ------ | -------- | --- |
| $0$    | $0$      | $0$ |
| $0$    | $1$      | $1$ |
| $1$    | $0$      | $0$ |
| $1$    | $1$      | $1$ |

Damit lässt sich die Zustandsübergangstabelle entsprechend einfach um die zugehörige Eingangsbelegung ergänzen. Für die D-Flip-Flops ist dies einfach eine Kopie der Zustandsspalten.

<!-- data-type="none" -->
| $F_{t}$ | $G_{t}$ | $A_{t}$ | $B_{t}$ | @gray($F_{t+1}$) | @gray($G_{t+1}$) | $DF$    | $DG$     |
| ------- | ------- | ------- | ------- | ---------------- | ---------------- | ------- | -------- |
| 0       | 0       | 0       | 0       | @gray(0)         | @gray(0)         | 0       | 0        |
| 0       | 0       | 0       | 1       | @gray(1)         | @gray(0)         | @red(1) | 0        |
| 0       | 0       | 1       | 0       | @gray(0)         | @gray(1)         | 0       | @blue(1) |
| 0       | 0       | 1       | 1       | @gray(0)         | @gray(0)         | 0       | 0        |
| 0       | 1       | 0       | 0       | @gray(0)         | @gray(1)         | 0       | @blue(1) |
| 0       | 1       | 0       | 1       | @gray(0)         | @gray(1)         | 0       | @blue(1) |
| 0       | 1       | 1       | 0       | @gray(0)         | @gray(1)         | 0       | @blue(1) |
| 0       | 1       | 1       | 1       | @gray(0)         | @gray(1)         | 0       | @blue(1) |
| 1       | 0       | 0       | 0       | @gray(1)         | @gray(0)         | @red(1) | 0        |
| 1       | 0       | 0       | 1       | @gray(1)         | @gray(0)         | @red(1) | 0        |
| 1       | 0       | 1       | 0       | @gray(1)         | @gray(0)         | @red(1) | 0        |
| 1       | 0       | 1       | 1       | @gray(1)         | @gray(0)         | @red(1) | 0        |
| 1       | 1       | 0       | 0       | @gray(D)         | @gray(D)         | D       | D        |
| 1       | 1       | 0       | 1       | @gray(D)         | @gray(D)         | D       | D        |
| 1       | 1       | 1       | 0       | @gray(D)         | @gray(D)         | D       | D        |
| 1       | 1       | 1       | 1       | @gray(D)         | @gray(D)         | D       | D        |

> **Aufgabe:** Lesen Sie die minimale Funktion für `DF` und `DG` ab!

$$
\begin{aligned}
DF &= F +\overline{G}\,\overline{A}\,B\\
DG &= G +\overline{F}\,A\,\overline{B}
\end{aligned}
$$

![Bild](../images/09_megaAVR_0/PAL_example_Solution.png)

Ein weiteres Einführungsbeispiel finden Sie unter [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/SebastianZug/StateMachines/master/README.md#1)

#### Realisierung

Eine Umsetzungsmöglichkeit für Schaltnetze sind die sogenannten PAL (Programmable Array Logic) die bereits in der Vorlesung 4 eingeführt wurden [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/TUBAF-IfI-LiaScript/VL_EingebetteteSysteme/master/04_Schaltnetze.md#13).

An dieser Stelle wurden die 2 stufigen Schaltfunktionen mit einem programmierbaren `AND` Array vorgestellt.

![Bild](../images/09_megaAVR_0/PAL.png "PAL Schema")
![Bild](../images/09_megaAVR_0/PAL16L8.png "PAL16L8  [^AMD]")


Diese erweitern wir nun um die Speicherglieder und deren Rückkopplung. Beachten Sie die Ergänzung auf der Ausgangsseite und die zusätzliche Clock-Leitung.

![Bild](../images/09_megaAVR_0/PAL16R8.png "PAL16L8  [^AMD]")


[^AMD]: Datenblatt PAL16R8 Family, Advanced Micro Devices, [link](http://www.applelogic.org/files/PAL16R8.pdf), 1996

#### Integration im 4809

![alt-text](../images/09_megaAVR_0/CCL_Logic.png "Konfigurierbare Logikbausteile des 4809 [^Microchip4809] Seite 116" )

Die CCL-Peripherie bietet eine Reihe von Look-up Tables (LUTs). Jede LUT besteht aus drei Eingängen, einer Wahrheitstabelle, einem Synchronisator/Filter und einem Flankendetektor. Jede LUT kann einen Ausgang als anwenderprogrammierbaren logischen Ausdruck erzeugen mit drei Eingängen. Der Ausgang wird aus den Eingängen mit Hilfe der kombinatorischen Logik generiert und kann gefiltert werden, um Spikes zu entfernen. Der CCL kann so konfiguriert werden, dass er bei Änderungen der LUT-Ausgänge eine Interrupt-Anforderung erzeugt.
Benachbarte LUTs können kombiniert werden, um bestimmte Operationen durchzuführen.

!?[alt-text](https://www.youtube.com/watch?v=beZXfAUR-PE)

### Event System

Das Eventsystem erlaubt eine direkte Peripherie-zu-Peripherie-Signalisierung, Peripheriegeräte können direkt Peripherieereignisse erzeugen, verwenden und darauf reagieren. Dabei werden entsprechen kurze Reaktionszeit garantiert. Auf dem 4809 sind 8 parallele Ereigniskanäle verfügbar, wobei jeder Kanal wird von einem Ereignisgenerator gesteuert wird und mehrere Ereignisbenutzer haben kann. Ereignisse sind dabei die Zustände der meisten Peripheriegeräten oder manuelle, aus der Software gesendet Signale. Das Ereignissystem funktioniert in den Modi "Aktiv", "Leerlauf" und "Ruhezustand.

![alt-text](../images/09_megaAVR_0/EventSystem_Idea.png "Event-Channel Konzept am Beispiel einer Timer / ADC Kombination [^Microchip4809] Seite 124")

Abfolge der Konfiguration:

1. ... Konfiguration eines Peripheriegerät als Quelle: Wenn es sich bei der erzeugenden Peripherie z. B. um einen Timer handelt, stellen Sie die Vorskalierung, das Vergleichsregister usw. so ein, dass das gewünschte Ereignis erzeugt wird.
2. ... Konfiguration eines Peripheriegerät als ereignisverarbeitende(n) Senke(m): Wenn z. B. der ADC der Ereignisbenutzer ist, stellen Sie den ADC Prescaler, Auflösung, Wandlungszeit usw. wie gewünscht ein und konfigurieren Sie die ADC-Wandlung so, dass sie beim Empfang eines Ereignisses startet eines Ereignisses startet.
3. ... Konfigurieren Sie des Ereignissystems: Im genannten Fall leitet der Timer/Compare seine Events  z. B. über Kanal 0, was durch Schreiben in `EVSYS.CHANNEL0` erreicht wird. Konfigurieren Sie den ADC so, dass er auf diesen Kanal hört, indem Sie in `EVSYS.USERn` schreiben, wobei n der Index ist, der dem ADC.

![alt-text](../images/09_megaAVR_0/EventChannelSystem.png "Event-Channel Konzept [^Microchip4809] Seite 123")

| Konfigurationsregister | Bedeutung                                        |
| ---------------------- | ------------------------------------------------ |
| `CHANNELn`             | Definition des singulären Triggers auf dem Kanal |
| `STROBEx`              | Vektor der Zieldevices                           |

Welche Inhalte können mit welchen Ausgaben verknüpft werden finden Sie im Handbuch ab Seite 125.

!?[alt-text](https://www.youtube.com/watch?v=WosagCSKdng&t=106s)

Events könnnen auch in Software ausgelöst werden.

## Software

siehe Beispielfälle im Coderepository!

## Aufgaben

- [ ] Installieren Sie sich das AVR Studio für die Arbeit mit dem 4809 Controller. Testen Sie den Aufbau anhand von einfachen Programmen.

- [ ] Implementieren Sie die Beispielumsetzung des Eventsystems mit Ihrem Controller als Einstiegsaufgabe nach. Den Taster dazu finden Sie im Bastelset.
