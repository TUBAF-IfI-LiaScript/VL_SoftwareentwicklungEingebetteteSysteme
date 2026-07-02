<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.1.0
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/exercises/05_Zusammenfassung.md#1)

# Debugging in der Praxis — Bug Hunt

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Softwareentwicklung für eingebettete Systeme`                                                                                                                                |
| **Semester**             | `Sommersemester 2026`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Praktisches Debugging am STM32 mit GDB / OpenOCD / PlatformIO`                                                                                                                       |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/exercises/05_Zusammenfassung.md](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/exercises/05_Zusammenfassung.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Worum geht es?

In der Vorlesung haben wir Debugging von Testen unterschieden: Ein Test findet
*dass* etwas kaputt ist — der Debugger hilft, *warum* zu verstehen. Genau das
üben wir hier **praktisch am STM32 Nucleo-F401RE**.

Das Beispielprojekt liegt im Repository:

[codeExamples/STM32/Debug_BugHunt](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/tree/main/codeExamples/STM32/Debug_BugHunt)

Es ist ein kleines Programm, das ein Feld von fünf Messwerten auswertet und
**Summe**, **Mittelwert** und **Maximum** bestimmt:

```c
#define N 5
const uint16_t messwerte[N] = { 200, 180, 220, 190, 210 };
```

Die **korrekten** Ergebnisse sind:

| Größe   | Sollwert                          |
| ------- | --------------------------------- |
| Summe   | 200+180+220+190+210 = **1000**    |
| Mittel  | 1000 / 5 = **200**                |
| Maximum | **220**                           |

Das Programm liefert aber **falsche** Werte — die grüne LED (LD2) blinkt
deshalb *langsam* (800 ms) statt *schnell* (100 ms). Es stecken **drei Fehler**
im Code, aufsteigend in der Schwierigkeit. Deine Aufgabe: sie **mit dem
Debugger** finden und erklären — nicht durch bloßes Lesen des Quelltextes.

> **Regel der Übung:** Für jeden Fehler sollst du (a) im Debugger den *falschen*
> Wert einer Variable **konkret zeigen** und (b) die *Ursache* benennen. Ein
> reines „da fehlt eine 0" ohne Debugger-Nachweis zählt nicht.

## Vorbereitung: das Board zum Debuggen bringen

Du kannst **einen von zwei Wegen** wählen — beide sind im Repository als
lauffähige Beispiele beschrieben:

1. **Komfortabel (empfohlen für den Einstieg):**
   [Debug_PlatformIO](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/tree/main/codeExamples/STM32/Debug_PlatformIO)
   — in VS Code Breakpoint setzen, **F5** drücken (*PIO Debug*).
2. **Unter der Haube:**
   [Debug_OpenOCD_GDB](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/tree/main/codeExamples/STM32/Debug_OpenOCD_GDB)
   — OpenOCD als GDB-Server starten und `arm-none-eabi-gdb` von Hand verbinden.

```bash
# bauen und flashen
pio run
pio run -t upload
```

Setze einen ersten Breakpoint auf den Aufruf `auswerten();` in `main()` und
arbeite dich von dort vor.

## Aufgabe 1 — der Overflow (leicht)

Setze einen Breakpoint in die Summen-Schleife der Funktion `auswerten()` und
beobachte die Variable `sum` im Watch-Fenster, während du mit *Step Over* (F10)
durch die Schleife gehst.

> **1a)** Welchen Wert hat `sum` am Ende der Schleife — und warum ist er **nicht**
> 1000? Notiere den beobachteten Wert.
>
> **1b)** Wirf einen Blick auf den **Datentyp** von `sum`. Welchen Wertebereich
> deckt er ab? Rechne nach: Was ergibt 1000 „modulo" diesem Bereich?
>
> **1c)** Wie behebt man den Fehler mit einer minimalen Änderung?

<!--
Lösungshinweis (für Betreuer):
`sum` ist `uint8_t` (0..255). Die Aufsummierung läuft über; am Chip beobachtet:
sum = 232  (= 1000 mod 256). Folgefehler: average = 232/5 = 46.
Fix: sum als uint16_t (oder uint32_t) deklarieren.
-->

## Aufgabe 2 — die uninitialisierte Variable (mittel)

Die Maximum-Bestimmung nutzt eine **lokale** Variable `lokales_max`. Setze
einen Breakpoint auf die *erste* Zeile der Maximum-Schleife und sieh dir
`lokales_max` an, **bevor** die Schleife das erste Mal vergleicht
(`print lokales_max` bzw. Watch-Fenster).

> **2a)** Welchen Wert hat `lokales_max` *vor* der ersten Nutzung? Woher kommt
> dieser Wert?
>
> **2b)** Führe das Programm mehrmals aus (Reset + neu debuggen). Ist der
> Startwert **immer gleich**? Was bedeutet das für die Reproduzierbarkeit des
> Fehlers?
>
> **2c)** Warum wird eine *globale* Variable dieses Problem in der Regel **nicht**
> zeigen, eine *lokale* aber schon? (Stichwort: `.bss`-Initialisierung im
> Startup-Code vs. Stack.)
>
> **2d)** Wie behebt man den Fehler?

<!--
Lösungshinweis (für Betreuer):
`lokales_max` wird ohne Startwert benutzt -> Stack-Müll. Da es lokal ist, wird
es NICHT vom Startup auf 0 genullt (das gilt nur für .bss/globale Variablen).
Der Fehler ist NICHTDETERMINISTISCH: ist der Müllwert zufällig größer als alle
Messwerte, bleibt das Maximum falsch; ist er kleiner, "funktioniert" es zufällig.
Genau das ist die Gefahr solcher Bugs — deshalb: VOR der Nutzung inspizieren,
nicht nur das Endergebnis prüfen.
Fix: `uint16_t lokales_max = 0;`
-->

## Aufgabe 3 — der Off-by-one (schwer)

Am Ende von `auswerten()` steht eine Prüf-Schleife über das Array. Setze einen
Breakpoint auf die Schleife und beobachte die Laufvariable `i` sowie den Index,
mit dem auf `messwerte[i]` zugegriffen wird.

> **3a)** Welchen größten Wert erreicht `i`? Wie viele Elemente hat `messwerte`
> — und welcher Index ist damit der letzte **gültige**?
>
> **3b)** Auf welches Element greift die Schleife im letzten Durchlauf zu? Was
> liegt an dieser Speicherstelle (Tipp: Nachbarvariable `guard` mit dem
> auffälligen Wert `0xBEEF` — betrachte sie im Watch oder setze einen
> **Watchpoint**: `watch guard`).
>
> **3c)** Warum ist so ein Fehler besonders tückisch (Stichwort: liest
> „zufällig" gültig aussehenden Speicher, stürzt aber nicht zwingend ab)?
>
> **3d)** Korrigiere die Schleifenbedingung.

<!--
Lösungshinweis (für Betreuer):
`for (i = 0; i <= N; i++)` greift auf messwerte[5] zu — ein Element hinter dem
Array (gültig sind 0..4). Ein Watchpoint auf `guard` zeigt den Fremdzugriff auf
Nachbarspeicher. Fix: Bedingung `i < N`.
-->

## Abschluss

> Wenn alle drei Fehler behoben sind, liefert das Programm `sum = 1000` (bzw.
> mit korrektem Typ), `average = 200`, `maximum = 220` — und die LED blinkt
> **schnell** (100 ms). Prüfe das am Board.

**Zum Nachdenken:** Welche der drei Fehler hätte ein Host-seitiger Unit-Test
(ohne Hardware) gefunden? Welcher ist am Target/im Debugger leichter zu sehen?
(Vgl. die Test-Pyramide aus der Vorlesung.)

## Organisation der Prüfungen

> Die Veranstaltung wird mit einer mündlichen Prüfung abgeschlossen, die aus zwei Teilen besteht:

1. eigenes Implementierungsbeispiel (ATmega328, ATmega4809 mittels Assembler oder avrlibc, STM32F4 mittels HAL) ODER Edrys Beispiellabor aus dem Praktikumsteil
2. einer Diskussion zu den Themenfeldern der Vorlesung

Das konkrete Beispiel können Sie selbst wählen. Melden Sie sich bitte zur Abstimmung eines Termines.

## Diskussion des vergangenen Aufgabenblattes

> Hier sind Sie gefragt ...
