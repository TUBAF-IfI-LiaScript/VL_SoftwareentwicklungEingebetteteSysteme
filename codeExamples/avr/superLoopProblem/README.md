# Warum eine Super-Loop versagt — und ein RTOS hilft

Dieses Beispiel begleitet das Eskalations-Narrativ aus der Vorlesung
[09_FreeRTOS.md](../../../lectures/09_FreeRTOS.md) und macht den Unterschied
zwischen einer Super-Loop und FreeRTOS **auf realer Hardware messbar**.

## Szenario

Zwei Aufgaben mit unterschiedlichen, voneinander unabhängigen Zeitbasen
laufen auf einem Arduino Uno (ATmega328):

| Aufgabe          | Periode | Anforderung                                              |
| ---------------- | ------- | -------------------------------------------------------- |
| **Regler** (A)   | 10 ms   | **hart** — Abtastzeit ist Teil der Reglerauslegung       |
| **Display** (B)  | 250 ms  | unkritisch — darf gerne mal verschoben werden            |

Der Haken: Die Display-Aktualisierung belegt den I²C-Bus — und damit die CPU —
für mehrere Millisekunden. Der Framebuffer eines 128×64-OLED (SSD1306) ist
128·64 = 8192 Bit = **1024 Byte**. Jedes Byte braucht auf dem I²C-Bus 9 Bit-Zeiten
(8 Daten + 1 ACK). Ein voller Frame bei 400 kHz (Fast Mode):

```
1024 · 9 Bit / 400 000 Bit/s ≈ 23 ms
```

— also etwa **zwei** Reglerperioden. Bei 100 kHz wären es sogar ~92 ms. Im
Beispiel ist diese Buszeit mit `delayMicroseconds()` modelliert (1024 Byte à
~22 µs), damit der Effekt auch **ohne** angeschlossenes OLED am Oszilloskop
sichtbar wird.

> **Warum blockiert I²C, UART aber nicht?** Beide nutzen eine Hardware-Peripherie
> (USART bzw. TWI) fürs Bit-Timing — der Unterschied liegt in der *Bibliothek*.
> `Serial.print()` puffert und sendet per ISR im Hintergrund, kehrt also sofort
> zurück. Die Arduino-`Wire`-Bibliothek ist auf dem AVR dagegen **blockierend**:
> `Wire.endTransmission()` → `twi_writeTo()` busy-wartet mit
> `while(wait && (TWI_MTX == twi_state)){…}` auf den Abschluss der Übertragung.
> Die CPU steckt die vollen ~23 ms in dieser Schleife fest. Genau diese Art von
> Last — eine blockierende Bibliothek, die nicht an eine ISR delegiert — macht das
> RTOS-Argument zwingend: Nur **Präemption** gibt die Zeit dem Regler zurück.
> (Quellen: [twi.c](https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/Wire/src/utility/twi.c),
> [Wire.cpp](https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/Wire/src/Wire.cpp).)

## Die drei Stufen

| Environment        | Datei                   | Idee                                  | Ergebnis am 10-ms-Takt          |
| ------------------ | ----------------------- | ------------------------------------- | ------------------------------- |
| `stage1_naiv`      | `src/stage1_naiv.cpp`   | `delay(10)` + blockierende I²C-Ausgabe | bricht in **jedem** Display-Zyklus zusammen |
| `stage2_timer`     | `src/stage2_timer.cpp`  | `millis()`-Zeitbasen, kooperativ      | besser, aber **immer noch** Aussetzer beim Schreiben |
| `stage3_freertos`  | `src/stage3_freertos.cpp` | zwei Tasks + Präemption + `vTaskDelayUntil()` | **stabil**, auch während der Ausgabe |

## Messen

An **Pin D8** (`PIN_REGLER`) wird bei jedem Reglerschritt getoggelt. Auf
einem Oszilloskop oder Logikanalysator entsteht so eine Rechteckschwingung,
deren Halbperiode genau dem Reglertakt entspricht (10 ms → 50 Hz am Pin).

```
stage1 / stage2:                        stage3 (FreeRTOS):

  __    __    __      __    __             __    __    __    __    __
 |  |  |  |  |  |    |  |  |  |            |  |  |  |  |  |  |  |  |  |
_|  |__|  |__|  |____|  |__|  |__         _|  |__|  |__|  |__|  |__|  |__
                ^^^^                       gleichmäßig — kein Einbruch
                Display blockiert
                ~23 ms = ~2 Takte weg
```

## Bauen & Flashen

```bash
pio run -e stage1_naiv     -t upload   # die naive Variante
pio run -e stage2_timer    -t upload   # die "von Hand optimierte" Variante
pio run -e stage3_freertos -t upload   # die RTOS-Lösung

pio device monitor -b 9600             # Statusmeldung beim Start ansehen
```

## Kernaussage

Stufe 2 zeigt: Mit `millis()` lässt sich eine kooperative Zeitscheibe bauen —
solange **jede** Aufgabe freiwillig schnell zurückkehrt. Sobald eine Aufgabe
blockiert oder lange rechnet, bräuchte man **Präemption**, und die baut man
nicht mehr sinnvoll von Hand. Genau das liefert ein RTOS: Prioritäten +
präemptiver Scheduler trennen *Funktionalität* von *Timing*.
