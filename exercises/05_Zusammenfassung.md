<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.4
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/exercises/05_Zusammenfassung.md#1)

# Verwendung der IDE

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2021`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Zusammenfassung und Ausblick`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/05_Zusammenfassung.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/05_Zusammenfassung.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Organisation der Prüfungen

> Die Veranstaltung wird mit einer mündliche Prüfung abgeschlossen, die aus zwei Teilen besteht:

1. eigenes Implementierungsbeispiel (ATmega328, ATmega4809 mittels Assembler oder avrlibc, STM32F4 mittels HAL) ODER Edrys Beispiellabor aus dem Praktikumsteil
2. einer Diskussion zu den Themenfeldern der Vorlesung

Das konkrete Beispiel können Sie selbst wählen. Melden Sie sich bitte zur Abstimmung eines Termines.

## Diskussion des vergangenen Aufgabenblattes

> Hier sind Sie gefragt ...

## Hardwarevergleich

> Die ATmega Controller haben keine integrierten digital Analog-Wandler. Welches alternative Konzept wird genutzt und welche Nachteile birgt es?


## Debugging Interface

Hierbei springen wir von der Idee nochmals auf die debugging-Strategien zurück

Im folgenden Beispiel wird das [unity Testframework](https://github.com/ThrowTheSwitch/Unity#unity-test-api) genutzt

https://docs.platformio.org/en/latest/tutorials/ststm32/stm32cube_debugging_unit_testing.html
