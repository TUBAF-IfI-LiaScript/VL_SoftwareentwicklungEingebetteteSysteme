<!--
author:   Sebastian Zug, Karl Fessel & Andrè Dietrich
email:    sebastian.zug@informatik.tu-freiberg.de

version:  1.0.2
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md
         https://raw.githubusercontent.com/liaScript/mermaid_template/master/README.md

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->


[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/lectures/09_FreeRTOS.md#1)


# FreeRTOS in der Praxis


| Parameter                | Kursinformationen                                                                                                                                                                                    |
| ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Veranstaltung:**       | `Vorlesung Softwareentwicklung für eingebettete Systeme`                                                                                                                                                           |
| **Semester**             | `Sommersemester 2026`                                                                                                                                                                                              |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                                    |
| **Inhalte:**             | `FreeRTOS: Tasks, Semaphoren, Mutexe, Queues, praktische Beispiele`                                                                                                                                             |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/09_FreeRTOS.md](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/lectures/09_FreeRTOS.md) |
| **Autoren**              | @author                                                                                                                                                                                              |

![](https://media.giphy.com/media/26gR2qGRnxxXAvhBu/giphy.gif)

---

## Wozu überhaupt ein RTOS?

> **Die berechtigte Frage:** "Eine LED blinken lassen kann ich mit `_delay_ms()`
> in drei Zeilen. Warum soll ich mir einen Scheduler, Tasks und Prioritäten
> antun?" — Diese Skepsis ist gesund. Ein RTOS rechtfertigt sich **nicht** am
> einfachen Beispiel, sondern erst dort, wo die Super-Loop *nachweisbar*
> zusammenbricht. Genau diesen Punkt erarbeiten wir hier Schritt für Schritt.

Wir betrachten eine Aufgabe, die zwei **unabhängige Zeitbasen** verlangt — eine
Situation, die in eingebetteten Systemen die Regel und nicht die Ausnahme ist:

- **(A) Ein Regler** (z. B. ein PI-Regler für Motordrehzahl) muss *exakt* alle
  **10 ms** einen neuen Stellwert berechnen. Die Abtastzeit `T = 10 ms` ist Teil
  der Reglerauslegung — weicht sie ab, ändert sich effektiv die Reglerverstärkung,
  das System schwingt oder wird träge. Das ist eine **harte** Echtzeitanforderung.
- **(B) Eine Display-Ausgabe** soll alle **250 ms** den aktuellen Zustand auf
  einem grafischen OLED (SSD1306, 128×64) über I²C darstellen. Zeitlich völlig
  unkritisch — *aber* das Beschreiben des Displays belegt den Bus (und damit die
  CPU) **für mehrere Millisekunden**.

**Wie lange dauert ein Display-Update?** Der Framebuffer eines 128×64-OLED ist
128·64 = 8192 Bit = **1024 Byte**. Jedes Byte braucht auf dem I²C-Bus 8 Datenbits
plus 1 ACK-Bit, also 9 Bit-Zeiten. Ein voller Frame:

$$ t_{\text{Frame}} = \frac{1024 \text{ Byte} \cdot 9 \text{ Bit}}{f_{\text{SCL}}}
   = \frac{9216 \text{ Bit}}{400\,000\,\text{Bit/s}} \approx 23 \text{ ms} $$

bei 400 kHz (Fast Mode). Bei 100 kHz wären es sogar ~92 ms. Diese ~23 ms blockieren
auf dem Arduino die CPU — und das ist der Knackpunkt:

> **Achtung — warum I²C hier blockiert, UART aber nicht:** Beide nutzen eine
> Hardware-Peripherie (USART bzw. TWI) für das Bit-Timing. Der Unterschied liegt
> in der *Bibliothek*: `HardwareSerial` puffert und sendet per Interrupt im
> Hintergrund — `Serial.print()` kehrt sofort zurück (vgl. die Frage *"Warum gerät
> die UART-Übertragung beim Kontextwechsel nicht durcheinander?"*). Die
> Arduino-`Wire`-Bibliothek dagegen ist auf dem AVR **blockierend**:
> `Wire.endTransmission()` pollt die TWI-Statusbits in einer Schleife und kehrt
> erst zurück, wenn die *gesamte* Übertragung durch ist. Die CPU steckt also die
> vollen ~23 ms in dieser Warteschleife fest. Genau diese Art von Last — eine
> blockierende Bibliothek, die nicht an eine ISR delegiert — macht das
> RTOS-Argument zwingend.
>
> Quellen: Arduino-`Wire`/TWI-Implementierung
> [Wire.cpp](https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/Wire/src/Wire.cpp)
> · [twi.c](https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/Wire/src/utility/twi.c)
> — die Übertragung läuft per `ISR(TWI_vect)`, aber `twi_writeTo()` busy-wartet
> mit `while(wait && (TWI_MTX == twi_state)){ … }` auf deren Abschluss; UART zum
> Vergleich
> [HardwareSerial.cpp](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp).

Beide Perioden (10 ms / 250 ms) sind nicht das Problem — das Problem ist, dass
die unkritische Aufgabe (B) **die CPU länger belegt als die Periode** der
kritischen Aufgabe (A): ~23 ms blockierender I²C-Transfer gegen einen 10-ms-Takt.

### Stufe 1 — Die naive Super-Loop

Der erste Reflex: beides geradeaus in `loop()` mit `delay()`.

```cpp
void loop() {
  static uint8_t diag = 0;
  reglerSchritt();                 // (A) alle 10 ms

  if (++diag >= 25) {              // (B) jeden 25. Durchlauf ~ 250 ms
    diag = 0;
    displaySchreiben(stellwert);   // ~23 ms blockierend (I²C-Frame)
  }
  delay(10);                       // Annahme: Schleife kostet 0 ms
}
```

**Was schiefgeht:**

- `displaySchreiben()` belegt die CPU ~23 ms. In dieser Zeit läuft *kein*
  Reglerschritt — in **jedem** Display-Zyklus fallen rund 2 Abtastschritte aus.
- `delay(10)` ignoriert die Laufzeit der Schleife selbst. Der Reglertakt ist
  systematisch *langsamer* als 10 ms und driftet.


### Stufe 2 — Selbstgebaute Zeitbasis mit `millis()`

"Dann ersetze ich `delay()` durch einen Zeitvergleich" — das klassische
kooperative Zeitscheiben-Muster (Bare-Metal-Scheduler):

```cpp
void loop() {
  uint32_t jetzt = millis();
  if (jetzt >= naechsterRegler) { naechsterRegler += 10;  reglerSchritt(); }
  if (jetzt >= naechstesDisplay) { naechstesDisplay += 250; displaySchreiben(stellwert); }
}
```

Das ist deutlich besser — und für viele Probleme **völlig ausreichend**. Aber
der Kern bleibt ungelöst:

- `displaySchreiben()` belegt die CPU weiterhin ~23 ms. Während dieser Zeit wird
  der obere `if`-Zweig *nicht erreicht* → der Reglertakt reißt erneut auf, nur
  seltener und schwerer zu diagnostizieren.
- Es gibt **keine Priorität**. Kooperatives Multitasking heißt: jede Aufgabe
  muss *freiwillig* schnell zurückkehren. Sobald **eine** Aufgabe blockiert oder
  lange rechnet, leiden **alle** anderen. Die zeitkritische Regelung kann die
  unkritische Display-Ausgabe nicht unterbrechen.

> **Hier endet, was man sauber von Hand bekommt.** Was jetzt fehlt, ist
> **Präemption**: die Fähigkeit, eine laufende, unwichtige Aufgabe *gegen ihren
> Willen* zu unterbrechen, sobald etwas Wichtigeres fällig ist. Diese Mechanik
> selbst zu bauen (Timer-ISR, Kontextsicherung, Stack-Umschaltung) ist genau das,
> was ein RTOS bereits fertig mitbringt.

### Stufe 3 — Die RTOS-Lösung

Jede Aufgabe wird ein eigener Task mit eigener **Priorität**. Der Regler-Task
bekommt die höhere Priorität:

```cpp
static void TaskRegler(void *pv) {            // Priorität 2 (hoch)
  TickType_t last = xTaskGetTickCount();
  for (;;) {
    reglerSchritt();
    vTaskDelayUntil(&last, pdMS_TO_TICKS(10)); // exakt 10 ms, driftfrei
  }
}

static void TaskDisplay(void *pv) {           // Priorität 1 (niedrig)
  for (;;) {
    displaySchreiben(stellwert);               // ~23 ms blockierend ...
    vTaskDelay(pdMS_TO_TICKS(250));            // ... wird aber präemptiert
  }
}
```

**Warum es jetzt funktioniert:**

1. **Präemption** — Wird der 10-ms-Tick des Regler-Tasks fällig, entzieht der
   Scheduler dem Display-Task die CPU *sofort*, mitten im I²C-Transfer. Der
   Reglertakt bleibt stabil, obwohl die Display-Ausgabe weiterhin ~23 ms läuft.
   (Der FreeRTOS-Tick selbst ist ein Interrupt und feuert auch während des
   `Wire`-Busy-Waits — `twi_writeTo()` sperrt die Interrupts nicht. Die laufende
   I²C-Übertragung wird dabei nicht beschädigt: Die TWI-Hardware taktet das
   aktuelle Byte autonom fertig; *nur* die CPU wechselt den Task.)
2. **Driftfreie Zeitbasis** — `vTaskDelayUntil()` rechnet relativ zum *letzten*
   Weckzeitpunkt; Laufzeit-Jitter akkumuliert sich nicht.
3. **Trennung von Funktionalität und Timing** — keine `millis()`-Buchhaltung
   mehr. Jeder Task beschreibt nur *seine* Aufgabe und *seine* Periode.

Am Oszilloskop (Toggle-Pin bei jedem Reglerschritt) wird der Unterschied direkt
sichtbar:

```ascii
Stufe 1 / Stufe 2:                         Stufe 3 (FreeRTOS):
  __    __    __      __    __               __    __    __    __    __
 |  |  |  |  |  |    |  |  |  |              |  |  |  |  |  |  |  |  |  |
_|  |__|  |__|  |____|  |__|  |__           _|  |__|  |__|  |__|  |__|  |__
                ^^^^                         gleichmäßig — kein Einbruch
                Display blockiert ~23 ms
                = ~2 Reglertakte verloren
```

Das vollständige, lauffähige Beispiel mit allen drei Stufen — inklusive
Messpunkt und Build-Anleitung — liegt im Repository:

[codeExamples/avr/superLoopProblem](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/tree/main/codeExamples/avr/superLoopProblem)


### Wann lohnt sich ein RTOS — und wann nicht?

Der Eskalationspfad zeigt: Ein RTOS ist **kein Selbstzweck**. Die Entscheidung
hängt davon ab, ob die Super-Loop an ihre Grenze stößt:

| Situation                                                             | Super-Loop reicht | RTOS sinnvoll |
| --------------------------------------------------------------------- | :---------------: | :-----------: |
| Eine einzige periodische Aufgabe (LED blinken)                        |         ✓         |       –       |
| Mehrere Aufgaben, alle kurz und nicht-blockierend                     |         ✓         |      (✓)      |
| Mehrere Zeitbasen, eine Aufgabe blockiert/rechnet lange               |         ✗         |       ✓       |
| Harte Echtzeit-Deadline trifft auf unkritische Hintergrundlast        |         ✗         |       ✓       |
| Mischung aus Hard- und Soft-Realtime-Komponenten                      |         ✗         |       ✓       |
| Code soll über Hardwarewechsel hinweg wiederverwendbar bleiben        |        (✗)        |       ✓       |

Daraus ergeben sich die konkreten Vorteile des RTOS gegenüber dem Super-Loop-Design
— jetzt *am Beispiel festgemacht* statt nur behauptet:

- **Trennung von Funktionalität und Timing** — der Task beschreibt *was* zu tun
  ist, der Scheduler *wann*. Kein manuelles `millis()`-Bookkeeping (Stufe 2 → 3).
- **Explizite Prioritäten** — die wichtige Aufgabe verdrängt die unwichtige
  automatisch (das, was Stufe 2 grundsätzlich nicht kann).
- **Mischung von Hard- und Soft-Realtime** in *einem* System ohne dass die
  unkritische Last die kritische gefährdet.
- **Wiederverwendbarkeit** — die Task-Logik ist von der Timing-Mechanik
  entkoppelt und übersteht einen Hardwarewechsel.

> **Merke:** Der Preis dafür ist realer Overhead (Speicher pro Task-Stack,
> Kontextwechselzeit, Komplexität). Auf einem ATmega328 mit 2 KB RAM sind das
> spürbare Grenzen — siehe Abschnitt *FreeRTOS auf verschiedenen Architekturen*.
> Ein RTOS ist ein Werkzeug, kein Pflichtprogramm.

### Der Haken: Was darf *nicht* unterbrochen werden?

Bis hier klang die Geschichte zu glatt: "Der Regler-Task verdrängt die
Display-Ausgabe — Problem gelöst." Aber sobald **echte** Kommunikation im Spiel
ist, kippt das. Wird ein Task *mitten in einer Bus-Übertragung* unterbrochen,
kann die Übertragung beschädigt werden. Wir scheinen ein neues Problem gegen das
alte eingetauscht zu haben:

```ascii
 Atomarität  <───────────  Zielkonflikt  ───────────>  Reaktivität
 (manches darf NICHT          (kritischer               (Regler MUSS
  unterbrochen werden)         Abschnitt)                sofort dran)
```

Jeder Schutz vor Unterbrechung (Interrupts sperren) erkauft **Konsistenz** mit
**Latenz** — und Latenz ist genau das, was die Echtzeitfähigkeit auffrisst. Die
Auflösung ist **quantitativ**: Es geht nicht um *ob*, sondern um *wie lange*.

| Größe                                       | Wert            |
| ------------------------------------------- | --------------- |
| Regler-Periode                              | 10 000 µs       |
| tolerierbarer Jitter (~0,5 %)               | ~50 µs          |
| kritischer Abschnitt für *eine* Mikro-Sequenz | ~2 µs         |
| *ganze* Display-Aktualisierung              | ~23 000 µs      |

Sperrt man die **ganzen 23 ms** → Regler verhungert. Sperrt man nur die **2-µs-
Mikro-Sequenz** (z. B. die register-kritischen Instruktionen) → Jitter 2 µs ≪
50 µs → Echtzeit bleibt intakt. Beide Anforderungen sind gleichzeitig erfüllbar,
**weil die unteilbare Einheit winzig ist und die präemptierbaren Pausen dazwischen
groß** (bei I²C: das einzelne Byte taktet ohnehin die TWI-Hardware autonom):

```ascii
FALSCH (atomar = ganze Funktion):
  [############ 23 ms gesperrt ############]   Regler verhungert

RICHTIG (atomar = nur die Mikro-Sequenz):
  [#2µs#]...Bus...[#2µs#]...Bus...[#2µs#]      Regler schlägt in den Lücken zu
   sperr   frei    sperr   frei
```

### Wo muss ich Atomarität *wirklich* einbauen?

Atomarität ist teuer, also baut man sie **nur dort ein, wo eine Unterbrechung
nachweisbar Schaden anrichtet**. Das passiert aus genau zwei Gründen:

1. **Geteilter, veränderlicher Zustand (Race):** Zwei Stränge fassen dieselbe
   Ressource an, mindestens einer verändert sie — nicht-atomares `digitalWrite`
   auf einen gemeinsamen Port, eine 16-Bit-Variable auf dem 8-Bit-AVR, ein von
   zwei Tasks geteilter I²C-Bus.
2. **Zeitlich gekoppelte Sequenz:** Die Pulsbreite oder exakte Reihenfolge *ist*
   die Information (WS2812, Software-SPI, 1-Wire).

**Trifft keiner der beiden zu — und das ist der Normalfall — bleibt der Code
präemptierbar.**

```ascii
Schritt 1 — KANN eine Unterbrechung hier Schaden anrichten?
  Schaden  ⟺  geteilter veränderlicher Zustand  ODER  Pulsbreite/Reihenfolge = Info
  │
  ├─ keiner von beiden?  ──────────────►  KEINE Atomarität  (häufigster Fall)
  │
  └─ mindestens einer trifft zu          →  schützen ist PFLICHT, weiter zu Schritt 2

Schritt 2 — WOMIT schützen? (schwächstes ausreichendes Werkzeug)
  │
  ├─ Pulsbreite/Reihenfolge ist Information   →  critical section
  │  (z. B. WS2812, Bitbang — gilt AUCH ohne     (Interrupts aus, nur µs!)
  │   geteilten Zustand)
  │
  └─ geteilter veränderlicher Zustand  →  Wer ist der andere ?
       ├─ andere TASKS            →  Mutex      (ISRs laufen weiter!)
       └─ eine ISR / HW-Register  →  critical section                                                       .
```

Angewendet auf unser I²C-Display-Beispiel (und zwei Vergleichsfälle mit anderer
Anbindung):

| Stelle                                        | geteilt? | pulsbreiten-kodiert? | Atomarität?             |
| --------------------------------------------- | :------: | :------------------: | ----------------------- |
| Der ~23-ms-Frame-Transfer als Ganzes          |   nein   |         nein         | **nein** (nur Last!)    |
| I²C-Bus, **nur der Display-Task** nutzt ihn   |   nein   |         nein         | **nein**                |
| I²C-Bus, **von zwei Tasks** geteilt           |  **ja**  |         nein         | **ja — Mutex**          |
| Zugriff auf `stellwert` (16 Bit, 8-Bit-CPU)   |  **ja**  |         nein         | **ja** — `ATOMIC_BLOCK` |
| *(Vergleich)* GPIO-Display-Pin am Regler-Port |  **ja**  |         nein         | **ja** — critical, µs   |
| *(Vergleich)* WS2812-LED-Strip (Bitbang)      |   nein   |        **ja**        | **ja** — critical       |

> **Die Kernregel:** Atomarität gehört *nur* dorthin, wo eine Unterbrechung
> **geteilten veränderlichen Zustand** zerreißt oder eine **pulsbreiten-kodierte
> Sequenz** zerstört. Dort nimmt man das **schwächste** ausreichende Werkzeug —
> ein **Mutex** sperrt keine Interrupts und lässt den Regler weiterlaufen, eine
> **critical section** sperrt Interrupts und bleibt deshalb so *kurz* wie
> physikalisch möglich. Solange `max(kritischer Abschnitt) ≪ Deadline-Jitter`,
> ist die Echtzeitfähigkeit gewahrt. Das ist mehr Kontrolle als die Super-Loop
> bietet, nicht weniger: Dort ist die *ganze* lange Funktion faktisch atomar.

*****************************************************************************

## Verwendung von Echtzeitbetriebssystemen

Ein Echtzeitbetriebssystem (RTOS) ist ein Betriebssystem (OS), das für Echtzeitanwendungen vorgesehen ist.  Ereignisgesteuerte Systeme schalten zwischen Tasks auf der Grundlage ihrer Prioritäten um, während Time-Sharing-Systeme die Tasks auf der Grundlage von Taktinterrupts umschalten. Die meisten RTOSs verwenden einen präemptiven Scheduling-Algorithmus.

Die konkrete Motivation — *wann* sich der Wechsel vom Super-Loop-Design lohnt
und *woran* es ohne RTOS scheitert — haben wir im vorangehenden Abschnitt
*Wozu überhaupt ein RTOS?* am Beispiel zweier Zeitbasen erarbeitet. Zusammengefasst:

- Trennung von Funktionalität und Timing - RTOS entlastet den Nutzer und behandelt Timing, Signale und Kommunikation
- explizite Definition von Prioritäten - deutliche bessere Skalierbarkeit als im SLD
- Mischung von Hard- und Softrealtime Komponenten
- Verbesserte Wiederverwendbarkeit des Codes insbesondere bei Hardwarewechseln

![alt-text](../images/08_Algorithms/RTOS_Vergleich.png "Einordnung von RTOS Umsetzungen - Motivierte nach Präsentation von Richard Berry (Gründer FreeRTOS)")

> FreeRTOS ist kein vollständiges Betriebssystem, sondern ein hochoptimierter, portabler Echtzeit-Kernel – ideal für bare-metal Systeme, bei denen Timing und Ressourcenverbrauch kritisch sind.

FreeRTOS ist für leistungsschwache eingebettete Systeme konzipiert. Der Kernel selbst besteht aus nur drei C-Dateien. Um den Code lesbar, einfach zu portieren und wartbar zu machen, ist er größtenteils in C geschrieben, aber es sind einige Assembler-Funktionen (meist in architekturspezifischen Scheduler-Routinen) enthalten. FreeRTOS kann eher als "Thread-Bibliothek" denn als "Betriebssystem" betrachtet werden, obwohl eine Kommandozeilenschnittstelle und POSIX-ähnliche E/A-Abstraktionserweiterungen verfügbar sind.

### FreeRTOS Grundlagen

Das Task Modell unterscheidet 4 Zustände: Running, Blocked, Suspended, Ready, die mit entsprechenden API Funktionen manipuliert werden können, bzw. durch den Scheduler gesetzt werden. FreeRTOS implementiert dafür einen präemptiven und einen kooperativen Multitasking Mode.

<!--
style="width: 80%;"
-->
```ascii
                        präemptiv                              kooperativ
Höchste         ^                                        ^                                 
Priorität   T1  |          XXXXXXXXXX                T1  |                    XXXXXXXXXX            
            T2  |XXXXXXXXXX         XXXXXXXXXX       T2  |XXXXXXXXXXXXXXXXXXXX     
                +----|----|----|----|----|----|->        +----|----|----|----|----|----|->
                0    2    4    6    8   10   12          0    2    4    6    8   10   12   
```

FreeRTOS implementiert mehrere Threads, indem es das Host-Programm in regelmäßigen kurzen Abständen eine Thread-Tick-Methode aufrufen lässt. Die Thread-Tick-Methode schaltet Tasks abhängig von der Priorität und einem Round-Robin-Schema um. Das übliche Intervall beträgt 1 bis 10 Millisekunden (1/1000 bis 1/100 einer Sekunde), über einen Interrupt von einem Hardware-Timer, aber dieses Intervall wird oft geändert, um einer bestimmten Anwendung zu entsprechen.

Der FreeRTOS-Echtzeit-Kernel misst die Zeit mit einer Tick-Count-Variable. Ein Timer-Interrupt (der RTOS-Tick-Interrupt) inkrementiert den Tick-Count - so kann der Echtzeit-Kernel die Zeit mit einer Auflösung der gewählten Timer-Interrupt-Frequenz messen.

Jedes Mal, wenn der Tick-Count inkrementiert wird, muss der Echtzeit-Kernel prüfen, ob es nun an der Zeit ist, einen Task zu entsperren oder aufzuwecken. Es ist möglich, dass ein Task, der während des Tick-ISRs geweckt oder entsperrt wird, eine höhere Priorität hat als der unterbrochene Task. Wenn dies der Fall ist, sollte der Tick-ISR zum neu geweckten/entblockten Task zurückkehren - effektiv unterbricht er einen Task, kehrt aber zu einem anderen zurück.

> **Merke:** Das präemptive Scheduling erfordert sowohl auf der Planungsebene, als auch auf der Verwaltungsebene einen größeren Overhead.

### Speicherverwaltung

FreeRTOS unterstützt 2 Strategien für die Speicherbereitstellung, eine statische und eine dynamische Allokation. Während die erste Variante es der Anwendung überlässt die notwendigen Speicherareale zu akquirieren, übernimmt das RTOS diese Aufgabe im zweiten Modus. Der angeforderte Speicher wird auf dem Heap abgelegt. Mit dem Aufruf von `xTaskCreate()` wird ein Speicherblock alloziert, so dass der Stack und der _task control block_ (TCB) dort abgelegt werden können. Queues, Mutexe und Semaphoren werden ebenfalls dort eingebunden.  

<!--
style="width: 80%;"
-->
```ascii
                  Arbeitsspeicher AVR (SRAM)

hohe Adresse   +-------------------+  RAMEND (z. B. 0x08FF, ATmega328)
(RAMEND)       | Stack für main    |
               | und IRQs          |
               |        |          |
               |        v  wächst nach unten
               +-------------------+
               | freier Speicher   |   <- hier kollidieren Stack & Heap
               |                   |      im Ernstfall (Stack Overflow!)
               +-------------------+
               |        ^  wächst nach oben
               | FreeRTOS-Heap     |      +-----------------+
               | (ucHeap[])        |  ->  | TCB Task A      |
               |                   |      +-----------------+
               |                   |      | Stack Task A    |
               |                   |      +-----------------+
               |                   |      | Queue X / Mutex |
               +-------------------+      +-----------------+
               | .data / .bss      |
               | (globale Var.)    |
niedrige Adr.  +-------------------+  RAMSTART (0x0100)
(RAMSTART)
```

Zwei wichtige Punkte zur Orientierung:

+ **Stack und Heap wachsen aufeinander zu.** Der Stack beginnt bei `RAMEND` (hohe
  Adresse) und wächst nach *unten*; der Heap wächst nach *oben*. Treffen sie sich
  im "freien Speicher", ist der RAM erschöpft — auf dem ATmega328 mit nur 2 KB ein
  realer Engpass.
+ **TCB und Stack eines Tasks sind getrennte Strukturen**, liegen aber benachbart
  im FreeRTOS-Heap. Der TCB *enthält* den Stack nicht, sondern *zeigt* über
  `pxTopOfStack` auf dessen aktuelle Spitze (siehe unten).

Beim Kontextwechsel sind auf dem AVR folgende Elemente auf dem Taskzugehörigen Stack zu speichern:

+ 32 general purpose processor Register
+ Status register
+ Program counter
+ 2 stack pointer registers.

```asm
#define portSAVE_CONTEXT()           
asm volatile (	                     
  "push  r0                    nt"
  ; Das Prozessorregister R0 wird zuerst gespeichert, da es beim Speichern des
  ; Statusregisters verwendet wird und mit seinem ursprünglichen Wert gespeichert
  ; werden muss.
  "in    r0, __SREG__          nt"
  "cli                         nt"
  "push  r0                    nt"
  ; Das Statusregister wird in R0 verschoben, damit es auf dem Stack gespeichert
  ; werden kann.
  "push  r1                    nt"
  "clr   r1                    nt"
  ; Der vom Compiler aus dem ISR-C-Quellcode generierte Code geht davon aus, dass
  ; R1 auf Null gesetzt ist. Der ursprüngliche Wert von R1 wird gespeichert,
  ; bevor R1 gelöscht wird.
  "push  r2                    nt"
  ...
  "push  r31                   nt"
  ; Sichern aller verbliebenen Register auf dem Stack
  "lds   r26, pxCurrentTCB     nt"
  "lds   r27, pxCurrentTCB + 1 nt"
  "in    r0, __SP_L__          nt"
  "st    x+, r0                nt"
  "in    r0, __SP_H__          nt"
  "st    x+, r0                nt"
  ; Sichern des Stackpointers auf dem Stack
);
```

Die TCB enthält unter anderem:

+ die Informationen zur Speicherverwaltung - Adresse der Stack-Startadresse in `pxStack` und den aktuellen Stackanfang in `pxTopOfStack`. FreeRTOS speichert auch einen Zeiger auf das Ende des Stacks in `pxEndOfStack`.
+ die anfängliche Priorität und die aktuelle Priorität des Tasks in `uxBasePriority` und `uxPriority`
+ den Namen des Tasks

Man könnte erwarten, dass jeder Task eine Variable hat, die FreeRTOS mitteilt, in welchem Zustand er sich befindet, aber das tut sie nicht. Stattdessen verfolgt FreeRTOS den Zustand des Tasks implizit, indem es den Task in die entsprechende Liste einträgt: ready list, suspended list, etc. Das Vorhandensein einer Aufgabe in einer bestimmten Liste zeigt den Zustand der Aufgabe an. Wenn ein Task von einem Zustand in einen anderen wechselt, verschiebt FreeRTOS ihn einfach von einer Liste in eine andere.

```c
typedef struct tskTaskControlBlock
{
  volatile portSTACK_TYPE *pxTopOfStack;                  /* Points to the location of
                                                             the last item placed on
                                                             the tasks stack.  THIS
                                                             MUST BE THE FIRST MEMBER
                                                             OF THE STRUCT. */
  unsigned portBASE_TYPE uxPriority;                      /* The priority of the task
                                                             where 0 is the lowest
                                                             priority. */
  portSTACK_TYPE *pxStack;                                /* Points to the start of
                                                             the stack. */
  signed char pcTaskName[ configMAX_TASK_NAME_LEN ];      /* Descriptive name given
                                                             to the task when created.
                                                             Facilitates debugging
                                                             only. */
  // ...

} tskTCB;
```

> **Warum muss `pxTopOfStack` das *erste* Member sein?** Beim Kontextwechsel lädt
> der Assembler-Code (siehe `portSAVE_CONTEXT` oben) den Zeiger `pxCurrentTCB` und
> muss *sofort* an die gespeicherte Stackspitze gelangen. Weil `pxTopOfStack` an
> Offset 0 liegt, ist die TCB-Adresse zugleich die Adresse von `pxTopOfStack` —
> der Zugriff geschieht *ohne* Offset-Berechnung. 

Zusammenfassung der FreeRTOS Komponenten:

| Komponente               | Aufgabe                                 |
| ------------------------ | --------------------------------------- |
| TCB                      | Speichert Task-Informationen            |
| Task-Listen              | Organisieren Tasks nach Status          |
| Scheduler                | Wählt aus, welcher Task ausgeführt wird |
| Kontextwechsel           | Speichert und lädt Task-Kontext         |
| Synchronisations-Objekte | Koordination und Kommunikation          |


### Implementierung Grundlagen

Die generellen Parameter einer FreeRTOS-Anwendung finden sich in der Datei [FreeRTOSConfig.h](https://github.com/Infineon/freertos/blob/master/Source/portable/COMPONENT_CM33/FreeRTOSConfig.h).

| FreeRTOS Parameter         | Bedeutung                                                                                                                             |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `configTOTAL_HEAP_SIZE`    | Größe des maximal verwendeten Gesamtspeichers aller Tasks usw.                                                                        |
| `configMINIMAL_STACK_SIZE` | Angabe der Minimalen Stackgröße                                                                                                       |
| `configMAX_PRIORITIES`     | Definition der Zahl der Prioritäten. Daraus folgt die Zahl der notwendigerweise zu etablierenden Listen für die Verwaltung der Tasks. |
| `configCPU_CLOCK_HZ`       | Taktfrequenz des Prozessors                                                                                                           |
| `configTICK_RATE_HZ`       | Frequenz des Tick-Interrupts in Hz — bestimmt die zeitliche Auflösung von `vTaskDelay()` & Co. (1 Tick = 1/`configTICK_RATE_HZ` s)    |

> **Achtung!** Die verwendete Implementierung für den AVR lässt einzelne Einstellungsmöglichkeiten außer Acht und realisiert diese fest im Code oder implementiert eigene Konfigurationsmethoden.

```c   TaskBasicStructure.c
xTaskCreate( TaskBlink
 ,  "Blink"   // A name just for humans
 ,  128       // This stack size can be checked & adjusted by reading the Stack Highwater
 ,  NULL      //Parameters passed to the task function
 ,  2         // Priority, with 2 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
 ,  &TaskBlink_Handler ); //Task handle


static void TaskBlinkRedLED(void *pvParameters) // Main LED Flash
{
   while(1)
   {
       PINB = ( 1 << PB5 );
       vTaskDelay(1000/portTICK_PERIOD_MS); // wait for one second
   }
}

vTaskStartScheduler();
```

> **Merke:** Die bisherigen Zeitfunktionen `_delay_ms()` und `_delay_us()` werden durch RTOS spezifische Funktionen ersetzt. Damit wird die Kontrolle an den Scheduler zurückgegeben.

Eine Vorgehen, um auch die Laufzeit der eigentlichen Anwendung zu berücksichtigen, ist die Überwachung anhand von `vTaskDelayUntil()`:


```c   TaskBasicStructure.c
// Perform an action every 10 ticks.
void vTaskFunction( void * pvParameters )
{
TickType_t xLastWakeTime;
const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );

        // Perform action here.
    }
}
```

### Schedulingvarianten

```c cooperativeScheduling.c
static void TaskBlinkRedLED(void *pvParameters) // Main LED Flash
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        PORTB |= ( 1 << PB5 );
        vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_PERIOD_MS ) );
        PORTB &= ~( 1 << PB5 );
        vTaskDelayUntil( &xLastWakeTime, ( 400 / portTICK_PERIOD_MS ) );
    }
}
```

Entsprechend muss aber in `FreeRTOSConfig.h` eine Makrovariable angepasst werden `#define configUSE_PREEMPTION 0`.

### Praxis: Wie passt man `FreeRTOSConfig.h` an?

Hier stößt das Lehrbuchmodell ("ändere einfach dein `FreeRTOSConfig.h`") auf die
Arduino-Realität. Mit `lib_deps = feilipu/FreeRTOS` zieht PlatformIO das Paket —
**inklusive einer fertigen `FreeRTOSConfig.h`** — nach `.pio/libdeps/uno/FreeRTOS/`.
Dieser Ordner ist ein **Build-Artefakt**: Er wird nicht eingecheckt und bei jedem
`pio pkg update` oder `rm -rf .pio` neu erzeugt. Editiert man die Datei *dort*,
ist die Änderung beim nächsten Build oder auf dem Rechner der Kollegin wieder weg.

Drei saubere Wege, die Konfiguration im **Projekt** zu verankern:

__1. Einzelne Makros per Build-Flag (`platformio.ini`)__

Für einzelne Schalter genügt ein `-D` in der `platformio.ini` — die Datei muss
gar nicht angefasst werden:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = feilipu/FreeRTOS
build_flags =
    -D configUSE_PREEMPTION=0      ; kooperatives Scheduling erzwingen
    -D INCLUDE_vTaskDelayUntil=1
```

> **Achtung — der Haken:** Das funktioniert *nur*, wenn das Makro in der
> `FreeRTOSConfig.h` mit `#ifndef` geschützt ist (`#ifndef X` / `#define X …` /
> `#endif`). Die `feilipu`-Datei definiert `configUSE_PREEMPTION` aber **hart**
> (ohne `#ifndef`) — ein `-D configUSE_PREEMPTION=0` erzeugt dann nur eine
> "macro redefined"-Warnung, und der Header gewinnt. Für die `INCLUDE_*`-Flags
> klappt der `-D`-Weg, für `configUSE_PREEMPTION` *nicht* zuverlässig.

__2. Eigene `FreeRTOSConfig.h` im Projekt (empfohlen)__

Robust ist eine **projekteigene** Kopie, die der Compiler *vor* der Lib findet:

```text
projekt/
├── platformio.ini
├── include/
│   └── FreeRTOSConfig.h     ← eigene Kopie, hier angepasst (im Git!)
└── src/
    └── main.cpp
```

```ini
build_flags = -I include      ; den eigenen include-Ordner VOR die Lib stellen
```

Man kopiert die Original-`FreeRTOSConfig.h` einmal nach `include/`, ändert dort
`configUSE_PREEMPTION` und **diese** Version wird kompiliert. Sie liegt im
Git-Repo, übersteht `rm -rf .pio` und ist auf jedem Rechner identisch.

__3. Verschiedene Configs pro Environment__

Für *denselben* Code mit unterschiedlicher Konfiguration (z. B. kooperativ vs.
präemptiv) bekommt jedes Environment seinen eigenen Config-Ordner:

```ini
[env:coop]
build_flags = -I config_coop      ; FreeRTOSConfig.h mit configUSE_PREEMPTION=0

[env:preempt]
build_flags = -I config_preempt   ; FreeRTOSConfig.h mit configUSE_PREEMPTION=1
```

> **Merke:** Konfiguration gehört ins **Projekt**, nie in die heruntergeladene
> Lib. Das deckt sich mit dem Hinweis oben, dass der AVR-Port einzelne
> Einstellungen fest im Code realisiert — gerade deshalb muss man genau wissen,
> *welcher* Schalter per `-D` greift und welcher eine eigene Config erzwingt.


```c preemtiveScheduling.c
static void WorkerTask(void *pvParameters)
{
  static uint32_t idelay;
  static uint32_t Delay;
  Delay = 10000000;
  /* Worker task Loop. */
  for(;;)
  {
    // Simulating some work here
	  for (idelay = 0; idelay < Delay; ++idelay);
  }
  /* Should never go there */
  vTaskDelete(worker_id);
}
```

Das ist die Standardimplementierung, eine Rekonfiguration ist nicht notwendig.



### Kommunikation / Synchronisation zwischen Tasks

         {{0-1}}
*****************************************************************************

Bisher haben wir allein über isolierte Tasks gesprochen, die ohne Relation zu einander bestehen. FreeRTOS bietet fünf primäre Mechanismen für die Kommunikation zwischen Tasks: _queues_, _semaphores_, _mutexes_, _stream buffers_ und _message buffers_. Allen gemeinsam ist, dass sie dazu führen können, dass Tasks blockieren, wenn die Ressource oder die Daten eines anderen Tasks nicht verfügbar sind.

Beispiele:

+ Ein Semaphore sperrt bzw. gibt den Zugriff auf ein Display frei.
+ Ein Task wartet auf das Eintreffen des Ergebnisses eines anderen Tasks, das über eine Queue übermittelt wurde.

| Konzept                            | Kategorie                       | Zweck / Typischer Anwendungsfall                                                 |
| ---------------------------------- | ------------------------------- | -------------------------------------------------------------------------------- |
| **Queue**                          | Kommunikation                   | Daten zwischen Tasks übertragen (z. B. Producer–Consumer)                        |
| **Semaphore**                      | Synchronisation                 | Ereignissignalisierung oder Ressourcenzugriff koordinieren (z. B. ISR → Task)    |
| **Mutex**                          | Schutz                          | Kritische Abschnitte schützen (z. B. Zugriff auf globale Variablen)              |
| **Counting Semaphore**             | Synchronisation                 | Zugriff auf begrenzte Anzahl an Ressourcen (z. B. Pool mit 3 Geräten)            |
| **Binary Semaphore**               | Synchronisation                 | Ereignissignal (wie ein Flag zwischen ISR ↔ Task oder Task ↔ Task)               |
| **Recursive Mutex**                | Schutz                          | Reentrant-sichere Mutexes (für Funktionen, die sich selbst rekursiv aufrufen)    |
| **Task Notification**              | Synchronisation / Kommunikation | Sehr schnelle 1:1-Kommunikation und Ereignissignalisierung zwischen Tasks        |
| **Event Groups**                   | Synchronisation                 | Bitweise Ereigniskombination (z. B. mehrere Flags zusammen auswerten)            |
| **Stream Buffer / Message Buffer** | Kommunikation                   | Byteweise oder blockweise Datenübertragung, ideal für z. B. UART oder Protokolle |



*****************************************************************************

         {{1-2}}
*****************************************************************************

__Queues__

Queues bieten eine Inter-Task-Kommunikation mit einer vom Benutzer definierbaren festen Länge. Der Entwickler gibt die Nachrichtenlänge bei der Erstellung der Warteschlange an. Dies geschieht durch den Aufruf

```c
QueueHandle_t queueName =xQueueCreate(queueLength, elementSize)
```

Der Eingabeparameter `queueLength` gibt die Anzahl der Elemente an, die die Warteschlange aufnehmen kann. `elementSize` gibt die Größe der einzelnen Elemente in Bytes an. Alle Elemente in der Warteschlange müssen die gleiche Größe haben. Die Warteschlange hat eine FIFO-Struktur (first in/first out), so dass der Empfänger immer das Element erhält, das als erstes eingefügt wurde.

```c
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>  // Für printf (je nach Plattform anpassen)

// Globale Queue-Handle
QueueHandle_t xQueue;

// Sender-Task: Sendet alle 1000 ms einen Wert in die Queue
void vSenderTask(void *pvParameters)
{
    int32_t valueToSend = 0;
    while(1)
    {
        if (xQueueSend(xQueue, &valueToSend, portMAX_DELAY) == pdPASS)
        {
            printf("Gesendet: %ld\n", valueToSend);
            valueToSend++;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1000 ms warten
    }
}

// Empfänger-Task: Empfängt Werte aus der Queue und gibt sie aus
void vReceiverTask(void *pvParameters)
{
    int32_t receivedValue;
    while(1)
    {
        if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdPASS)
        {
            printf("Empfangen: %ld\n", receivedValue);
        }
    }
}

int main(void)
{
    // Queue für 5 Integer-Elemente anlegen
    xQueue = xQueueCreate(5, sizeof(int32_t));
    if (xQueue == NULL)
    {
        // Fehlerbehandlung: Queue konnte nicht erstellt werden
        while(1);
    }

    // Tasks erstellen
    xTaskCreate(vSenderTask, "Sender", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vReceiverTask, "Receiver", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Scheduler starten
    vTaskStartScheduler();

    // Sollte nie erreicht werden
    for(;;);
}
```

Verhalten beim Warten auf eine Queue

+ Task blockiert ...  Der Empfänger-Task wird in den Blocked-Zustand versetzt, solange keine Daten in der Queue sind.
+ Scheduler wählt anderen Task ... Da der Empfänger-Task blockiert ist, kann er keine CPU-Zeit bekommen. Der Scheduler sucht sich einen anderen bereitstehenden Task mit der höchsten Priorität aus.
+ CPU wird anderen Tasks zugewiesen ... Der Sender-Task hat Priorität 2 und der Empfänger Priorität 1. Wenn der Empfänger wartet, läuft der Sender.
+ Task wird wieder bereit ... Sobald der Sender eine Nachricht in die Queue schreibt, wird der Empfänger-Task automatisch wieder aus dem Blocked-Zustand in die Ready-Liste versetzt.
+ Preemptives Scheduling ... Falls der Empfänger eine niedrigere Priorität hat als den Sender, kann er erst wieder laufen, wenn der Sender sich blockiert oder fertig ist. Wenn Prioritäten gleich oder der Empfänger höher ist, erfolgt ein sofortiger Kontextwechsel.


| Szenario                                            | Warum?                             |
| --------------------------------------------------- | ---------------------------------- |
| Ein Task erzeugt Daten, ein anderer verarbeitet sie | Entkopplung, Synchronisation       |
| Interrupt soll Daten an Task weitergeben            | ISR bleibt schnell, Task übernimmt |
| Kommunikation ohne Busy-Waiting                     | Task schläft, bis Daten ankommen   |
| Unterschiedliche Ausführungsgeschwindigkeit         | Queue puffert Zwischenstände       |
| Gemeinsame Datenverwaltung zwischen Tasks           | Thread-safe über Queue             |


*****************************************************************************

          {{2-3}}
*****************************************************************************

__Semaphoren__

Semaphore werden zur Synchronisation und zur Steuerung des Zugriffs auf gemeinsame Ressourcen zwischen Tasks verwendet. Ein Semaphor kann entweder binär oder zählend sein und ist im Wesentlichen nur ein nicht-negativer Integer-Zähler.

Ein binäres Semaphor wird auf 1 initialisiert und kann verwendet werden, um eine Ressource zu bewachen, die nur von einem Task zu einem Zeitpunkt gehandhabt werden kann. Wenn eine Task die Ressource übernimmt, wird der Semaphor auf 0 dekrementiert. Wenn ein anderer Task die Ressource verwenden möchte und sieht, dass der Semaphor 0 ist, blockiert sie. Wenn der erste Task mit der Nutzung der Ressource fertig ist, wird das Semaphor inkrementiert und steht damit anderen Tasks zur Verfügung.

Ein binärer Semaphor kann mit

```
SemaphoreHandle_t semaphoreName = xSemaphoreCreateBinary(void)
```

erstellt werden. Ein zählender Semaphor funktioniert auf die gleiche Weise, jedoch für Ressourcen, die von mehreren Tasks gleichzeitig verwendet werden können. Ein Zählsemaphor sollte auf die Anzahl der Tasks initialisiert werden, die gleichzeitig Zugriff auf die Ressource haben können, und wird mit

```
SemaphoreHandle_t semaphoreName =xSemaphoreCreateCounting(maxCount, initialCount)
```

erstellt. Wenn eine Task eine durch einen Semaphor geschützte Ressource wünscht, ruft sie die Funktion `xSemaphoreTake(semaphoreName,ticksToWait)` auf. Wenn der Semaphor zu 0 ausgewertet wird, blockiert die Task für die in ticksToWait angegebene Zeit. Wenn die Task mit der Verwendung des Semaphors fertig ist, wird die Funktion `xSemaphoreGive(semaphoreName)` aufgerufen.

*****************************************************************************

          {{3-4}}
*****************************************************************************

__Mutex__

Ein Mutex ist einem binären Semaphor sehr ähnlich, bietet aber zusätzlich einen Mechanismus zur Prioritätsvererbung. Wenn ein hochpriorer Task beim Zugriff auf eine Ressource blockiert wird, die bereits von einem niederprioren Task belegt ist, erbt der niederpriore Task die Priorität des hochprioren Tasks, bis er den Mutex freigegeben hat.

Dies stellt sicher, dass die Blockierzeit des hochprioren Tasks minimiert wird, da der niedrigpriore Task nun nicht mehr von anderen Tasks mit mittlerer Priorität präemptiert werden kann.

Eine Mutex wird mit der Funktion

```
semaphoreHandle_t mutexName = xSemaphoreCreateMutex(void)
```

erstellt. Mutexe sollten nicht von einem Interrupt aus verwendet werden, da der Mechanismus der Prioritätsvererbung nur für einen Task sinnvoll ist, nicht aber für einen Interrupt.

*****************************************************************************

          {{4-5}}
*****************************************************************************

__Beispiel__

Im Beispiel wird auf die Ressource Serielle Schnittstelle durch zwei Tasks zugegriffen. Um ein Überschreiben der Inhalte zu verhindern ist eine Synchronisation erforderlich.

> **Warum hier ein Mutex und kein (binäres) Semaphor?** Beide könnten die UART
> technisch schützen — aber die *Semantik* unterscheidet sich, und an dieser
> Stelle passt der Mutex:
>
> 1. **Es ist Besitz, kein Signal.** Beide Tasks wollen die Schnittstelle
>    *exklusiv benutzen*; derselbe Task, der `xSemaphoreTake()` aufruft, gibt sie
>    mit `xSemaphoreGive()` auch wieder frei (take und give im *selben* Task). Das
>    ist das Lehrbuch-Muster für einen Mutex. Ein binäres Semaphor wird dagegen
>    typischerweise von *einem* Strang gegeben und von einem *anderen* genommen —
>    etwa wenn eine ISR einem Task ein Ereignis signalisiert.
> 2. **Prioritätsvererbung.** `TaskA` hat Priorität 2, `TaskB` Priorität 1. Hält
>    der niederpriore `TaskB` gerade die UART und der höherpriore `TaskA` will sie
>    haben, **erbt `TaskB` temporär Priorität 2**, bis er freigibt. So kann kein
>    mittelpriorer Task `TaskB` verdrängen und `TaskA` damit indirekt blockieren —
>    die **Prioritätsinversion** ist vermieden. Ein binäres Semaphor bietet diesen
>    Schutz *nicht*.
>
> Faustregel: *Ressource schützen, die ein Task „besitzt" und selbst wieder
> freigibt → Mutex. Ein Ereignis von A nach B signalisieren (besonders ISR → Task)
> → (binäres) Semaphor.*
>
> Übrigens: In FreeRTOS ist ein Mutex intern ein Spezialfall eines Semaphors.
> Deshalb gibt es **keinen** Typ `MutexHandle_t` — der korrekte Typ ist
> `SemaphoreHandle_t`, erzeugt über `xSemaphoreCreateMutex()`.

[Codesammlung](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/codeExamples/avr/FreeRTOS_taskHandling/src/main.cpp)

```c 
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // FreeRTOS-Funktionen für Mutexe/Semaphore (gemeinsamer Header)

// Mutex (kein Semaphor!): schützt die gemeinsame Ressource "serielle
// Schnittstelle". Take und Give erfolgen IMMER im selben Task (Besitz).
SemaphoreHandle_t xSerialMutex;

void TaskA( void *pvParameters );
void TaskB( void *pvParameters );

void setup() {

  Serial.begin(9600);
  
  if ( xSerialMutex == NULL )  
  {
    xSerialMutex = xSemaphoreCreateMutex();  // erzeugt einen Mutex
    if ( ( xSerialMutex ) != NULL )
      xSemaphoreGive( ( xSerialMutex ) );  // initial freigeben
  }

  xTaskCreate(
    TaskA
    ,  "A"  
    ,  128  
    ,  NULL
    ,  2  // Priority, with 2 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskB
    ,  "B"
    ,  128 
    ,  NULL
    ,  1  
    ,  NULL );

}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskA( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  for (;;) 
  {
    //Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    // Ist der Mutex belegt, bis zu 5 Ticks auf seine Freigabe warten.
    if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      xSemaphoreGive( xSerialMutex );
    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskB( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  for (;;)
  {
    //Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
      xSemaphoreGive( xSerialMutex ); 
    }

    vTaskDelay(1);
  }
}
```

__Was passiert mit und ohne Mutex?__

Schützt der Mutex die Ausgabe (`xSemaphoreTake` … `xSemaphoreGive` um den
`println` herum), erscheint **jede Zeile vollständig und ungeteilt** — die
Reihenfolge von A und B hängt vom Scheduling ab, aber sie verzahnen sich nie:

```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
```

Entfernt man den Mutex (beide `xSemaphoreTake`/`xSemaphoreGive`-Zeilen
auskommentieren und nur den `println` stehen lassen), zerreißt die Ausgabe
sichtbar in verschränkte A- und B-Blöcke:

```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBAAAAAAAAAAAAAAAAAAAAAAAAAAA...BBBBBBBBBBBBBBBBAAAAAAAAAAAAAA...BBB
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBAAAAAAAAAAAAAAAA...BBBBBBBBBBBBBBBBAAAAAAAAAAAAAA...BBBBBBBBB
```

> **Warum zerreißt es trotz gepufferter UART?** Im einleitenden Abschnitt hieß es,
> `Serial.println()` puffere und kehre sofort zurück — das stimmt, *aber nur
> solange die Nachricht in den Sendepuffer passt*. Der TX-Ringpuffer von
> `HardwareSerial` ist auf dem ATmega328 nur **64 Byte** groß; die A/B-Strings hier
> sind aber **~74 Zeichen** lang. Sobald der Puffer voll ist, kann `Serial.write()`
> die Bytes nicht mehr nur ablegen, sondern **busy-wartet**, bis die Hintergrund-ISR
> wieder Platz geschaffen hat:
>
> ```cpp
> // HardwareSerial::write(), sinngemäß:
> while (nächster_Kopf == _tx_buffer_tail) { /* warten, bis die ISR den Puffer leert */ }
> ```
>
> **Genau in diesem Warten wird der Task präemptiert.** Der andere Task läuft an,
> schiebt *seine* Bytes in denselben Puffer — und so verschränken sich die Blöcke.
> Die Blockgröße (~16 Zeichen) entspricht grob dem, was pro Zeitscheibe durch den
> Puffer abfließt. Eine Nachricht **kürzer als 64 Byte** würde diesen Effekt kaum
> zeigen, weil `println` dann sofort zurückkehrt und selten mitten in der Ausgabe
> unterbrochen wird. Das Beispiel ist also bewusst „überlang" gewählt, damit der
> Mutex-Nutzen sichtbar wird — und es illustriert nebenbei, dass selbst die
> „nicht-blockierende" UART blockierend wird, sobald man ihren Puffer überfüllt.

*****************************************************************************

          {{5-6}}
*****************************************************************************

__Event Groups__

Event Groups in FreeRTOS sind ein Mechanismus zur Synchronisation von Tasks über Bitmuster. Sie ermöglichen es, mit einer Gruppe von Bits (Events) komplexe Abhängigkeiten zwischen Tasks auszudrücken.

Eine Event Group besteht intern aus einem 32-Bit-Wert. Jedes Bit steht für ein bestimmtes Ereignis (Event).

Tasks können:

* Bits setzen (xEventGroupSetBits)
* auf Bits warten (xEventGroupWaitBits)
* Bits löschen (xEventGroupClearBits)

Event Groups können auf mehrere Bits gleichzeitig warten, und entsprechende UND/ODER-Logik anwenden.

```c 
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <stdio.h>

// Bitdefinitionen
#define BIT_0  (1 << 0)
#define BIT_1  (1 << 1)

// Globale EventGroup
EventGroupHandle_t xEventGroup;

void vTaskA(void *pvParameters)
{
    // Warten auf beide Bits (BIT_0 und BIT_1)
    EventBits_t bits = xEventGroupWaitBits(
        xEventGroup,
        BIT_0 | BIT_1,
        pdTRUE,      // Bits nach Empfang löschen
        pdTRUE,      // Warte auf ALLE gesetzten Bits (AND)
        portMAX_DELAY
    );

    printf("Task A: Beide Events empfangen! Bits: %02X\n", bits);

    vTaskDelete(NULL);
}

void vTaskB(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("Task B: setzt BIT_0\n");
    xEventGroupSetBits(xEventGroup, BIT_0);
    vTaskDelete(NULL);
}

void vTaskC(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("Task C: setzt BIT_1\n");
    xEventGroupSetBits(xEventGroup, BIT_1);
    vTaskDelete(NULL);
}

int main(void)
{
    xEventGroup = xEventGroupCreate();

    xTaskCreate(vTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vTaskB, "TaskB", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTaskC, "TaskC", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    for(;;);
}

```

*****************************************************************************

### Denken wie ein RTOS Entwickler

```c
#define configUSE_TRACE_FACILITY    1    // ermöglicht, dass man Trace-Makros wie traceTASK_SWITCHED_IN nutzen kann.
#define INCLUDE_pcTaskGetName       1    // erlaubt Zugriff auf pcTaskGetName() für lesbare Tasknamen.

#define traceTASK_SWITCHED_IN()    TaskSwitchedIn(pxCurrentTCB)
#define traceTASK_SWITCHED_OUT()   TaskSwitchedOut(pxCurrentTCB)
```


```c
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>

// Prototypen der Trace-Funktionen (müssen "C" sein, damit der Linker passt)
extern "C" void TaskSwitchedIn(TaskHandle_t tcb);
extern "C" void TaskSwitchedOut(TaskHandle_t tcb);

// Implementierung der Trace-Funktionen
extern "C" void TaskSwitchedIn(TaskHandle_t tcb) {
  const char* name = pcTaskGetName(tcb);
  Serial.print("[IN ]  ");
  Serial.print(name);
  Serial.print("  Tick: ");
  Serial.println(xTaskGetTickCount());
}

extern "C" void TaskSwitchedOut(TaskHandle_t tcb) {
  const char* name = pcTaskGetName(tcb);
  Serial.print("[OUT]  ");
  Serial.print(name);
  Serial.print("  Tick: ");
  Serial.println(xTaskGetTickCount());
}

// Beispiel-Task 1
void Task1(void *pvParameters) {
  (void) pvParameters;
  while (1) {
    Serial.println("Task1 läuft...");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Beispiel-Task 2
void Task2(void *pvParameters) {
  (void) pvParameters;
  while (1) {
    Serial.println("Task2 läuft...");
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Warte auf Serial Monitor

  Serial.println("FreeRTOS Task Switch Logging startet");

  xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
  xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS Tasks übernehmen die Kontrolle
}
```


```
FreeRTOS Task Switch Logging startet
[OUT]  IDLE  Tick: 0
[IN ]  Task1  Tick: 1
Task1 läuft...
[OUT]  Task1  Tick: 1001
[IN ]  Task2  Tick: 1001
Task2 läuft...
...
```

+ Die Anpassung der `FreeRTOSConfig.h` sollte lokal erfolgen!
+ Das Logging per Serial ist langsam und kann das Timing beeinflussen, daher nur für Debug-Zwecke geeignet.


![alt-text](../images/08_Algorithms/tracealyzer-for-freertos.png "Darstellung der Analyse-Tools der Firma [perceio](https://percepio.com/tz/freertostrace/)")

!?[alt-text](https://www.youtube.com/watch?v=9UqIZW1PDUI)

## FreeRTOS auf verschiedenen Architekturen

Die FreeRTOS-API ist plattformübergreifend identisch — `xTaskCreate()`, `xSemaphoreTake()` etc. funktionieren auf allen Architekturen gleich. Die Unterschiede liegen in den Ressourcen und Hardwaremechanismen, die dem RTOS-Kernel zur Verfügung stehen.

<!--data-type="none"-->
| Aspekt | AVR ATmega328 (8-Bit) | STM32F401 Cortex-M4 (32-Bit) | ESP32-S3 Xtensa LX7 (32-Bit) |
|--------|----------------------|------------------------------|-------------------------------|
| RAM | 2 KB | 96 KB | 512 KB |
| Max. sinnvolle Tasks | 3–4 | Dutzende | Dutzende |
| Kontextwechsel | Software (~80 Taktzyklen, 32×8-Bit Register sichern) | Hardware-unterstützt (automatisches Stacking durch NVIC) | Hardware-unterstützt |
| Tick-Timer | 8-Bit Timer0 (bzw. Watchdog), ~1 ms Auflösung bei 16 MHz | 24-Bit SysTick, getaktet mit bis zu 84 MHz CPU-Takt | Hardware-Timer, 240 MHz CPU-Takt |
| MPU-Support | Nein | Ja (Task-Isolation) | Teilweise |
| Multi-Core (SMP) | Nein | Nein (Single-Core) | Ja (Dual-Core) |
| Tickless Idle (Low Power) | Eingeschränkt | Ja | Ja |

### Stack-Verbrauch als limitierender Faktor

Auf dem AVR mit 2 KB RAM ist der Stack-Verbrauch der entscheidende Engpass. Jeder Task benötigt seinen eigenen Stack — typisch 128–256 Bytes minimal. Dazu kommen der Idle-Task und der Timer-Task von FreeRTOS selbst.

```
  AVR ATmega328 (2048 Bytes RAM)         STM32F401 (96 KB RAM)
  ┌─────────────────────┐                ┌─────────────────────┐
  │ FreeRTOS Kernel ~300│                │ FreeRTOS Kernel ~2K │
  │ Idle Task      ~128 │                │ Idle Task      ~256 │
  │ Task 1         ~200 │                │ Task 1        ~1024 │
  │ Task 2         ~200 │                │ Task 2        ~1024 │
  │ Task 3         ~200 │                │ Task 3        ~1024 │
  │ Heap/Globals   ~500 │                │ ... weitere Tasks   │
  │ ─ ─ ─ ─ ─ ─ ─ ─- ─ ─│                │ Heap          ~80K  │
  │ Frei:       ~320    │                │ Frei:         ~60K+ │
  └─────────────────────┘                └─────────────────────┘
```

### FreeRTOS SMP auf dem ESP32-S3

> **Ausblick — kommendes Kapitel:** Der folgende Abschnitt ist ein *Vorgriff* auf
> das nächste Kapitel, in dem wir den ESP32 ausführlich behandeln. Hier soll er
> nur zeigen, dass dieselbe FreeRTOS-API auch Mehrkern-Systeme bedient — die
> Details (Core-Affinität, Wi-Fi/BLE-Koexistenz) folgen dort.

Der ESP32-S3 erweitert FreeRTOS um Symmetric Multi-Processing. Tasks können an einen bestimmten Core gebunden oder frei verteilt werden:

```c
// Task an Core 1 binden (z.B. Echtzeit-Regelung)
xTaskCreatePinnedToCore(controlTask, "Control", 4096, NULL, 5, NULL, 1);

// Task an Core 0 binden (z.B. Wi-Fi/BLE Kommunikation)
xTaskCreatePinnedToCore(commsTask, "Comms", 4096, NULL, 3, NULL, 0);

// Scheduler entscheidet frei über Core-Zuordnung
xTaskCreate(flexTask, "Flex", 4096, NULL, 2, NULL);
```

Typisches Muster:

- **Core 0**: Wi-Fi/BLE-Stack, MQTT, OTA-Updates
- **Core 1**: Sensorauswertung, Regelung, zeitkritische Aufgaben

<!-- TODO: Praktisches Codebeispiel AVR vs. STM32 vs. ESP32-S3 ergänzen -->
