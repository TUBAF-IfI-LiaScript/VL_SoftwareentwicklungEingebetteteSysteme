<!--
author:   Sebastian Zug, Karl Fessel & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.1
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->


[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md#1)


# Embedded Linux vs. Bare Metal / RTOS

| Parameter                | Kursinformationen                                                                                                                                                                                                                              |
| ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Veranstaltung:**       | `Vorlesung Softwareentwicklung für eingebettete Systeme`                                                                                                                                                                                       |
| **Semester**             | `Sommersemester 2026`                                                                                                                                                                                                                          |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                                                                              |
| **Inhalte:**             | `Embedded Linux Grundlagen, Build-Systeme, Echtzeitfähigkeit, Abgrenzung zu RTOS`                                                                                                                                                             |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/13_EmbeddedLinux.md](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/13_EmbeddedLinux.md) |
| **Autoren**              | @author                                                                                                                                                                                                                                        |

---

## Motivation

Wann ist ein RTOS die richtige Wahl, wann Embedded Linux? Dieses Kapitel liefert eine Entscheidungsmatrix und diskutiert die jeweiligen Stärken und Grenzen.

<!-- TODO: Inhalt ergänzen -->

## Embedded-Linux-Grundlagen

Der Boot-Prozess eingebetteter Linux-Systeme im Detail: Bootloader, Kernel, Rootfs und Init-System. Dazu Device Trees und die Unterscheidung zwischen Kernel-Modulen und Userspace-Treibern.

<!-- TODO: Inhalt ergänzen -->

## Build-Systeme: Yocto & Buildroot

Reproduzierbare Builds für Embedded Linux: Buildroot mit menuconfig, Yocto mit Layer-Konzept. Vergleich beider Ansätze hinsichtlich Flexibilität und Komplexität.

<!-- TODO: Inhalt ergänzen -->

## Echtzeit unter Linux

Echtzeitfähigkeit mit dem PREEMPT_RT Patch: Latenzvergleich zwischen Standard-Kernel, RT-Kernel und FreeRTOS. Messung und Bewertung mit cyclictest.

<!-- TODO: Inhalt ergänzen -->

## Architektonische Einordnung

Systemarchitekturen im Vergleich: Linux + ROS 2 vs. microROS auf MCU, Asymmetric Multiprocessing mit Linux auf Core A und RTOS auf Core M. Der STM32MP1 dient als konkretes Beispiel.

<!-- TODO: Inhalt ergänzen -->

## Zusammenfassung & Aufgaben

<!-- TODO: Inhalt ergänzen -->
