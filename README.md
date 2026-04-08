# Softwareentwicklung Eingebettete Systeme

Dieses Repository umfasst die Materialien für die Veranstaltung "Softwareentwicklung
Eingebettete Systeme" an der TU Bergakademie Freiberg (Master). Die Vorlesung hat 4 Schwerpunkte:

+ Auffrischung und Erweiterung der Architekturbegriffe von Mikrocontrollern (ATmega → XMEGA → Cortex-M4)
+ Diskussion von Echtzeitbetriebssystemen (Konzepte, Scheduling, FreeRTOS)
+ Methodische Softwareentwicklung für eingebettete Systeme (Architektur, Testen, CI/CD, Fehlertoleranz)
+ Ausblick auf Embedded Linux, Multicore und Edge AI (ESP32-S3)

Grundlage der Diskussionen und praktischen Übungen sind

+ die Mikrocontrollerfamilie ATmega von Microchip
+ die Mikrocontrollerfamilie XMEGA von Microchip
+ die Implementierung der Cortex-M Controller durch STM
+ den Arduino Nano ESP32 (Espressif ESP32-S3, Dual-Core, Vektor-Erweiterungen)

_Zeitplan_

| VL  | Tag    | Inhalt der Vorlesung                              | Block                              |
| --- | ------ | :------------------------------------------------ | ---------------------------------- |
| 0   | 01.04. | Einführung, Motivation, Grundbegriffe             | **I: Hardware-Progression**        |
| 1   | 08.04. | ATmega-Architektur                                |                                    |
| 2   | 15.04. | Interrupts, Timer, ADC, 8-Bit-Limitierungen      |                                    |
| 3   | 22.04. | Kommunikationsprotokolle (UART, I2C, SPI, CAN)   |                                    |
| 4   | 29.04. | XMEGA-Erweiterungen (ATmega4809)                 |                                    |
| 5   | 06.05. | Cortex-M Architektur & Peripherie                 | Hardware abgeschlossen             |
| 6   | 13.05. | RTOS-Konzepte & Task-Modell                       | **II: Echtzeitbetriebssysteme**    |
| 7   | 20.05. | Scheduling-Algorithmen & Synchronisation          |                                    |
| 8   | 27.05. | FreeRTOS in der Praxis                            | Echtzeitdiskussion abgeschlossen   |
| 9   | 03.06. | SW-Architektur für eingebettete Systeme           | **III: SW-Methodik**               |
| 10  | 10.06. | Debugging, Testen & CI/CD                         |                                    |
| 11  | 17.06. | Fehlertoleranz & Zuverlässigkeit                  |                                    |
| 12  | 24.06. | Embedded Linux vs. Bare Metal / RTOS              | SW-Methodik abgeschlossen          |
| 13  | 01.07. | Multicore & Edge AI: ESP32-S3                     | **IV: Ausblick & Abschluss**       |
| 14  | 08.07. | Vorstellung der Praktikumsergebnisse              |                                    |
| 15  | 15.07. | _Reserve_                                         |                                    |
