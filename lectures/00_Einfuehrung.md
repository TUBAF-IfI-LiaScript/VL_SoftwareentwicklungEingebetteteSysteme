<!--
author:   Sebastian Zug
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.1
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md#10
-->

# Einführung und Motivation

Die interaktive Version des Kurses ist unter diesem [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/SebastianZug/VL_ProzeduraleProgrammierung/master/00_Einfuehrung.md#1) zu finden.


<div>
  <span id="simulation-time"></span>
</div>
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
@AVR8js.sketch

## Zielstellung

                                      {{0-1}}
********************************************************************************

Was steht im Modulhandbuch über diesen Kurs?

**Qualifikationsziele /Kompetenzen:**

_Mit der erfolgreichen Teilnahme an der Veranstaltung sollen die Studierenden in der Lage sein:_

+ die Komponenten realer eingebetteter Controller-Architekturen(8Bit -32Bit) zu beschreiben und analysieren zu können
+ Controller im Hinblick auf bestimmte Anforderungsprofile zu beurteilen
+ Elemente eingebetteter Anwendungen (insbesondere Sensoren)in ihrer Funktion und Eignung auszuwählen und in Software und Hardware in eine Anwendung zu integrieren Methoden des Softwareentwurfes und verschiedenen Tool-Chains für die Implementierung eingebetteter Systeme anwenden zu können
+ Codefragmente im Hinblick auf die Qualität und mögliche Fehler zu analysieren

**Inhalte**

_**Gegenüberstellung verschieden Architekturen etablierter Controller**, Integration von Controllern in eingebetteten Anwendungen, Erweiterung als Sensor-Aktor-Systeme, Parameter von Sensorssystemen (IMU, Distanzmesssensorik, GNSS), Datenaufbereitung und Fusion, **Betriebssystemkonzepte für eingebettete Controller**, Methoden der Fehlertoleranz_

********************************************************************************

                                      {{1-2}}
********************************************************************************
**Und was heißt das nun konkret? Worum geht es?**

| Arduino Uno Board                                                                                                 | Nucleo 64                                                                                          |
| ----------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| ![ArduinoUno](../images/00_Einfuehrung/arduino-uno-rev3.jpg)<!-- style="width: 100%; auto; max-width: 415px;" --> | ![ArduinoUno](../images/00_Einfuehrung/Nucleo64.jpg)<!-- style="width: 100%; max-width=315px;" --> |
| Microchip ATmega 328p                                                                                             | STM32F401                                                                                          |
| 8-Bit AVR Familie                                                                                                 | 32-Bit Cortex M4 Prozessor                                                                         |
| C, Assembler                                                                                                      | C, C++                                                                                             |
| avrlibc, FreeRTOS                                                                                                 | CMSIS, mbedOS, FreeRTOS                                                                            |
| 10 Bit Analog-Digital-Wandler, 16 Bit Timer,                                                                                                                   |                                                                                                    |


********************************************************************************

                                      {{2-3}}
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
| Karl Fessel             | karl.fessel@informatik.tu-freiberg.de   |

> Bitte melden Sie sich im OPAL unter [Eingebettete Systeme](https://bildungsportal.sachsen.de/opal/auth/RepositoryEntry/26860322818/CourseNode/102563572218999) für die Veranstaltung an. Dies ist im Kontext der Pandemiesituation Teil des Hygienekonzepts der Hochschule.

### Zeitplan

Die Veranstaltung wird sowohl für die Vorlesung als auch die Übung in Präsenz durchgeführt.

| Veranstaltungen | Tag | Zeitslot | Ort | Bemerkung |
| --------------- | --- | -------- | --- | --------- |
| Vorlesung       | ??  | ??       | ??  |           |
| Übungen         | ??  | ??       | ??  |           |

Gegenwärtig wird davon ausgegangen, dass die Veranstaltung vollständig als Online-Kurs umgesetzt wird.

### Prüfungsmodalitäten

> *Credit-Points:* 6


> *Arbeitsaufwand:* Der Zeitaufwand beträgt 180h und setzt sich zusammen aus 60h Präsenzzeit und 120h Selbststudium. Letzteres umfasst die Vor- undNachbereitung der Lehrveranstaltung, die eigenständige Lösung von Übungsaufgaben sowie die Prüfungsvorbereitung.

> *Prüfungsform:* Die Veranstaltung wird mit einer mündlichen Prüfung abgeschlossen.

## Literaturempfehlungen

1. Umfassende Lehrbücher



2. Videos


Bei den jeweiligen Vorlesungsinhalten werden zusätzliche Materialien angegeben.

## Schreiben Sie an den Materialien mit!

                              {{0-1}}
****************************************************************************

Die Lehrmaterialien finden Sie unter GitHub, einer Webseite für das Versionsmanagement und die Projektverwaltung.

[https://github.com/TUBAF-IfI-LiaScript/VL_EingebetteteSysteme/blob/dev/01_HistorischerUeberblick.md](https://github.com/TUBAF-IfI-LiaScript/VL_EingebetteteSysteme/blob/dev/01_HistorischerUeberblick.md)

Die Unterlagen selbst sind in der Auszeichnungsprache LiaScipt verfasst und öffentlich verfügbar.

****************************************************************************

                         {{1-2}}
****************************************************************************

Markdown ist eine Auszeichnungssprache für die Gliederung und Formatierung von Texten und anderen Daten. Analog zu HTML oder LaTex werden die Eigenschaften und Organisation von Textelementen (Zeichen, Wörtern, Absätzen) beschrieben. Dazu werden entsprechende "Schlüsselworte", die sogenannten Tags verwandt.

Markdown wurde von John Gruber und Aaron Swartz mit dem Ziel entworfen, die Komplexität der Darstellung so weit zu reduzieren, dass schon der Code sehr einfach lesbar ist. Als Auszeichnungselemente werden entsprechend möglichst kompakte Darstellungen genutzt.

```markdown HelloWorld.md
# Überschrift

__eine__ Betonung __in kursiver Umgebung__

* Punkt 1
* Punkt 2

Und noch eine Zeile mit einer mathematischen Notation $a=cos(b)$!
```

----------------------------------------------------------------------------<h1>Überschrift</h1>
<i>eine <em>Betonung</em> in kursiver Umgebung</i>
<ul>
<li>Punkt 1</li>
<li>Punkt 2</li>
</ul>
Und noch eine Zeile mit einer mathematischen Notation $a=cos(b)$!

----------------------------------------------------------------------------

Eine gute Einführung zu Markdown finden Sie zum Beispiel unter

* [MarkdownGuide](https://www.markdownguide.org/getting-started/)
* [GitHubMarkdownIntro](https://guides.github.com/features/mastering-markdown/)

Mit einem entsprechenden Editor und einigen Paketen macht das Ganze dann auch Spaß

* Wichtigstes Element ist ein Previewer, der es Ihnen erlaubt "online" die Korrektheit der Eingaben zu prüfen
* Tools zur Unterstützung komplexerer Eingaben wie zum Beispiel der Tabellen (zum Beispiel für Atom mit [markdown-table-editor](https://atom.io/packages/markdown-table-editor))
* Visualisierungsmethoden, die schon bei der Eingabe unterstützen
* Rechtschreibprüfung (!)

****************************************************************************


                                 {{2-3}}
****************************************************************************

Allerdings vermisst Markdown trotz seiner Einfachheit einige Features, die
für die Anwendbarkeit in der (Informatik-)Lehre sprechen:

* Ausführbarer Code
* Möglichkeiten zur Visualisierung
* Quizze Tests und Aufgaben
* spezifische Tools für die Modellierung Simulation etc.

```cpp     ArduinoSimulator.ino
void setup() {
  Serial.println("Hello stuff.");
}

void thing(char i) {
  switch(i) {
  case 0: Serial.println("a pear"); break;
  case 1: Serial.println("an apple"); break;
  case 2: Serial.println("an elephant"); break;
  case 3: Serial.println("an arduino"); break;
  }
}

void loop() {
  Serial.print("here's ");
  thing(random(4));
}
```
@NetSwarm.loop

```js
// Init components
AND(["and1", "and2"], ["and3"], "AND1");
OR(["or1", "or2"], ["or3"], "OR1");
XOR(["xor1", "xor2"], ["xor3"], "XOR1");
Button("btn1", "BUTTON1");
Button("btn2", "BUTTON2");
Button("btn3", "BUTTON3");
Button("btn4", "BUTTON4");
Lamp("lmp1", "LAMP1");

// IO IN
wire("btn1", "and1");
wire("btn2", "and2");
wire("btn3", "or1");
wire("btn4", "or2");

// AND, OR -> XOR
wire("and3", "xor1");
wire("or3", "xor2");

// IO OUT
wire("xor3", "lmp1", "Main Output");
```

``` js @DigiSim.eval
// Init components
AND(["and1", "and2"], ["and3"], "AND1");
OR(["or1", "or2"], ["or3"], "OR1");
XOR(["xor1", "xor2"], ["xor3"], "XOR1");
Button("btn1", "BUTTON1");
Button("btn2", "BUTTON2");
Button("btn3", "BUTTON3");
Button("btn4", "BUTTON4");
Lamp("lmp1", "LAMP1");

// IO IN
wire("btn1", "and1");
wire("btn2", "and2");
wire("btn3", "or1");
wire("btn4", "or2");

// AND, OR -> XOR
wire("and3", "xor1");
wire("or3", "xor2");

// IO OUT
wire("xor3", "lmp1", "Main Output");
```

Eine Reihe von Einführungsvideos findet sich unter [Youtube](https://www.youtube.com/channel/UCyiTe2GkW_u05HSdvUblGYg). Die Dokumentation von LiaScript ist hier [verlinkt](https://liascript.github.io/course/?https://raw.githubusercontent.com/liaScript/docs/master/README.md#1)

***************************************************************************

## Trotz Simulation und Virtuellem ...

... braucht es aber auch immer etwas zum anfassen.

> Wir stellen Ihnen die entsprechenden Controller beider Plattformen zur Verfügung.

## Wie können Sie zum Gelingen der Veranstaltung beitragen?

* Stellen Sie Fragen, seinen Sie kommunikativ!

> Hinweis auf OPAL Forum!

* Organisieren Sie sich in Arbeitsgruppen!

> Bearbeiten Sie Aufgaben gemeinsam online.

* Bringen Sie sich mit Implementierungen in die Veranstaltung ein.

## Und wenn Sie dann immer noch programmieren wollen ...

Dann wartet das TUFbots-Team auf Sie ...

![WALL-E](./images/00_Einfuehrung/RoboCup.jpeg)<!--
style="width: 80%; display: block; margin-left: auto; margin-right: auto;" -->

Quelle: _Team RobOTTO_, OVGU Magdeburg

## Und jetzt sind Sie an der Reihe

> Mit welchen Systemen haben Sie bereits gearbeitet und welche Projekte haben Sie damit verwirklicht?

## Hausaufgabe

+ Legen Sie sich, sofern das noch nicht geschehen ist, einen GitHub Account an ... und seien Sie der erste, der einen Fehler, eine unglückliche Formulierung usw. in den Unterlagen findet und diesen als _Contributer_ korrigiert :-)
