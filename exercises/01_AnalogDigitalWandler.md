<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.5
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md
         https://github.com/liascript/CodeRunner

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/main/exercises/01_AnalogDigitalWandler.md#1)

# Analog-Digital-Wandler

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Softwareentwicklung Eingebettete Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2026`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Einarbeitung und digitale Input/Output Operationen`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/exercises/01_AnalogDigitalWandler.md](https://github.com/TUBAF-IfI-LiaScript/VL_SoftwareentwicklungEingebetteteSysteme/blob/main/exercises/01_AnalogDigitalWandler.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Diskussion des vergangenen Aufgabenblattes

Lösungsansätze
============================================

> Hier sind Sie gefragt ...

Einbettung der Simulation in die Entwicklungsumgebung
============================================

Plattformio integiert zwei Debugging Tools für den Arduino Uno unmittelbar:

+ `simavr` als reine Simulationumgebung [Link](https://docs.platformio.org/en/latest/plus/debug-tools/simavr.html)
+ `avr-stub` als Hardware-in-the-loop Debugger [Link](https://docs.platformio.org/en/latest/plus/debug-tools/avr-stub.html#debugging-tool-avr-stub)
+ `avr-jtag`
+ `unity` als Serial basierte Testumgebung 
+ ...
+ `pysimavr` als Python-basiertes Testtool

## Hinweise und Anregungen

![](https://media.giphy.com/media/9PrqNHPAdWyJVOXntF/giphy-downsized.gif)

### Filtern von Ergebnissen

Ein digitales Filter manipuliert diskrete Signale mit spezifischer Hardware oder in Form einer Software. Ein übliches Vorgehen ist die nachfolgend diskutierte Trennung zwischen der Auslegung und der Realisierung auf der konkreten Hardware.

Vorteile gegenüber analogen Filtern

+ keine Schwankungen durch Toleranz der Bauteile
+ keine Alterung der Bauteile
+ kein manueller Abgleich in der Fertigung notwendig, daher raschere Endprüfung von Geräten
+ mögliche Filterfunktionen, die mit Analogfiltern nur schwer oder gar nicht realisierbar sind, beispielsweise Filter mit linearer Phase.

Nachteile digitaler Filter

+ begrenzter Frequenzbereich (aufgrund endlicher Abtastraten)
+ begrenzter Wertebereich (durch Wertequantisierung)
+ durch interne Rundungs-, Abschneide- und Begrenzungsoperationen zur Wortlängenbegrenzung weisen digitale Filter in der Praxis Quantisierungsrauschen und andere nichtlineare Effekte auf
+ bei nichtelektrischen Ein- und Ausgangsgrößen zusätzlicher Aufwand für die Wandlung.

Beispiel FIR Filter

$$
y(n) = \sum_{n=0}^{N} b_i x(n-i)
$$

Realisierung des Filters mit der scipy Methode [firwin](https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.firwin.html).

```python       IllustrateFIR.py
from numpy import sin, arange, pi
from scipy.signal import lfilter, firwin
import matplotlib.pyplot as plt

#------------------------------------------------
# Create a signal for demonstration.
#------------------------------------------------
# 96 samples of (1000Hz + 15000 Hz) at 48 kHz (~2 Perioden 1 kHz)
sample_rate = 48000.
nsamples = 96

F_1KHz = 1000.
A_1KHz = 1.0

F_15KHz = 15000.
A_15KHz = 0.5

t = arange(nsamples) / sample_rate
signal = A_1KHz * sin(2*pi*F_1KHz*t) + A_15KHz*sin(2*pi*F_15KHz*t)

#------------------------------------------------
# Create a FIR filter and apply it to signal.
#------------------------------------------------
# The Nyquist rate of the signal.
nyq_rate = sample_rate / 2.

# The cutoff frequency of the filter: 6KHz
cutoff_hz = 6000.0

# Length of the filter (number of coefficients, i.e. the filter order + 1)
numtaps = 9

# Use firwin to create a lowpass FIR filter
fir_coeff = firwin(numtaps, cutoff_hz/nyq_rate)

# Use lfilter to filter the signal with the FIR filter
filtered_signal = lfilter(fir_coeff, 1.0, signal)

#------------------------------------------------
# Plot the original and filtered signals.
#------------------------------------------------

# The first N-1 samples are "corrupted" by the initial conditions
warmup = numtaps - 1

# The phase delay of the filtered signal
delay = (warmup / 2) / sample_rate

fig, ax = plt.subplots()
# Plot the original signal
ax.plot(t, signal)

# Plot the filtered signal, shifted to compensate for the phase delay
ax.plot(t-delay, filtered_signal, 'r-')

# Plot just the "good" part of the filtered signal.  The first N-1
# samples are "corrupted" by the initial conditions.
ax.plot(t[warmup:]-delay, filtered_signal[warmup:], 'g', linewidth=4)

ax.grid(True)

print ("Koffizienten =" + ''.join(["%+.10f" % x for x in fir_coeff]))


#plt.show()  
plt.savefig('foo.png') # notwendig für die Ausgabe in LiaScript
```
@LIA.eval(`["main.py"]`, `none`, `python3 main.py`)

Die nachfolgende Lösung illustriert die intuitive Umsetzung des Ansatzes anhand von Gleitkommazahlen. Welche Adaptionen drängen sich auf?

```c          FIRimplementation.c
#define nc 11    // Anzahl der Filterkoeffizienten

// Filterkoeffizienten b[0] = b_N, ..., b[nc - 1] = b_0
static const float b[nc] = {0.0637, 0, -0.1061, 0, 0.3183, 0.5,
                            0.3183, 0, -0.1061, 0, 0.0637};

static float circular_buffer[nc] = {0};
static uint8_t zeiger = 0;

// Wird bei jedem neuen Eingangswert aufgerufen
float fir_update(float new_sample) {
  // Schreibe neuen Eingangswert in den Ringpuffer
  circular_buffer[zeiger] = new_sample;
  // Inkrementiere Zeiger modulo nc
  zeiger = (zeiger + 1) % nc;

  // Berechne neuen Ausgangswert
  float y = 0;
  for (uint8_t i = 0; i < nc; i++) {
    y += b[i] * circular_buffer[(zeiger + i) % nc];
  }
  return y;
}
```

> **Anregungen zur Optimierung für den ATmega328P** (8-Bit, 16 MHz, _keine FPU_)
>
> Die obige Implementierung ist als Referenz gedacht — auf einem Mikrocontroller ohne Gleitkommaeinheit ist sie nicht echtzeitfähig. Folgende Schritte helfen, den Filter realistisch einsetzbar zu machen:

1. **`float` vermeiden.** Jede `float`-Multiplikation kostet auf dem AVR 150–200 Taktzyklen (Software-Emulation), bei `nc=11` also etwa 125 µs allein für Multiplikationen. Eine **Festkommaarithmetik** im Q15-Format (`int16_t × int16_t → int32_t` Akkumulator) reduziert eine MAC-Operation auf 10–15 Takte — Faktor 10–20 schneller.

2. **Koeffizienten in `PROGMEM`.** 11 × 4 Byte = 44 Byte RAM allein für die Konstanten. Mit `PROGMEM` und `pgm_read_*` wandern sie in den 32-kB-Flash. (Passt didaktisch zur PROGMEM-Aufgabe weiter unten.)

3. **Modulo-Operation eliminieren.** `%` auf 8-Bit-AVR ohne Hardware-Division kostet ~150 Takte und wird `nc`-mal pro Sample aufgerufen. Bei einer **Zweierpotenz-Länge** (`nc=8` oder `16`) wird daraus ein billiges `& (nc-1)`. Alternative: `if (++idx >= nc) idx = 0;` — ein einzelner Vergleich.

4. **Symmetrie linearphasiger Filter ausnutzen.** Die Koeffizienten `{0.0637, 0, -0.1061, 0, 0.3183, 0.5, 0.3183, 0, -0.1061, 0, 0.0637}` sind spiegelsymmetrisch. Man kann zuerst `x[k] + x[N-1-k]` addieren und dann nur mit der einen Hälfte der Koeffizienten multiplizieren → **halbe Anzahl Multiplikationen**.

5. **Ringpuffer-Schreibrichtung umkehren.** Wenn neue Samples nach hinten geschoben (oder der Index rückwärts laufen) lässt, entfällt das `(zeiger + i) % nc` zugunsten eines linearen Zugriffs `buffer[i]`. Bei größeren Filtern lohnt sich aber der Ringpuffer mehr.

6. **Nullkoeffizienten überspringen.** Im obigen Beispiel sind 4 von 11 Koeffizienten exakt 0 (typisch für Halbband-Filter). Eine ausgewählte Multiplikation spart ~36 % der MACs.

7. **Loop-Unrolling.** Bei festem, kleinem `nc` lohnt sich vollständiges Entrollen — der Compiler kann dann konstante Indizes nutzen und spart die Schleifenverwaltung.

8. **ISR-Sicherheit.** Wird `fir_update()` aus dem ADC-Interrupt aufgerufen, müssen `circular_buffer` und `zeiger` entweder atomar gelesen werden oder die ISR muss alleinigen Zugriff haben. Sonst drohen Race Conditions zwischen Filter und Hauptschleife.

> Eine sinnvolle Reihenfolge zur eigenen Umsetzung: erst (1) + (2) + (3) — das ist der größte Hebel und deckt sich mit dem Rest des Aufgabenblattes. (4) und (6) sind elegante Folgeoptimierungen, (7) und (8) eher fortgeschritten.

### Wann ADC, wann Analog-Comparator?

Der ATmega328P besitzt neben dem 10-Bit-ADC einen **Analog-Comparator**, der zwei Spannungen vergleicht und das Ergebnis als 1-Bit-Signal (`ACO`-Bit in `ACSR`) bereitstellt. Für reine **Schwellwertentscheidungen** (Lichtschranke, Über-/Unterspannung, Nulldurchgang einer AC-Größe) ist er dem ADC deutlich überlegen:

+ **Latenz:** Vergleichsergebnis liegt nach wenigen Taktzyklen vor — eine ADC-Wandlung dauert demgegenüber ~104 µs (13 Wandlertakte bei 125 kHz ADC-Clock).
+ **Energie:** Kein Sampling, keine sukzessive Approximation.
+ **Interrupt:** Der Comparator kann ohne Polling per ISR auf Flanken reagieren — ideal für ereignisgesteuerte Erkennung.

Der ADC ist immer dann nötig, wenn der **konkrete Messwert** gebraucht wird (Distanz, Temperatur, Joystick-Position).

> Ein vollständiges, kommentiertes Codebeispiel zur Konfiguration des Analog-Comparators mit Bandgap-Referenz und Multiplexer-Eingang findet sich unter [`codeExamples/avr/AnalogComparator/`](../codeExamples/avr/AnalogComparator/). Die Registerbeschreibung steht im Datenblatt [^AtmelHandbuch] im Kapitel „Analog Comparator".

[^AtmelHandbuch]: Firma Microchip, megaAVR® Data Sheet, [Link](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf)

## Aufgaben

Roter Faden dieser Übung ist der **Joystick** — von der ADC-Initialisierung über die Wertaufbereitung bis zum gefilterten Signal mit speicheroptimierten Koeffizienten.

- [ ] **Joystick anschließen und auslesen.** Verbinden Sie den Joystick mit dem Arduino über GND und +5V. `Vx` und `Vy` liefern analoge Spannungen zwischen 0 V und 5 V, `SW` ist ein digitaler Tastendruck. Schreiben Sie eine Funktion, um den ADC zu initialisieren, und eine Funktion, um die analogen Werte beider Achsen auf eine sinnvolle Auslenkungsgröße (z.B. ±100 % oder Winkel in Grad) abzubilden. Visualisieren Sie die Daten z.B. mit dem SerialPlotter der Arduino-Umgebung oder einem alternativen Tool ([Python](https://thepoorengineer.com/en/arduino-python-plot/)).

- [ ] **Joystick-Daten filtern.** Entwerfen Sie einen Filter für die Glättung der Joystick-Werte und experimentieren Sie mit unterschiedlichen Konfigurationen (Ordnung, Grenzfrequenz). Orientieren Sie sich an der oben gezeigten FIR-Implementierung — und an den Optimierungsanregungen, sobald die Float-Variante läuft.

- [ ] **Filterparameter in `PROGMEM` ablegen.** Verschieben Sie die Filterkoeffizienten mit `PROGMEM` in den Programmspeicher und greifen Sie mit `pgm_read_*` zu. Analysieren Sie die Konsequenzen anhand des erzeugten Assemblercodes und evaluieren Sie den Geschwindigkeitsnachteil.

  > **Toolchain-Hinweis:** Den Assemblercode erhalten Sie z.B. mit `pio run -t disassemble` oder direkt über `avr-objdump -d -S .pio/build/uno/firmware.elf > firmware.lss`. Vergleichen Sie die generierten Lade-Instruktionen vor und nach der Umstellung (`lds`/`ld` für RAM vs. `lpm` für Flash).


__Alternativ__

- [ ] **Internen Temperatursensor des ATmega328 auswerten.** Vergleichen Sie die Messwerte mit den Daten eines Heizleiters [Link](https://www.mangolabs.de/product/steel-head-thermistor/). Beachten Sie: der Sensor ist über `MUX[3:0] = 1000` und die interne Bandgap-Referenz (1.1 V) anzusprechen und benötigt eine individuelle Kalibrierung.

- [ ] **GDB-Interface von `simavr` testen.** Alternativ könnte auch SimulAVR eine Option sein. Auf dieser Basis ließe sich ein Prototyp für skriptgetriebenes Debugging bauen.

- [ ] **Analog-Comparator vs. ADC vermessen.** Evaluieren Sie die Zeitdauer für die Realisierung einer Messung mit dem Analog-Comparator und dem ADC. Erörtern Sie Vor- und Nachteile der beiden Ansätze.