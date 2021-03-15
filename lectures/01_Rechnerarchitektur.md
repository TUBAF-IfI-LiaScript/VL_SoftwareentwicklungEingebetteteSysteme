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

# Grundbegriffe der Rechnerarchitektur

Die interaktive Version des Kurses ist unter diesem [Link](https://liascript.github.io/course/?https://raw.githubusercontent.com/SebastianZug/VL_ProzeduraleProgrammierung/master/01_Rechnerarchitektur.md#1) zu finden.

**Lernziele**

* Wiederholung der Grundbegriffe der Rechnerarchitektur
* Diskussion von generellen Unterscheidungsmerkmalen bei der Auswahl eines Prozessors
* Ablauf des Compiliervorganges

**Fragen an die heutige Veranstaltung ...**

* Erläutern Sie den Unterschied zwischen dem RISC und CISC Ansatz.
* Erklären Sie das Grundkonzept des Pipelining.
* Beschreiben Sie die Abläufe bei der Abarbeitung von Befehlen.
*

## Was ist eine Rechnerarchitektur?

> Rechnerarchitektur ist ein Teilgebiet der Technischen Informatik, das sich mit dem Design von Rechnern (Computern) und speziell mit deren Organisation sowie deren externem und internem Aufbau (was ebenfalls mit 'Rechnerarchitektur' bezeichnet wird) beschäftigt.

> Die **Architektur** eines Rechners beschreibt dessen grundsätzlichen Aufbau (Hardwarestruktur) und das Zusammenspiel der Komponenten (Organisationsstruktur).

Beispiel: Der von Neumann-Rechner arbeitet sequentiell. Befehl für Befehl wird abgeholt, interpretiert, ausgeführt und das Resultat abgespeichert. Seine Komponenten definieren sich zu

+ Steuerwerk (Taktgeber und Befehlszähler)
+ Speicherwerk
+ Rechenwerk (CPU)
+ E/A-Einheit.

Die Datenbreite, Adressierungsbreite, Registeranzahl und Befehlssatz können als "Gestaltungsparameter" verstanden werden.

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

Im Unter

| Aspekt          | Intel 4004 | Microchip Atmega | Texas Instruments MSP 430 | STM 32  |
| --------------- | ---------- | ---------------- | ------------------------- | ------- |
| Architektur     |            | Harvard          | von-Neumann               | Harvard |
| Busbreite       |            | 8/32             | 16                        | 32      |
| Registeranzahl  |            |                  |                           |         |
| Befehlssatz     |            |                  |                           |         |
| Geschwindigkeit |            |                  |                           |         |

## Befehlsabarbeitung und Instruktions-Sets

```ascii

dfgd

```


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


## Mikrocontroller vs. Mikroprozessor

Typischerweise fehlen beim  Mikroprozessor im Chip viele Dinge, die einen Rechner ausmachen, wie zum Beispiel der Arbeitsspeicher (RAM), Programmspeicher usw.. Entsprechend sind dann der Daten- und Adress-Bus zur Ansteuerung von externem Speicher sowie Leitungen für Interrupts aus dem Chip herausgeführt. Beim Mikrocontroller sind die Komponenten wie der Programmspeicher im Chip
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

## Vom C Programm zum ausführbaren Code
