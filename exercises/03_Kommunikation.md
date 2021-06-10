<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.2
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md
         https://github.com/liascript/CodeRunner
         https://github.com/LiaTemplates/Pyodide

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/exercises/01_AnalogDigitalWandler.md#1)

# Timer

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2021`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Praktische Diskussion der Timer Funktionalität`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/01_AnalogDigitalWandler.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/01_AnalogDigitalWandler.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Diskussion des vergangenen Aufgabenblattes

> Hier sind Sie gefragt ...

## I2C Kommunikation - Beschleunigungssensoren

__Funktionsprinzip__

| Prinzip                                  | Sensor                                                                                                                                                                                                                                                                                                                |
| ---------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Dehnungsmesstreifen                      | ... Bestimmung der Kraft auf die Testmasse, indem die Verformung der Befestigung (z. B. eines Stabes) mittels Dehnungsmessstreifen bestimmt wird.                                                                                                                                                                     |
| Piezoelektrische Beschleunigungssensoren | ... ein piezokeramisches Sensorplättchen wandelt Druckschwankungen in elektrische Signale um. Die Druckschwankung wird durch eine an der Piezokeramik befestigte (seismische) Masse erzeugt und wirkt bei einer Beschleunigung des Gesamtsystems auf die Piezokeramik.                                                |
| Mikro-elektro-mechanische Systeme (MEMS) | Feder-Masse-Systeme, bei denen die „Federn“ nur wenige μm breite Silicium-Stege sind und auch die Masse aus Silicium hergestellt ist. Durch die Auslenkung bei Beschleunigung kann zwischen dem gefedert aufgehängten Teil und einer festen Bezugselektrode eine Änderung der elektrischen Kapazität gemessen werden. |

Konzentrieren wir uns auf das letztgenannte Konzept.

<iframe width="560" height="315" src="https://www.youtube.com/embed/eqZgxR6eRjo" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

![Bild](../images/exercises/MEMSStruktur.png " [^4]")

Aus der "Integration" der Samples über der Zeit folgt eine mangelnde Langzeitstabilität, daher koppelt man ein INS beispielsweise liefert eine Kombination mit einem Global Positioning System (GPS).

Welche Schnittstellen bieten diese Sensoren und wie können wir diese über den I2C Bus bedienen?

| Dokument              | Webseite                                                                                 |
| --------------------- | ---------------------------------------------------------------------------------------- |
| Handbuch 6050         | [Link](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)    |
| Registerstruktur 6050 | [Link](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf) |

Eine Beispielimplementierung findet sich im Ordner mit den Codebeispielen.

[^4]: *MEMS - Micro-Electro-Mechanical Systems* [Elektronik Kompendium](http://www.elektronik-kompendium.de/sites/bau/1503041.htm )]

## I2C Kommunikation - Datenaustausch zwischen Controllern

vgl. Codebeispiel [Link](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/tree/main/codeExamples/avr/I2C_TOarduino)


## Aufgaben

- [ ] Verteilen Sie die Aufgaben aus dem Beispiel der vergangen Woche auf die zwei Ihnen vorliegenden Controller. Nehmen Sie den Joystick am Arduino Uno Wifi in Betrieb und übertragen Sie die Informationen an den zweiten Controller, der den Schrittmotor und die Datenerfassung steuert. Damit entsteht ein Remote-Fernzugriff.

- [ ] Nehmen Sie die Echtzeituhr aus der Bastelbox in Betrieb und evaluieren Sie deren Registerset. Folgen Sie dazu den Vorgaben auf [MangoLabs](https://www.mangolabs.de/product/real-time-clock-module/). Was ist die grundlegende Idee der Echtzeituhr?
