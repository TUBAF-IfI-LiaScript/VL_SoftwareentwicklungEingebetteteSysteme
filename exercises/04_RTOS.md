<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.3
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/exercises/04_IDE_4809.md#1)

# Beispielaufgaben für die RTOS Programmierung

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2025`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Beispielaufgaben für die RTOS Programmierung`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/04_IDE_4809.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/04_IDE_4809.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Diskussion des vergangenen Aufgabenblattes

> Hier sind Sie gefragt ...

## FreeRTOS

> Worin unterscheidnen sich die FreeRTOS-Implementierungen auf AVR und ARM Cortex-M?

| Aspekt                       | AVR (z.B. ATmega328)                                              | ARM Cortex-M (z.B. Cortex-M3/M4)                                                         |
| ---------------------------- | ----------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| **Programmiermodell**        | 8-Bit-RISC, einfache Registerstruktur                             | 32-Bit-RISC, komplexere Register (z.B. 16 Core-Register)                                 |
| **Kontextwechsel**           | Manuell per Assembly, speichert ca. 32 Register                   | Hardware unterstützt Kontext-Switch (PendSV-Interrupt) + Registerspeicherung automatisch |
| **Stack-Initialisierung**    | Manuell per Assembly, weniger Register zu speichern               | Standardisiertes Stack-Frame mit PC, PSR, R0-R12, LR, usw.                               |
| **Timer für Tick-Interrupt** | Häufig Timer0 oder Timer1 mit Compare Match Interrupt             | SysTick Timer oder ein beliebiger Timer mit NVIC-Interrupt                               |
| **Interrupt-Verwaltung**     | Manuell, mit CLI/SFI-Befehlen (Interrupt Enable/Disable)          | NVIC-Controller mit Prioritätssteuerung                                                  |
| **Speicher-Layout**          | 8-Bit-Register, kleiner RAM, spezieller Speicherbereich für Stack | 32-Bit-Register, größerer RAM, standardisierte Stackpointer                              |
| **Assembly-Code Umfang**     | Kürzer und simpler, da weniger Register                           | Umfangreicher, da mehr Register gesichert werden müssen                                  |

> Wie funktioniert die Sicherung beim Kontextwechsel in FreeRTOS auf ARM Cortex-M? (PendSV = "Pending Supervisor Call")

```ascii
┌────────────────────────────┐
│       Task A läuft         │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│   Ereignis passiert (z.B.  │
│   ISR setzt Task B auf     │
│     "ready")               │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│  FreeRTOS setzt PendSV-    │
│  Interrupt aus ISR         │
│  (SCB->ICSR |= PENDSVSET)  │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│    PendSV wartet (hat      │
│    niedrigste Priorität)   │
└────────────┬───────────────┘
             │ CPU ist frei
             ▼
┌────────────────────────────┐
│      PendSV-Handler        │
│  ▸ speichert Kontext Task A│
│  ▸ wählt nächsten Task     │
│  ▸ lädt Kontext Task B     │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│        Task B läuft        │
└────────────────────────────┘                                                           .
```

> ... wie sieht danach der Stack aus?

```ascii
Stack (nach Hardware-Ablage bei PendSV-Entry)

┌───────────────┐ ← automatische HW-Sicherung
│ xPSR          │   Statusregister (Flags, Thumb-Bit etc.)
│ PC            │
│ LR            │   Link Register (Rücksprungadresse)
│ R12           │   temporäres Register, wird oft als Scratch-Register verwendet
│ R3            │   Übergabeparameter & Rückgabewerte
│ R2            │
│ R1            │
│ R0            │
├───────────────┤ ← dann Software sichert:
│ R4            │ ← manuell vom OS gesichert
│ ...           │
│ R11           │
└───────────────┘                                                                        
```

## Beispiele der Umsetzung

siehe Code Ordner ...

## Aufgaben

- [ ] Implementieren Sie eine einfache RTOS-Anwendung, die eine Idle Funktion umfasst. Recherchieren Sie dazu  die Funktionsweise des Idle Tasks in FreeRTOS. Der Idle Task soll eine LED blinken lassen, um zu zeigen, dass das System im Leerlauf ist.
- [ ] Realisieren Sie eine einfache Kommunikation mit einer Queue - Task 1 erzeugt alle 200 ms eine Zahl (z.B. einen Zähler) und sendet sie über eine FreeRTOS-Queue an Task 2, die die Zahl empfängt und ausgibt.
