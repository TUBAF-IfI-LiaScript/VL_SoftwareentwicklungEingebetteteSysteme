# Debug-Demo (Variante 1): PlatformIO-integriertes Debugging

Debuggen des STM32 Nucleo-F401RE **komfortabel über die PlatformIO-Oberfläche**.
PlatformIO startet OpenOCD und GDB im Hintergrund automatisch — man bekommt eine
grafische Debug-Sitzung, ohne die Werkzeuge selbst zu bedienen.

> Die **Variante 2** (`../Debug_OpenOCD_GDB`) zeigt denselben Vorgang von Hand:
> OpenOCD und `arm-none-eabi-gdb` manuell gestartet. Beide debuggen exakt
> denselben Code — nur der Komfort- vs. der „unter-der-Haube"-Weg unterscheiden sich.

## Voraussetzungen

- ST **Nucleo-F401RE** (der ST-LINK/V2-1 ist on-board — kein externer Debugger nötig)
- VS Code mit der **PlatformIO-IDE-Erweiterung**
- USB-Kabel Board ↔ PC

## Bauen & Flashen

```bash
pio run                 # kompilieren
pio run -t upload       # auf das Board flashen
```

## Debuggen — grafisch (empfohlen für die Demo)

1. Projektordner in VS Code öffnen.
2. In den Code klicken, links neben eine Zeilennummer → **Breakpoint** setzen.
   Guter Ort: die Zeile `accumulator += compute_next(counter);` in `src/main.c`.
3. **F5** (bzw. „Run and Debug" → *PIO Debug*).
   PlatformIO baut mit Debug-Flags, flasht und hält automatisch in `main()` an
   (`debug_init_break = tbreak main` in `platformio.ini`).
4. Steuerung über die Debug-Leiste:
   - **Continue (F5)** — bis zum nächsten Breakpoint laufen
   - **Step Over (F10)** — Zeile ausführen
   - **Step Into (F11)** — in `compute_next()` hineinspringen
   - **Step Out (Shift+F11)** — aus der Funktion zurück
5. **Variablen beobachten:** `counter` und `accumulator` ins *Watch*-Fenster
   ziehen. Bei jedem Schleifendurchlauf ändern sich die Werte sichtbar.

## Debuggen — Kommandozeile

```bash
pio debug --interface=gdb      # startet Server + GDB, öffnet eine (gdb)-Konsole
```

## Was man didaktisch zeigt

| Konzept              | im Beispiel                                                    |
| -------------------- | ------------------------------------------------------------- |
| Breakpoint           | Halt in der `while`-Schleife                                  |
| Watch / Variablen    | `counter`, `accumulator` live beobachten                      |
| Step Into / Over     | Verhalten an `compute_next()` vergleichen                     |
| Debug-Optimierung    | `debug_build_flags = -Og -g3` — sonst „verschwinden" Variablen |

> **Merke:** Ohne `-Og -g3` optimiert der Compiler lokale Variablen weg und
> Breakpoints landen an unerwarteten Stellen. Debug-freundlich zu übersetzen ist
> der erste Schritt jeder Embedded-Debug-Sitzung.
>
> Selbst *mit* `-Og` kann eine kurzlebige lokale Variable (wie `result` in
> `compute_next()`) im Watch als **`optimized out`** erscheinen — sie liegt dann
> nur in einem Register. Das ist normal und zeigt anschaulich, dass die
> Beobachtbarkeit von der Optimierungsstufe abhängt. Die **globalen** `counter`
> und `accumulator` bleiben dagegen durchgehend sichtbar.
