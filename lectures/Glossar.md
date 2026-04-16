<!--

author:   Sebastian Zug & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de
version:  0.0.1
language: de
narrator: Deutsch Female
comment:  Glossar zentraler Begriffe der Vorlesung Softwareentwicklung für eingebettete Systeme
icon:     https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg

-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/Glossar.md#1)

# Glossar -- Softwareentwicklung für eingebettete Systeme

| Parameter                | Kursinformationen                                                                                                                                                                                                              |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Vorlesung Softwareentwicklung für eingebettete Systeme`                                                                                                                                                                       |
| **Semester**             | `Sommersemester 2026`                                                                                                                                                                                                          |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                                                              |
| **Inhalte:**             | `Glossar zentraler Begriffe`                                                                                                                                                                                                   |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/Glossar.md](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/Glossar.md) |
| **Autoren**              | @author                                                                                                                                                                                                                        |

Dieses Glossar fasst die zentralen Begriffe der Vorlesung zusammen und verweist auf die jeweiligen Vorlesungsmaterialien. Nutzen Sie es zur Prüfungsvorbereitung und als Nachschlagewerk.

> Die Begriffe sind nach den vier Blöcken der Vorlesung gegliedert. Innerhalb jedes Blocks sind sie alphabetisch sortiert.

-----

## Block I: Hardware-Progression

> Vorlesungen 00--05: Von grundlegenden Architekturbegriffen über ATmega und XMEGA bis zum Cortex-M4.

### ADC (Analog-Digital-Converter)

Wandelt analoge Eingangssignale (z.B. Sensorspannungen) in digitale Werte um. Beim ATmega328 steht ein 10-Bit-ADC mit mehreren Eingangskanälen und konfigurierbarer Referenzspannung zur Verfügung. Der XMEGA bietet einen erweiterten ADC mit fünf internen Referenzspannungen.

> Siehe [Vorlesung 03 -- Interrupts, Timer, ADC](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/03_InterruptsTimerADC.md), [Vorlesung 05 -- XMEGA](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/05_XMEGA.md)

### ALU (Arithmetic Logic Unit)

Die arithmetisch-logische Einheit führt Rechen- und Vergleichsoperationen innerhalb des Prozessors aus. Sie ist Teil des Rechenwerks im Von-Neumann-Modell und operiert auf den Daten in den Registern.

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md)

### Assembler

Maschinennahe Programmiersprache, die einen direkten Bezug zum Befehlssatz des Prozessors hat. Jeder Assembler-Befehl entspricht (in der Regel) einem Maschinenbefehl. Wird im Kurs verwendet, um die Arbeitsweise des ATmega auf Register-Ebene zu verstehen.

> Siehe [Vorlesung 00 -- Einführung](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/00_Einfuehrung.md)

### ATmega328

8-Bit-Mikrocontroller von Microchip (ehemals Atmel) mit 16 MHz Takt, 32 KB Flash, 2 KB SRAM und 1 KB EEPROM. Basis-Plattform des Kurses (Arduino Uno). Implementiert eine modifizierte Harvard-Architektur.

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### CAN (Controller Area Network)

Serielles Bussystem, das ursprünglich für die Automobilindustrie entwickelt wurde. Ermöglicht Multi-Master-Kommunikation mit Priorisierung durch Nachrichten-IDs und ist besonders robust gegenüber elektromagnetischen Störungen.

> Siehe [Vorlesung 04 -- Kommunikationsprotokolle](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/04_Kommunikationsprotokolle.md)

### CCL (Configurable Custom Logic)

Hardware-Logikblöcke im XMEGA, die einfache kombinatorische Logikfunktionen (UND, ODER, XOR etc.) direkt in der Peripherie ausführen -- ohne CPU-Beteiligung. In Kombination mit dem Event System können so Reaktionen in Hardware-Geschwindigkeit realisiert werden.

> Siehe [Vorlesung 05 -- XMEGA](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/05_XMEGA.md)

### CISC (Complex Instruction Set Computer)

Prozessorarchitektur mit einem umfangreichen Befehlssatz, bei der einzelne Befehle komplexe Operationen ausführen können. Im Gegensatz zu RISC benötigen CISC-Befehle oft mehrere Takte und variieren in ihrer Länge. Beispiel: x86.

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md)

### Cortex-M

Prozessorfamilie von ARM für Mikrocontroller-Anwendungen. Im Kurs wird der Cortex-M4 (STM32F401, 84 MHz, 96 KB SRAM) als Vertreter der 32-Bit-Klasse genutzt. Bietet gegenüber AVR: NVIC, DMA, FPU, MPU und SysTick-Timer.

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### DMA (Direct Memory Access)

Mechanismus, der Peripheriegeräten erlaubt, Daten direkt mit dem Arbeitsspeicher auszutauschen, ohne die CPU zu belasten. Besonders wichtig bei hohen Datenraten (z.B. ADC-Abtastung, Kommunikationsschnittstellen). Verfügbar ab Cortex-M.

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### EEPROM (Electrically Erasable Programmable Read-Only Memory)

Nichtflüchtiger Speicher, der byteweise gelesen und geschrieben werden kann. Wird zur Speicherung von Konfigurationsdaten oder Kalibrierungswerten verwendet, die auch nach einem Neustart erhalten bleiben sollen. Beim ATmega328: 1 KB.

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### Event System

Hardware-Mechanismus des XMEGA, der Peripheriemodule direkt miteinander verbinden kann -- ohne Software-Intervention oder CPU-Last. Ereignisse (z.B. Timer-Overflow, Pin-Änderung) werden über dedizierte Kanäle weitergeleitet.

> Siehe [Vorlesung 05 -- XMEGA](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/05_XMEGA.md)

### Flash-Speicher

Nichtflüchtiger Programmspeicher des Mikrocontrollers, in dem der ausführbare Code abgelegt wird. Beim ATmega328: 32 KB. Kann nur blockweise gelöscht und neu beschrieben werden (im Gegensatz zu EEPROM).

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### FPU (Floating Point Unit)

Koprozessor für Gleitkomma-Berechnungen. Auf 8-Bit-AVR-Controllern fehlt eine FPU, sodass Floating-Point-Operationen in Software emuliert werden müssen (langsam und speicherintensiv). Der Cortex-M4 verfügt über eine Hardware-FPU.

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### GPIO (General Purpose Input/Output)

Universell nutzbare digitale Ein-/Ausgangs-Pins eines Mikrocontrollers. Über Konfigurationsregister (DDRx, PORTx, PINx beim ATmega) werden Richtung und Zustand gesteuert. Grundlage jeder Hardware-Interaktion.

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### HAL (Hardware Abstraction Layer)

Software-Schicht, die den Zugriff auf Hardware-Register hinter einer einheitlichen API verbirgt. Ermöglicht Portabilität zwischen verschiedenen Mikrocontrollern und vereinfacht die Anwendungsentwicklung. Beispiele: Arduino-Framework, STM32 HAL, ASF4.

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md), [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### Harvard-Architektur

Rechnerarchitektur mit getrennten Speichern und Bussen für Programm und Daten. Ermöglicht gleichzeitigen Zugriff auf Befehle und Daten. Die AVR-Controller verwenden eine modifizierte Harvard-Architektur (getrennter Flash- und SRAM-Speicher, aber einheitlicher Adressraum).

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md), [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### I2C / TWI (Inter-Integrated Circuit / Two Wire Interface)

Synchrones serielles Kommunikationsprotokoll mit zwei Leitungen (SDA, SCL). Verwendet ein Master-Slave-Prinzip mit Adressierung. Geeignet für die Kommunikation mit Sensoren und anderen ICs über kurze Distanzen bei moderaten Datenraten.

> Siehe [Vorlesung 04 -- Kommunikationsprotokolle](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/04_Kommunikationsprotokolle.md)

### Interrupt

Asynchrones Signal, das den normalen Programmablauf unterbricht und eine Interrupt-Service-Routine (ISR) auslöst. Ermöglicht die zeitnahe Reaktion auf Ereignisse ohne Polling. Wichtige Konzepte: Interrupt-Vektor-Tabelle, globales/individuelles Enable, Priorisierung.

> Siehe [Vorlesung 03 -- Interrupts, Timer, ADC](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/03_InterruptsTimerADC.md)

### ISR (Interrupt Service Routine)

Funktion, die beim Auftreten eines bestimmten Interrupts automatisch ausgeführt wird. Sollte möglichst kurz gehalten werden, um andere Interrupts nicht zu blockieren. Beim AVR: Deklaration über das Makro `ISR(VECTOR_NAME)`.

> Siehe [Vorlesung 03 -- Interrupts, Timer, ADC](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/03_InterruptsTimerADC.md)

### Mikrocontroller vs. Mikroprozessor

Ein **Mikroprozessor** enthält nur die CPU und benötigt externe Komponenten (Speicher, Peripherie). Ein **Mikrocontroller** integriert CPU, Speicher (Flash, SRAM, EEPROM), Peripherie (GPIO, Timer, ADC, Kommunikation) und Taktgenerator auf einem Chip -- ein vollständiges System-on-Chip (SoC).

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### MPU (Memory Protection Unit)

Hardware-Einheit im Cortex-M, die Speicherbereiche mit Zugriffsrechten (Lese-/Schreibschutz, Ausführungsschutz) versehen kann. Wird in RTOS-Umgebungen genutzt, um Tasks voneinander zu isolieren und Speicherfehler zu erkennen.

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### NVIC (Nested Vectored Interrupt Controller)

Interrupt-Controller des Cortex-M mit konfigurierbaren Prioritätsstufen und Unterstützung für verschachtelte (nested) Interrupts. Ermöglicht deterministische Interrupt-Latenzzeiten und ist ein wesentlicher Vorteil gegenüber dem einfachen AVR-Interrupt-System.

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### Pipelining

Technik zur Beschleunigung der Befehlsausführung, bei der aufeinanderfolgende Befehle in verschiedenen Verarbeitungsstufen (Fetch, Decode, Execute) gleichzeitig bearbeitet werden. Der ATmega verwendet eine 2-stufige, der Cortex-M eine 3-stufige Pipeline.

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md)

### PORTMUX (Port Multiplexer)

Mechanismus des XMEGA, der die flexible Zuordnung von Peripheriefunktionen (UART, SPI, I2C) zu verschiedenen physischen Pins ermöglicht. Erleichtert das PCB-Layout und vermeidet Konflikte bei der Pin-Belegung.

> Siehe [Vorlesung 05 -- XMEGA](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/05_XMEGA.md)

### Register

Schnelle, CPU-interne Speicherzellen zur temporären Datenhaltung während der Befehlsausführung. Der ATmega besitzt 32 Arbeitsregister (R0--R31) sowie spezielle I/O-Register zur Peripherie-Steuerung.

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md), [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### RISC (Reduced Instruction Set Computer)

Prozessorarchitektur mit einem reduzierten, einheitlichen Befehlssatz. Befehle sind gleich lang und werden idealerweise in einem Takt ausgeführt. Beispiele: AVR, ARM. Im Gegensatz zu CISC wird Komplexität in den Compiler verlagert.

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md)

### SPI (Serial Peripheral Interface)

Synchrones serielles Kommunikationsprotokoll mit vier Leitungen (MOSI, MISO, SCK, SS). Ermöglicht Vollduplex-Kommunikation bei hohen Datenraten. Verwendet ein Master-Slave-Prinzip mit dedizierter Chip-Select-Leitung pro Slave.

> Siehe [Vorlesung 04 -- Kommunikationsprotokolle](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/04_Kommunikationsprotokolle.md)

### SRAM (Static Random Access Memory)

Flüchtiger Arbeitsspeicher des Mikrocontrollers für Variablen, Stack und Heap. "Statisch" bedeutet, dass die Daten ohne Refresh erhalten bleiben, solange Spannung anliegt. Beim ATmega328: 2 KB, beim Cortex-M4 (STM32F401): 96 KB.

> Siehe [Vorlesung 02 -- ATmega-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/02_ATmegaArchitektur.md)

### SysTick

Systemtimer im Cortex-M, der speziell für RTOS-Zwecke vorgesehen ist. Erzeugt periodische Interrupts für den Scheduler (Tick-Interrupt). Wird typischerweise mit 1 kHz konfiguriert (1 ms Tick-Periode).

> Siehe [Vorlesung 06 -- Cortex-M Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/06_CortexM.md)

### Timer/Counter

Peripheriemodul, das unabhängig von der CPU Takte zählt. Wird für Zeitmessung, PWM-Erzeugung, periodische Interrupt-Generierung und Ereigniszählung eingesetzt. Beim ATmega: Timer0 (8-Bit), Timer1 (16-Bit), Timer2 (8-Bit).

> Siehe [Vorlesung 03 -- Interrupts, Timer, ADC](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/03_InterruptsTimerADC.md)

### UART (Universal Asynchronous Receiver Transmitter)

Asynchrones serielles Kommunikationsprotokoll mit zwei Leitungen (TX, RX). Benötigt keine gemeinsame Taktleitung, dafür müssen Sender und Empfänger die gleiche Baudrate verwenden. Grundlage für RS-232 und viele Debug-Schnittstellen.

> Siehe [Vorlesung 04 -- Kommunikationsprotokolle](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/04_Kommunikationsprotokolle.md)

### UPDI (Unified Program and Debug Interface)

Ein-Draht-Debug- und Programmierschnittstelle des XMEGA. Ersetzt die bisherigen separaten ISP/debugWIRE/JTAG-Schnittstellen durch ein einheitliches Interface und benötigt nur einen Pin.

> Siehe [Vorlesung 05 -- XMEGA](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/05_XMEGA.md)

### Von-Neumann-Architektur

Rechnermodell mit fünf Funktionseinheiten: Steuerwerk, Rechenwerk (ALU), Speicherwerk, Eingabewerk und Ausgabewerk. Programme und Daten teilen sich einen gemeinsamen Speicher (im Gegensatz zur Harvard-Architektur).

> Siehe [Vorlesung 01 -- Grundbegriffe](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/01_Grundbegriffe.md)

-----

## Block II: Echtzeitbetriebssysteme

> Vorlesungen 07--09: RTOS-Konzepte, Scheduling und FreeRTOS.

### BCET (Best-Case Execution Time)

Die kürzeste Ausführungszeit eines Tasks unter günstigsten Bedingungen. Zusammen mit der WCET definiert sie den Zeitkorridor, in dem ein Task abläuft. Relevant für die Analyse des Jitters.

> Siehe [Vorlesung 07 -- RTOS-Konzepte](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/07_RTOS_Konzepte.md)

### Deadline

Der späteste Zeitpunkt, bis zu dem ein Task seine Berechnung abgeschlossen haben muss. Unterscheidung: Bei **harten** Echtzeitsystemen führt eine Verletzung zu einem Systemversagen, bei **weichen** zu einer Qualitätsminderung.

> Siehe [Vorlesung 07 -- RTOS-Konzepte](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/07_RTOS_Konzepte.md)

### EDD (Earliest Due Date)

Scheduling-Algorithmus für aperiodische Tasks, der den Task mit der frühesten Deadline zuerst ausführt. Optimal für die Minimierung der maximalen Verspätung bei einer Menge einmaliger Aufgaben.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md)

### FreeRTOS

Weit verbreitetes Open-Source-Echtzeitbetriebssystem für Mikrocontroller. Bietet präemptives und kooperatives Scheduling, Semaphore, Mutexe, Queues und Software-Timer. Im Kurs auf dem STM32 und ESP32-S3 eingesetzt.

> Siehe [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### Harte vs. weiche Echtzeit

**Harte Echtzeit**: Jede Deadline muss eingehalten werden -- eine Verletzung ist ein Systemfehler (z.B. Airbag-Auslösung). **Weiche Echtzeit**: Gelegentliche Deadline-Verletzungen sind tolerierbar und führen nur zu Qualitätseinbußen (z.B. Video-Streaming).

> Siehe [Vorlesung 07 -- RTOS-Konzepte](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/07_RTOS_Konzepte.md)

### Kooperatives vs. präemptives Multitasking

**Kooperativ**: Tasks geben die CPU freiwillig ab (z.B. durch `taskYIELD()`). **Präemptiv**: Der Scheduler kann einen laufenden Task jederzeit unterbrechen, um einen höher priorisierten Task auszuführen. FreeRTOS unterstützt beide Modi.

> Siehe [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### Mutex (Mutual Exclusion)

Synchronisationsmechanismus zum exklusiven Zugriff auf gemeinsam genutzte Ressourcen. Nur der Task, der den Mutex besitzt, kann ihn wieder freigeben. FreeRTOS-Mutexe unterstützen Priority Inheritance zur Vermeidung von Prioritätsinversion.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md), [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### PCP (Priority Ceiling Protocol)

Protokoll zur Vermeidung von Prioritätsinversion und Deadlocks. Jedem Mutex wird eine Prioritäts-Obergrenze (Ceiling) zugewiesen -- die höchste Priorität aller Tasks, die ihn verwenden können. Ein Task darf einen Mutex nur sperren, wenn seine Priorität höher ist als die Ceilings aller aktuell gesperrten Mutexe.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md)

### PIP (Priority Inheritance Protocol)

Protokoll zur Abschwächung von Prioritätsinversion. Wenn ein niedrig priorisierter Task eine Ressource hält, die ein höher priorisierter Task benötigt, erbt der niedrig priorisierte Task temporär die höhere Priorität, bis er die Ressource freigibt.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md)

### Prioritätsinversion

Situation, in der ein hoch priorisierter Task durch einen niedrig priorisierten Task blockiert wird, weil dieser eine gemeinsame Ressource hält. Kann durch mittlere Tasks, die den niedrigen Task verdrängen, verschärft werden. Lösungen: PIP, PCP.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md)

### Queue (Nachrichtenwarteschlange)

FIFO-Puffer zur Kommunikation zwischen Tasks oder zwischen ISR und Task. Ermöglicht den sicheren Datenaustausch ohne gemeinsame Variablen. In FreeRTOS: `xQueueCreate()`, `xQueueSend()`, `xQueueReceive()`.

> Siehe [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### Rate Monotonic Scheduling

Optimaler statischer Scheduling-Algorithmus für periodische Tasks: Der Task mit der kürzesten Periode erhält die höchste Priorität. Garantiert Schedulability, wenn die Gesamtauslastung unter $n \cdot (2^{1/n} - 1)$ liegt (ca. 69% für viele Tasks).

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md)

### Semaphor

Synchronisationsprimitiv zur Koordination von Tasks. **Binärer Semaphor**: signalisiert Ereignisse (z.B. ISR -> Task). **Zählender Semaphor**: kontrolliert den Zugang zu einer begrenzten Anzahl gleichartiger Ressourcen. Im Unterschied zum Mutex hat der Semaphor keinen Besitzer.

> Siehe [Vorlesung 08 -- Scheduling-Algorithmen](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/08_SchedulingAlgorithmen.md), [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### Super Loop

Einfachstes Architekturmuster für eingebettete Software: eine Endlosschleife (`while(1)`), die sequentiell alle Aufgaben abarbeitet. Vorteile: einfach, deterministisch. Nachteile: keine echte Nebenläufigkeit, schwierige Einhaltung von Timing-Anforderungen bei wachsender Komplexität.

> Siehe [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### Task (RTOS)

Ausführungseinheit in einem RTOS -- eine Funktion mit eigenem Stack und eigenem Kontext. Zustände: **Running** (wird ausgeführt), **Ready** (bereit, wartet auf CPU), **Blocked** (wartet auf Ereignis/Ressource), **Suspended** (manuell angehalten).

> Siehe [Vorlesung 07 -- RTOS-Konzepte](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/07_RTOS_Konzepte.md), [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### TCB (Task Control Block)

Datenstruktur, in der das RTOS den Zustand eines Tasks speichert: Stackpointer, Priorität, Task-Zustand, Name und weitere Verwaltungsinformationen. Wird bei jedem Kontextwechsel gelesen und aktualisiert.

> Siehe [Vorlesung 09 -- FreeRTOS](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md)

### WCET (Worst-Case Execution Time)

Die längste Ausführungszeit eines Tasks unter ungünstigsten Bedingungen (Cache-Misses, Interrupts, Speicherzugriffskonflikte). Grundlage jeder Echtzeitanalyse -- die Einhaltung von Deadlines muss auch im Worst Case garantiert werden.

> Siehe [Vorlesung 07 -- RTOS-Konzepte](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/07_RTOS_Konzepte.md)

-----

## Block III: Software-Methodik

> Vorlesungen 10--12: Architektur, Testen, CI/CD und Fehlertoleranz.

### Assertion

Zusicherung im Code, die eine Bedingung zur Laufzeit prüft und bei Verletzung das Programm anhält (oder eine Fehlerbehandlung auslöst). Dient der frühzeitigen Erkennung von Programmierfehlern und unzulässigen Zuständen.

> Siehe [Vorlesung 11 -- Debugging, Testen & CI/CD](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/11_Debugging_Testen_CICD.md)

### AUTOSAR (Automotive Open System Architecture)

Standardisierte Software-Architektur für Steuergeräte in der Automobilindustrie. **Classic**: statisch konfiguriert, für harte Echtzeit. **Adaptive**: dynamisch, POSIX-basiert, für leistungsfähige Plattformen (z.B. autonomes Fahren).

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### BSP (Board Support Package)

Software-Schicht zwischen HAL und Anwendung, die board-spezifische Konfigurationen kapselt: Pin-Zuordnungen, Taktfrequenzen, Speicher-Layout. Ermöglicht den Wechsel des konkreten Boards, ohne die Anwendungslogik zu ändern.

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### CI/CD (Continuous Integration / Continuous Deployment)

Automatisierte Build-, Test- und Deployment-Pipeline. **CI** stellt sicher, dass jede Code-Änderung kompiliert und die Tests besteht. **CD** automatisiert die Auslieferung (z.B. Firmware-Updates). Besondere Herausforderungen in der Embedded-Welt: Hardware-in-the-Loop-Tests, Cross-Compilation.

> Siehe [Vorlesung 11 -- Debugging, Testen & CI/CD](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/11_Debugging_Testen_CICD.md)

### Dependability (Verlässlichkeit)

Oberbegriff für die Fähigkeit eines Systems, einen bestimmten Dienst zuverlässig und vertrauenswürdig zu erbringen. Umfasst: Reliability (Zuverlässigkeit), Availability (Verfügbarkeit), Safety (Sicherheit), Integrity (Integrität) und Maintainability (Wartbarkeit).

> Siehe [Vorlesung 12 -- Fehlertoleranz](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/12_Fehlertoleranz.md)

### HiL (Hardware-in-the-Loop)

Testverfahren, bei dem die reale Hardware (z.B. Steuergerät) in eine simulierte Umgebung (Streckenmodell, Sensorik, Aktorik) eingebettet wird. Ermöglicht realitätsnahe Tests, ohne das physische Gesamtsystem aufbauen zu müssen. Stufe nach MiL und SiL.

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### MiL (Model-in-the-Loop)

Früheste Verifikationsstufe in der modellbasierten Entwicklung. Das Regelungsmodell (z.B. in Simulink) und das Streckenmodell werden gemeinsam simuliert -- ohne reale Hardware oder generierten Code.

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### MTBF (Mean Time Between Failures)

Mittlere Betriebsdauer zwischen zwei Ausfällen eines reparierbaren Systems. Wichtige Kenngröße der Zuverlässigkeit. Berechnung: $MTBF = \frac{\text{Gesamtbetriebszeit}}{\text{Anzahl Ausfälle}}$.

> Siehe [Vorlesung 12 -- Fehlertoleranz](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/12_Fehlertoleranz.md)

### Redundanz

Einsatz zusätzlicher, für den Normalbetrieb nicht zwingend erforderlicher Komponenten zur Erhöhung der Zuverlässigkeit. Formen: **Hardware-Redundanz** (mehrfache Sensoren/Aktoren), **Software-Redundanz** (N-Version-Programming), **Informationsredundanz** (Prüfsummen, ECC), **Zeitredundanz** (Wiederholung).

> Siehe [Vorlesung 12 -- Fehlertoleranz](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/12_Fehlertoleranz.md)

### SiL (Software-in-the-Loop)

Verifikationsstufe, bei der der aus dem Modell generierte Code auf einem PC ausgeführt und gegen das Streckenmodell getestet wird. Prüft die korrekte Code-Generierung, bevor Hardware ins Spiel kommt.

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### Statechart (Harel-Zustandsdiagramm)

Erweiterung klassischer endlicher Automaten um Hierarchie (Zustandsverschachtelung), Nebenläufigkeit (parallele Regionen) und History-Zustände. Ermöglicht die kompakte Modellierung komplexer Verhaltenslogik in eingebetteten Systemen.

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

### Statische Code-Analyse

Untersuchung des Quellcodes ohne Ausführung. Erkennt potenzielle Fehler wie uninitialisierte Variablen, Pufferüberläufe, nicht erreichbaren Code oder MISRA-C-Verletzungen. Werkzeuge: PC-Lint, Cppcheck, Polyspace.

> Siehe [Vorlesung 11 -- Debugging, Testen & CI/CD](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/11_Debugging_Testen_CICD.md)

### Watchdog-Timer

Hardware-Timer, der das System zurücksetzt, wenn er nicht regelmäßig durch die Software zurückgesetzt ("gefüttert") wird. Schutzmechanismus gegen Endlosschleifen, Deadlocks und andere Fehlerzustände, die die normale Programmausführung verhindern.

> Siehe [Vorlesung 11 -- Debugging, Testen & CI/CD](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/11_Debugging_Testen_CICD.md)

### Zustandsautomat (State Machine)

Modell, das ein System als endliche Menge von Zuständen mit definierten Übergängen beschreibt. In eingebetteten Systemen weit verbreitet zur Steuerungslogik. Unterscheidung: **flache** Automaten (einfache Zustandsliste) vs. **hierarchische** Automaten (Statecharts).

> Siehe [Vorlesung 10 -- SW-Architektur](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/10_SW_Architektur.md)

-----

## Block IV: Ausblick & Abschluss

> Vorlesungen 13--14: Embedded Linux, Multicore und Edge AI.

### AMP (Asymmetric Multiprocessing)

Multiprocessing-Architektur, bei der verschiedene Kerne unterschiedliche Betriebssysteme oder Aufgaben ausführen. Beispiel: Ein Cortex-A-Kern mit Linux für die Applikationslogik und ein Cortex-M-Kern mit FreeRTOS für die Echtzeit-Steuerung.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

### Buildroot

Build-System zur Erstellung kompletter Embedded-Linux-Images (Toolchain, Kernel, Root-Dateisystem). Konfiguration über `menuconfig`. Einfacher als Yocto, aber weniger flexibel für große Projekte.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

### Device Tree

Datenstruktur, die die Hardware-Konfiguration einer Plattform beschreibt (Peripherie, Speicherbereiche, Interrupt-Zuordnungen). Wird dem Linux-Kernel beim Booten übergeben und macht ihn plattformunabhängig -- die Hardware-Beschreibung liegt nicht im Kernel-Code.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

### ESP32-S3

Dual-Core-Mikrocontroller (Xtensa LX7, 240 MHz) von Espressif mit integriertem Wi-Fi und Bluetooth LE. Bietet Vektor-Erweiterungen (PIE) für maschinelles Lernen. Im Kurs die leistungsfähigste Plattform (Arduino Nano ESP32).

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### microROS

ROS-2-Framework für Mikrocontroller. Ermöglicht die Integration von RTOS-basierten Knoten in ein ROS-2-Netzwerk über einen Agent auf einem Linux-Host. Bridging zwischen Echtzeit-Welt und dem ROS-Ökosystem.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

### OTA (Over-the-Air Update)

Aktualisierung der Firmware eines eingebetteten Systems über eine drahtlose Verbindung (Wi-Fi, BLE, Mobilfunk). Erfordert besondere Maßnahmen: Dual-Partition-Schema (Rollback bei Fehler), kryptographische Signatur, Integritätsprüfung.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### PREEMPT_RT

Linux-Kernel-Patch, der den Kernel nahezu vollständig präemptibel macht. Wandelt Spinlocks in schlafbare Mutexe um und ermöglicht Interrupt-Behandlung in Kernel-Threads. Ergebnis: Latenzzeiten im Bereich weniger Mikrosekunden -- eine Annäherung an harte Echtzeit.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

### Quantisierung (TinyML)

Umwandlung eines neuronalen Netzes von Gleitkomma- in Ganzzahl-Darstellung (z.B. Float32 -> Int8). Reduziert Modellgröße und Rechenaufwand drastisch und ermöglicht die Ausführung von ML-Modellen auf Mikrocontrollern mit begrenzten Ressourcen.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### Register Windows (Xtensa)

Mechanismus der Xtensa-Architektur (ESP32-S3), bei dem Funktionsaufrufe nicht den gesamten Registersatz auf den Stack sichern, sondern ein "Fenster" über den physischen Registersatz verschieben. Beschleunigt Funktionsaufrufe und -rücksprünge.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### SMP (Symmetric Multiprocessing)

Multiprocessing-Architektur, bei der alle Kerne gleichberechtigt sind und dasselbe Betriebssystem ausführen. Beim ESP32-S3 verteilt der FreeRTOS-Scheduler Tasks automatisch auf beide Kerne; mit `xTaskCreatePinnedToCore()` kann eine Core-Affinität festgelegt werden.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### Spinlock

Synchronisationsmechanismus für Multicore-Systeme, bei dem ein Kern in einer aktiven Schleife wartet, bis ein Lock frei wird. Geeignet für sehr kurze kritische Abschnitte; bei langen Wartezeiten ineffizient (Busyblocker). Im ESP32-S3 für Inter-Core-Synchronisation eingesetzt.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### TFLite Micro (TensorFlow Lite for Microcontrollers)

Inferenz-Framework von Google für die Ausführung vortrainierter ML-Modelle auf Mikrocontrollern. Workflow: Modell trainieren (PC/Cloud) -> Quantisieren -> Als C-Array exportieren -> Auf MCU ausführen. Kein Betriebssystem erforderlich.

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### TinyML

Oberbegriff für maschinelles Lernen auf ressourcenbeschränkten Geräten (Mikrocontroller mit wenigen KB bis MB RAM). Typische Anwendungen: Keyword Spotting, Anomalie-Erkennung, Gestenerkennung -- direkt auf dem Gerät, ohne Cloud-Anbindung (Edge AI).

> Siehe [Vorlesung 14 -- ESP32-S3](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/14_ESP32S3.md)

### Yocto Project

Mächtiges Build-Framework für Embedded Linux, das auf einem Layer-Konzept basiert. Jeder Layer kann Board-Support, Software-Pakete oder Konfigurationen beisteuern. Flexibler als Buildroot, aber mit steilerer Lernkurve. Erzeugt vollständige Linux-Distributionen.

> Siehe [Vorlesung 13 -- Embedded Linux](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/13_EmbeddedLinux.md)

-----

## Plattformvergleich

Die folgende Tabelle fasst die vier Kursplattformen zusammen:

<!-- data-type="none" -->
| Eigenschaft       | ATmega328             | ATmega4809 (XMEGA)       | STM32F401 (Cortex-M4)     | ESP32-S3                    |
| :---------------- | :-------------------- | :------------------------ | :------------------------- | :-------------------------- |
| **Architektur**   | 8-Bit AVR (RISC)      | 8-Bit AVR (RISC)          | 32-Bit ARM (RISC)          | 32-Bit Xtensa LX7 (RISC)   |
| **Takt**          | 16 MHz                | 20 MHz                    | 84 MHz                     | 240 MHz                     |
| **Flash**         | 32 KB                 | 48 KB                     | 512 KB                     | bis 16 MB (extern)          |
| **SRAM**          | 2 KB                  | 6 KB                      | 96 KB                      | 512 KB                      |
| **Kerne**         | 1                     | 1                         | 1                          | 2 (SMP)                     |
| **FPU**           | --                    | --                        | Ja (Single Precision)      | --                          |
| **Konnektivität** | --                    | --                        | --                         | Wi-Fi, BLE                  |
| **Debug**         | ISP/debugWIRE         | UPDI                      | SWD/JTAG                   | JTAG                        |
| **Besonderheit**  | Einfachheit           | Event System, CCL         | DMA, NVIC, MPU             | Dual-Core, PIE, TinyML      |
| **Vorlesung**     | 02                    | 05                        | 06                         | 14                          |
