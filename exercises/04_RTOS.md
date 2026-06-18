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


> Und wie funktioniert der Kontextwechsel auf dem **AVR (ATmega328)**?

Der gesamte Kontextwechsel passiert per Software – in
FreeRTOS für AVR direkt im **Tick-Interrupt** (bzw. in `yield`). Die ISR sichert den kompletten
Registersatz selbst auf den Stack des laufenden Tasks, tauscht den Stackpointer und stellt den
Kontext des nächsten Tasks wieder her.

```ascii
┌────────────────────────────┐
│       Task A läuft         │   ← Anwendungscode, Interrupts aktiv
└────────────┬───────────────┘
             │ Tick-Interrupt (Timer) oder taskYIELD()
             ▼
┌────────────────────────────┐
│  ISR-Prolog (Makro         │   ← Auslöser: Hardware springt in den ISR-Vektor,
│  portSAVE_CONTEXT):        │     PC liegt bereits auf dem Stack von Task A
│  ▸ alle 32 Register R0–R31 │   ← Software sichert den KOMPLETTEN Registersatz
│  ▸ SREG (Statusregister)   │     (keine HW-Hilfe wie beim Cortex-M!)
│    auf Stack von Task A    │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│  aktuellen Stackpointer in │   ← der gesicherte Kontext wird über den SP
│  TCB von Task A sichern    │     im TCB "verankert" – so findet ihn FreeRTOS
│  (pxCurrentTCB→pxTopOf-    │     beim nächsten Mal wieder
│   Stack = SP)              │
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│  vTaskSwitchContext():     │   ← Scheduler-Entscheidung: höchstpriorer
│  ▸ wählt nächsten Task     │     ready-Task wird gewählt (Round-Robin bei
│  ▸ pxCurrentTCB → Task B   │     gleicher Priorität)
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│  SP = pxCurrentTCB→        │   ← Kern des Wechsels: NUR der Stackpointer wird
│       pxTopOfStack         │     umgebogen → ab jetzt arbeiten wir auf
│  (Stack von Task B)        │     dem Stack von Task B
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│  ISR-Epilog (Makro         │   ← spiegelbildlich zum Prolog: der bei Task B
│  portRESTORE_CONTEXT):     │     früher gesicherte Kontext wird zurückgeladen
│  ▸ SREG + R0–R31 von       │
│    Task-B-Stack zurück     │
│  ▸ RETI                    │   ← RETI holt PC vom Stack + aktiviert Interrupts
└────────────┬───────────────┘
             │
             ▼
┌────────────────────────────┐
│        Task B läuft        │   ← läuft genau dort weiter, wo er zuletzt
└────────────────────────────┘     unterbrochen wurde                                                                         .
```

> ... und wie sieht der Stack eines AVR-Tasks dabei aus?

```ascii
Stack eines AVR-Tasks (komplett per Software gesichert)

┌───────────────┐ ← höchste Adresse (Stack wächst nach unten)
│ Task-Funktion │   PC (Rücksprung-/Startadresse, 2 bzw. 3 Bytes)
│   (Adresse)   │
├───────────────┤ ← portSAVE_CONTEXT legt ab:
│ R0            │
│ SREG          │   Statusregister – direkt nach R0 gesichert
│ R1            │   (vom OS auf 0 gesetzt erwartet)
│ R2            │
│ ...           │   ▸ KEINE Hardware-Sicherung
│ R31           │   ▸ alle 32 Register manuell auf den Stack
├───────────────┤
│ SP →          │ ← pxTopOfStack, wird im TCB gespeichert
└───────────────┘                                                                                                            .
```

> Aber der native Stack kann doch dafür nicht genutzt werden?

Nein, denn der native Stack (der im Reset-Vektor initialisiert wird) wird von allen Tasks gemeinsam
genutzt – er wächst nach unten und wird von der Hauptfunktion (`main`) und allen ISRs verwendet.
Die Task-Stacks hingegen werden im Heap angelegt (per `pvPortMalloc`) und wachsen nach oben. 

```ascii
SRAM des ATmega328 (2 KB, 0x0100 … 0x08FF)

0x08FF ┌────────────────────────────┐ ← RAMEND
       │  "nativer" Stack (main,    │   wächst nach UNTEN
       │   ISRs, vor Scheduler-Start│        │
       │   – wird zum Idle/Setup)   │        ▼
       │            ...             │
       │   ↕  freier RAM            │
       │            ...             │
       │        ▲                   │
       │        │   Heap wächst     │   ← hier liegen TCB + Task-Stacks
       │   ┌────┴─────┐  nach OBEN  │     (pvPortMalloc → malloc)
       │   │ Stack T2 │             │
       │   │ TCB  T2  │             │
       │   │ Stack T1 │             │
       │   │ TCB  T1  │             │
       │   ├──────────┤ ← __malloc_heap_start
       │   │  .bss    │   globale/statische Variablen
0x0100 │   │  .data   │
       └────────────────────────────┘                                                                                        .
``` 


## Einstieg: FreeRTOS unter PlatformIO (ATmega328 / Arduino Uno)

Für die heutigen Aufgaben arbeiten wir mit dem **ATmega328P** (Arduino Uno) und dem
Arduino-Port von FreeRTOS. 

> Nutzen Sie gern die Aduino-Funktionen (z.B. `digitalWrite`, `attachInterrupt`, …) – sie sind mit FreeRTOS kompatibel. Damit fokussieren wir uns auf die RTOS-Konzepte und müssen nicht zu viel AVR-spezifischen Code schreiben. Das können wir ja schon ...

Die FreeRTOS-API muss beim Aufruf aus ISR und Task heraus korrekt verwendet werden (z.B. `xTaskResumeFromISR` vs. `vTaskResume`), aber die Arduino-Funktionen können wie gewohnt genutzt werden.


### Projekt anlegen

Entweder über die PlatformIO-Oberfläche (*PIO Home → New Project*, Board: `Arduino Uno`,
Framework: `Arduino`) oder direkt auf der Kommandozeile:

```bash
pio project init --board uno
```

Anschließend die Projektkonfiguration so anpassen, dass die FreeRTOS-Bibliothek (Arduino-Port
von Richard Barry / @feilipu) eingebunden wird:

```ini
; platformio.ini
[env:uno]
platform      = atmelavr
board         = uno
framework     = arduino
monitor_speed = 9600
lib_deps      = feilipu/FreeRTOS
build_flags   =
    -D INCLUDE_vTaskDelay=1
    -D INCLUDE_xTaskGetTickCount=1
```

https://registry.platformio.org/libraries/feilipu/FreeRTOS

> **Hinweis:** Die zentralen Konfigurationsschalter von FreeRTOS (Preemption, Heap-Größe,
> `configTICK_RATE_HZ`, …) stehen in der `FreeRTOSConfig.h` des Ports. Für die ersten
> Schritte genügen die Voreinstellungen.

### Beispiel 1 – Blink-Task per externem Interrupt steuern

Ein Blink-Task lässt die LED blinken. Ein **externer Interrupt** (Taster an einem der
Hardware-Interrupt-Pins des ATmega328 – `INT0` = D2 oder `INT1` = D3) schaltet diesen Task
bei jedem Tastendruck abwechselnd ab und wieder an. Dazu nutzen wir die FreeRTOS-Zustände
*Suspended* ↔ *Ready/Running*, die wir mit `vTaskSuspend` / `vTaskResume` umschalten.

Wichtig: Aus einer ISR heraus dürfen **nur** die `…FromISR`-Varianten der API aufgerufen
werden. `vTaskResume` besitzt mit `xTaskResumeFromISR` eine solche Variante; für das
Suspendieren setzen wir in der ISR lediglich ein Flag und führen `vTaskSuspend` im Task selbst
aus.

```cpp
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define TASTER_PIN 2          // INT0 auf dem ATmega328 / Arduino Uno

TaskHandle_t blinkHandle = NULL;
volatile bool taskAktiv   = true;   // gewünschter Zustand des Blink-Tasks

void TaskBlink(void *pvParameters);
void tasterISR();

void setup() {
  pinMode(TASTER_PIN, INPUT_PULLUP);

  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, &blinkHandle);

  // fallende Flanke: Taster zieht den Pin gegen Masse
  attachInterrupt(digitalPinToInterrupt(TASTER_PIN), tasterISR, FALLING);
  // Der Scheduler übernimmt nach setup() automatisch die Kontrolle.
}

void loop() { /* leer – die Arbeit passiert im Task */ }

// --- ISR: möglichst kurz halten -----------------------------------------
void tasterISR() {
  taskAktiv = !taskAktiv;                 // Zustand umschalten
  if (taskAktiv) {
    // Task aus der ISR heraus wieder aufwecken
    xTaskResumeFromISR(blinkHandle);
  }
  // Das Suspendieren übernimmt der Task selbst (siehe unten)
}

// --- Blink-Task ---------------------------------------------------------
void TaskBlink(void *pvParameters) {
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);
  for (;;) {
    if (!taskAktiv) {
      digitalWrite(LED_BUILTIN, LOW);     // LED aus, bevor wir pausieren
      vTaskSuspend(NULL);                 // Task suspendiert sich selbst
    }
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(250));
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}
```

> **Aufbau:** Taster zwischen Pin D2 und GND; der interne Pull-up (`INPUT_PULLUP`) hält den
> Pin im Ruhezustand auf HIGH, der Tastendruck erzeugt die fallende Flanke.
>
> **Beobachtung:** Jeder Tastendruck schaltet das Blinken um. Über die Variable `taskAktiv`
> kommunizieren ISR und Task miteinander – sie ist deshalb `volatile` deklariert.

### Beispiel 2 – Kommunikation über eine Queue

Ein Producer-Task erzeugt im 200-ms-Takt einen Zählerwert und schickt ihn über eine Queue
an einen Consumer-Task, der ihn ausgibt.

```cpp
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t zahlenQueue;

void TaskProducer(void *pvParameters);
void TaskConsumer(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Queue für 5 Elemente vom Typ uint16_t
  zahlenQueue = xQueueCreate(5, sizeof(uint16_t));

  if (zahlenQueue != NULL) {
    xTaskCreate(TaskProducer, "Producer", 128, NULL, 1, NULL);
    xTaskCreate(TaskConsumer, "Consumer", 128, NULL, 1, NULL);
  } else {
    Serial.println(F("Queue konnte nicht angelegt werden!"));
  }
}

void loop() { }

void TaskProducer(void *pvParameters) {
  (void) pvParameters;
  uint16_t zaehler = 0;
  for (;;) {
    zaehler++;
    xQueueSend(zahlenQueue, &zaehler, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void TaskConsumer(void *pvParameters) {
  (void) pvParameters;
  uint16_t empfangen;
  for (;;) {
    // blockiert, bis ein Wert vorliegt
    if (xQueueReceive(zahlenQueue, &empfangen, portMAX_DELAY) == pdPASS) {
      Serial.print(F("Empfangen: "));
      Serial.println(empfangen);
    }
  }
}
```

> Verändern Sie versuchsweise die Sendefrequenz oder die Queue-Länge und beobachten Sie,
> ab wann `xQueueSend` blockiert.

## Aufgaben

- [ ] **Aufwärmen:** Legen Sie nach obiger Anleitung ein PlatformIO-Projekt für den Arduino Uno an, übersetzen Sie *Beispiel 1* und laden Sie es auf das Board. Schließen Sie einen Taster zwischen Pin D2 und GND an und prüfen Sie, dass jeder Tastendruck das Blinken der LED an- und ausschaltet.
- [ ] **ISR-Synchronisation:** Erklären Sie, warum in der ISR `xTaskResumeFromISR` statt `vTaskResume` verwendet werden muss und weshalb das Suspendieren im Task selbst erfolgt. Ergänzen Sie eine einfache Entprellung (Software-Debounce) für den Taster und beobachten Sie das Verhalten ohne und mit Entprellung.
- [ ] **Idle-Task:** Implementieren Sie eine RTOS-Anwendung, die eine Idle-Funktion nutzt. Recherchieren Sie dazu die Funktionsweise des Idle Tasks in FreeRTOS (Hook `vApplicationIdleHook`, dazu in der `FreeRTOSConfig.h` `configUSE_IDLE_HOOK = 1` setzen). Der Idle Task soll eine LED blinken lassen, um zu zeigen, dass das System im Leerlauf ist.
- [ ] **Queue-Kommunikation:** Erweitern Sie *Beispiel 2*: Task 1 erzeugt alle 200 ms eine Zahl (z.B. einen Zähler) und sendet sie über eine FreeRTOS-Queue an Task 2, die die Zahl empfängt und über den Serial Monitor ausgibt. Experimentieren Sie mit Queue-Länge und Sendetakt und beschreiben Sie, wann `xQueueSend` blockiert.
