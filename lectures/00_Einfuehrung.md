<!--

author:   Sebastian Zug, Karl Fessel & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de
version:  0.0.3
language: de
narrator: Deutsch Female
comment:  Fokus der Lehrveranstaltung und Organisation, Verknüpfungspunkte mit weiteren Inhalten 
icon:     https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md

-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/lectures/00_Einfuehrung.md#1)

# Einführung und Motivation

| Parameter                | Kursinformationen                                                                                                                                                                                |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Vorlesung Digitale Systeme / Softwareentwicklung für eingebettete Systeme`                                                                                                                      |
| **Semester**             | `Sommersemester 2025`                                                                                                                                                                            |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                                |
| **Inhalte:**             | `Motiviation und Organisation der Veranstaltung `                                                                                                                                                |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/00_Einfuehrung.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/00_Einfuehrung.md) |
| **Autoren**              | @author                                                                                                                                                                                          |

![](https://media.giphy.com/media/26gR2qGRnxxXAvhBu/giphy.gif)

------------------------------------------------

```cpp       avrlibc.cpp
// preprocessor definition
#define F_CPU 16000000UL
#define ANSWER_TO_LIFE 42

#include <avr/io.h>
#include <util/delay.h>

int main (void) {
  Serial.begin(9600);

  volatile byte a;

  asm ("ldi %0, %1\n\t"
      : "=r" (a)
      : "M" (ANSWER_TO_LIFE));

  Serial.print("Antwort auf die Frage, warum ich an dieser Vorlesung teilnehme: ");
  Serial.println(a);

  while(1) {
       _delay_ms(1000);
  }
  return 0;
}
```

## Zielstellung

                                      {{0-1}}
********************************************************************************

**Qualifikationsziele /Kompetenzen:**

+ die Komponenten realer eingebetteter Controller-Architekturen(8Bit -32Bit) zu beschreiben und analysieren zu können
+ Controller im Hinblick auf bestimmte Anforderungsprofile zu beurteilen
+ Elemente eingebetteter Anwendungen (insbesondere Sensoren)in ihrer Funktion und Eignung auszuwählen und in Software und Hardware in eine Anwendung zu integrieren Methoden des Softwareentwurfes und verschiedenen Tool-Chains für die Implementierung eingebetteter Systeme anwenden zu können
+ Codefragmente im Hinblick auf die Qualität und mögliche Fehler zu analysieren

**Inhalte**

+ Auffrischung und Erweiterung der Architekturbegriffe von Mikrocontrollern
+ Diskussion von Echtzeitbetriebssystemen
+ Methodische Softwareentwicklung 


********************************************************************************

                                      {{1-2}}
********************************************************************************

**Und was heißt das nun konkret? Worum geht es?**

**Hardware-Plattformen**

<!--data-type="none"-->
| Arduino Uno Board                                                                                                 | Nucleo 64                                                                                          |
| ----------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| ![ArduinoUno](../images/00_Einfuehrung/arduino-uno-rev3.jpg)<!-- style="width: 100%; auto; max-width: 415px;" --> | ![ArduinoUno](../images/00_Einfuehrung/Nucleo64.jpg)<!-- style="width: 100%; max-width=315px;" --> |
| Microchip ATmega 328p                                                                                             | STM32F401                                                                                          |
| 8-Bit AVR Familie                                                                                                 | 32-Bit Cortex M4 Prozessor                                                                         |
| C, Assembler                                                                                                      | C, C++                                                                                             |
| avrlibc, FreeRTOS                                                                                                 | CMSIS, mbedOS, FreeRTOS                                                                            |
| 10 Bit Analog-Digital-Wandler, 16 Bit Timer,                                                                      | 10 Timer, 16- and 32-bit (84 MHz), 12-bit ADC                                                     |


********************************************************************************

                                      {{2-3}}
********************************************************************************

**Inhalte der Vorlesung**

| VL  | Tag    | Inhalt der Vorlesung                           | Bemerkung                          |
| --- | ------ | :--------------------------------------------- | ---------------------------------- |
| 0   | 01.04. | Einführung und Motivation / Grundbegriffe      | Wiederholung                       |
| 1   | 08.04. | ATmega Familien                                |                                    |
| 2   | 15.04. | Analog Digitalwandler / Interrupts             |                                    |
| 3   | 22.04. | Performancebeschränkungen von 8Bit Controllern |                                    |
| 4   | 29.04. | Kommunikationsprotokolle                       | Grundlagen abgeschlossen           |
| 5   | 06.05. | XMEGA Architektur (AtMega4809)                 |                                    |
| 6   | 13.05. | CortexM Architektur                            |                                    |
| 7   | 20.05. | _Konferenzreise_                               | Architekturüberblick abgeschlossen |
| 8   | 27.05. | CortexM Peripherie                             |                                    |
| 9   | 03.06. | RTOS Konzepte                                  |                                    |
| 10  | 10.06. | RTOS Implementierungen                         | Echtzeitdiskussion abgeschlossen   |
| 11  | 17.06. | Anforderungsanalyse                            |                                    |
| 12  | 24.06. | Systementwurf                                  |                                    |
| 13  | 01.07. | Testen / Wartungsphase                         |                                    |
| 14  | 08.07. | Vorstellung der Praktikumsergebnisse           |                                    |


********************************************************************************

                                      {{3-4}}
********************************************************************************

Dabei werden unterschiedliche Ebenen der Abstraktion der Abläufe im Rechner adressiert.

<!--
style="width: 80%; min-width: 420px; max-width: 720px;"
-->
```ascii

                Abstraktionsebenen

           +----------------------------+ -.
  Ebene 6  | Problemorientierte Sprache |  |
           +----------------------------+  |
                                           ⎬ Anwendungssoftware
           +----------------------------+  |
  Ebene 5  | Assemblersprache           |  |
           +----------------------------+ -.

           +----------------------------+
  Ebene 4  | Betriebssystem             |     Systemsoftware
           +----------------------------+

           +----------------------------+
  Ebene 3  | Istruktionsset             |     Maschinensprache
           +----------------------------+

           +----------------------------+ -.
  Ebene 2  | Mikroarchitektur           |  |
           +----------------------------+  |
                                           ⎬ Automaten, Speicher, Logik
           +----------------------------+  |  
  Ebene 1  | Digitale Logik             |  |  
           +----------------------------+ -.  

           +----------------------------+
  Ebene 0  | E-Technik, Physik          |     Analoge Phänomene
           +----------------------------+                                      .
```

********************************************************************************

## Organisation

| Name                    | Email                                   |
|:----------------------- |:--------------------------------------- |
| Prof. Dr. Sebastian Zug | sebastian.zug@informatik.tu-freiberg.de |

> Bitte melden Sie sich im OPAL unter [Digitale Systeme/Softewareentwicklung für eingebettete Systeme](https://bildungsportal.sachsen.de/opal/auth/RepositoryEntry/19949355008/CourseNode/99400814778000) für die Veranstaltung an. 

### Zeitplan

Die Veranstaltung wird sowohl für die Vorlesung als auch die Übung in Präsenz durchgeführt.

| Veranstaltungen | Tag      | Zeitslot    | Bemerkung      |
| --------------- | -------- | ----------- | -------------- |
| Vorlesung       | Dienstag | 09:45-11:15 |                |
| Praktikum       | Dienstag | 18:00-19:30 | ungerade Woche |
| Übungen         | Freitag  | 08:00-09:30 | gerade Woche   |


### Prüfungsmodalitäten

> *Credit-Points:* 6

> *Arbeitsaufwand:* Der Zeitaufwand beträgt 180h und setzt sich zusammen aus 60h Präsenzzeit und 120h Selbststudium. Letzteres umfasst die Vor- undNachbereitung der Lehrveranstaltung, die eigenständige Lösung von Übungsaufgaben sowie die Prüfungsvorbereitung.

> *Prüfungsform:* Die Veranstaltung wird mit einer mündlichen Prüfung abgeschlossen. Dabei starten wir zunächst mit einem von Ihnen gewählten Codebeispiel und gehen dann auf die Inhalte der Vorlesungen und Übungen ein.

## Schreiben Sie an den Materialien mit!

                              {{0-1}}
****************************************************************************

Die Lehrmaterialien finden Sie unter GitHub, einer Webseite für das Versionsmanagement und die Projektverwaltung.

[https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme)

Die Unterlagen selbst sind in der Auszeichnungsprache LiaScript verfasst und öffentlich verfügbar.

****************************************************************************

                         {{1-2}}
****************************************************************************

Allerdings vermisst Markdown trotz seiner Einfachheit einige Features, die
für die Anwendbarkeit in der (Informatik-)Lehre sprechen:

* Ausführbarer Code
* Möglichkeiten zur Visualisierung
* Quizze Tests und Aufgaben
* spezifische Tools für die Modellierung Simulation etc.

<div>
  <wokwi-led color="red" pin="13" port="B" label="13"></wokwi-led>
  <span id="simulation-time"></span>
</div>
```cpp       avrlibc.cpp
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

//16.000.000 Hz / 1024 = 15.625

int main(void)
{
  Serial.begin(9600);
  DDRB |= (1 << PB5);  // Ausgabe LED festlegen
  // Timer 1 Konfiguration
  //         keine Pins verbunden
  TCCR1A  = 0;
  TCCR1B  = 0;
  // Timerwert
  TCNT1   = 0;
  TCCR1B |= (1 << CS12) | (1 <<CS10);  

  while (1) //infinite loop
  {
     if (TCNT1>15625){
        TCNT1 = 0;  
        PINB = (1 << PB5); // LED ein und aus
     }
  }
}
```
@AVR8js.sketch

> **Frage:** Welche Komponente des Controllers wird hier genutzt? An welcher Stelle hätten Sie alternative Ideen?

Eine Reihe von Einführungsvideos findet sich unter [Youtube](https://www.youtube.com/channel/UCyiTe2GkW_u05HSdvUblGYg). Die Dokumentation von LiaScript ist hier [verlinkt](https://liascript.github.io/course/?https://raw.githubusercontent.com/liaScript/docs/master/README.md#1)

***************************************************************************

## Trotz Simulation und Virtuellem ...

... braucht es aber auch immer etwas zum anfassen.

> Wir stellen Ihnen die entsprechenden Controller der genannten Plattformen zur Verfügung.

![Bastelbox](../images/00_Einfuehrung/Paket.jpeg)

## Wie können Sie zum Gelingen der Veranstaltung beitragen?

* Stellen Sie Fragen, seinen Sie kommunikativ!

> Hinweis auf OPAL Forum!

* Organisieren Sie sich in Arbeitsgruppen!

> Bearbeiten Sie Aufgaben gemeinsam online.

* Bringen Sie sich mit Implementierungen in die Veranstaltung ein.

## Und jetzt sind Sie an der Reihe

> Mit welchen Systemen haben Sie bereits gearbeitet und welche Projekte haben Sie damit verwirklicht?

## Hausaufgabe

+ Legen Sie sich, sofern das noch nicht geschehen ist, einen GitHub Account an ... und seien Sie der erste, der einen Fehler, eine unglückliche Formulierung usw. in den Unterlagen findet und diesen als _Contributer_ korrigiert :-)
+ Überlegen Sie sich, zu welches Thema Sie im Praktikum vorstellen wollen:

  + microROS / micro DDS
  + DSP Implementierung im STM32F4    
  + mbedOS vs FreeRTOS
  + CAN Bus - Theorie und Anwendung
  + Schlafmodi von Controllern (elektrische Vermessung)
  + virtuelle Maschinen auf eingebetteten Systemen (MicroPython, WASM-Interpreter)
  + Echtzeitfähigkeit des Raspberry Pi 
  + ... eigene Ideen

  Die Präsentationen sollten 30 Minuten einnehmen und mit einer praktischen Demonstration kombiniert werden.

+ Setzen Sie sich mit platformio.org auseinader und testen Sie die Features zum Debugging.
