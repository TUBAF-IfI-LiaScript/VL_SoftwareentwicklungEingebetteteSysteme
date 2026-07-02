# Debug-Demo (Variante 2): OpenOCD + GDB von Hand

Derselbe Code wie in [`../Debug_PlatformIO`](../Debug_PlatformIO), aber der
Debug-Vorgang wird **manuell** aufgebaut. Ziel: sichtbar machen, *was* PlatformIO
in Variante 1 im Hintergrund automatisiert.

## Das Bild dahinter

Ein Debugger auf dem PC steuert keinen Chip direkt — dazwischen sitzen zwei
Bausteine:

```ascii
  arm-none-eabi-gdb          OpenOCD                ST-LINK          STM32
  (Debugger, PC)   ◄─────►  (GDB-Server)  ◄─────►  (on-board)  ◄──►  Cortex-M4
      "was tun?"    TCP       übersetzt      USB     SWD-Adapter  SWD   Ziel
                   :3333     GDB <-> SWD
```

- **GDB** kennt Quellcode, Breakpoints, Variablen — spricht aber kein SWD.
- **OpenOCD** ist die Brücke: bietet GDB einen Server auf TCP-Port **3333** an
  und übersetzt dessen Befehle in SWD-Transaktionen zum ST-LINK.
- **ST-LINK** (auf dem Nucleo-Board integriert) spricht **SWD** mit dem Ziel-Chip
  — genau das Debug-Interface aus der Vorlesung (Kap. 11, Abschnitt *SWD*).

## Voraussetzungen

- ST **Nucleo-F401RE** (ST-LINK on-board)
- **OpenOCD** und **arm-none-eabi-gdb** installiert. Beide sind bereits in den
  PlatformIO-Toolchains enthalten:
  - OpenOCD:  `~/.platformio/packages/tool-openocd/bin/openocd`
  - GDB:      `~/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gdb`
  - (oder systemweit installierte Versionen)

## Schritt 1 — Bauen

```bash
pio run          # erzeugt .pio/build/nucleo_f401re/firmware.elf
```

Die **.elf**-Datei ist wichtig: Sie enthält die Debug-Symbole (Variablennamen,
Zeilennummern), die GDB braucht.

## Schritt 2 — OpenOCD als GDB-Server starten (Terminal A)

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```

> **Config-Dateien nicht gefunden?** Die `.cfg` liegen im `scripts`-Ordner der
> OpenOCD-Installation. Bei der PlatformIO-Toolchain ist das:
> `~/.platformio/packages/tool-openocd/openocd/scripts`. Diesen Pfad mit `-s`
> voranstellen, dann werden `interface/…` und `target/…` relativ dazu gefunden:
>
> ```bash
> OCD=~/.platformio/packages/tool-openocd
> "$OCD/bin/openocd" -s "$OCD/openocd/scripts" \
>     -f interface/stlink.cfg -f target/stm32f4x.cfg
> ```

Erwartete Ausgabe (gekürzt, real auf dem Nucleo-F401RE beobachtet):

```
Info : STLINK V2J39M27 (API v2) VID:PID 0483:374B
Info : Target voltage: 3.26 V
Info : [stm32f4x.cpu] Cortex-M4 r0p1 processor detected
Info : [stm32f4x.cpu] target has 6 breakpoints, 4 watchpoints
Info : Listening on port 3333 for gdb connections
```

Die letzte Zeile ist entscheidend — OpenOCD wartet nun auf Port **3333**.
Dieses Terminal **offen lassen**.

## Schritt 3 — GDB verbinden (Terminal B)

```bash
arm-none-eabi-gdb .pio/build/nucleo_f401re/firmware.elf
```

In der `(gdb)`-Konsole:

```gdb
target extended-remote localhost:3333   # mit dem OpenOCD-Server verbinden
monitor reset halt                      # Ziel zurücksetzen und anhalten
load                                     # firmware.elf auf den Chip flashen
break main                               # Breakpoint auf main() setzen
continue                                 # bis main() laufen
```

## Schritt 4 — Debuggen (dieselbe Demo wie Variante 1)

```gdb
break compute_next                # Breakpoint auf die Funktion
continue                          # bis compute_next() laufen
print n                           # Argument ansehen
step                              # in die Funktion / Zeile hinein
print result                      # lokale Variable ansehen
info locals                       # alle lokalen Variablen
print counter                     # globale Variable
print accumulator
continue                          # nächster Durchlauf
```

> **Lehrmoment — `value has been optimized out`:** Beim `print result` nach dem
> `step` meldet GDB unter Umständen genau das. Das ist **kein Fehler**, sondern
> der Optimierer bei der Arbeit: Bei `-Og` hält der Compiler die kurzlebige
> lokale Variable `result` nicht durchgängig in einem eigenen, adressierbaren
> Speicherplatz — sie „lebt" zeitweise nur in einem Register oder wird direkt
> in den Rückgabewert gefaltet. Beobachtbar bleiben dagegen die **globalen**
> Variablen `counter` und `accumulator` (`print counter`). Merksatz für die
> Vorlesung: *Was man debuggen kann, hängt von der Optimierungsstufe ab* — mit
> `-O0` wäre `result` sichtbar, aber der Code entspräche weniger dem, was später
> released wird. `-Og` ist der bewusste Kompromiss.

Nützliche Befehle:

| GDB-Befehl            | Wirkung                                 |
| --------------------- | --------------------------------------- |
| `next` / `n`          | Zeile ausführen (Step Over)             |
| `step` / `s`          | in Funktion hineinspringen (Step Into)  |
| `continue` / `c`      | bis zum nächsten Breakpoint             |
| `print <var>` / `p`   | Variable ausgeben                       |
| `info registers`      | CPU-Register anzeigen                   |
| `backtrace` / `bt`    | Aufrufkette (Call Stack)                |
| `monitor reset halt`  | Ziel über OpenOCD zurücksetzen          |
| `quit`                | GDB beenden                             |

## Der Bezug zu Variante 1

Alles, was hier von Hand passiert — OpenOCD starten, `target remote`, `load`,
`break main` — erledigt PlatformIO in Variante 1 automatisch beim Druck auf F5.
Wer diesen Ablauf einmal manuell gesehen hat, versteht, warum eine Debug-Sitzung
scheitern kann (falsche `.cfg`, Port 3333 belegt, fehlende Symbole in der `.elf`)
— und kann gezielt eingreifen.

> **Merke:** GDB ↔ OpenOCD ↔ ST-LINK ↔ SWD ist die Standard-Kette für
> Cortex-M-Debugging. Dieselben Bausteine automatisiert später ein CI-Runner,
> um On-Target-Tests zu flashen und auszuwerten (siehe Kap. 11, *CI/CD für
> Embedded*).
