<!--
author:   Sebastian Zug, Karl Fessel
email:    sebastian.zug@informatik.tu-freiberg.de

version:  0.0.1
language: de
narrator: Deutsch Female

import:  https://raw.githubusercontent.com/liascript-templates/plantUML/master/README.md
         https://github.com/LiaTemplates/AVR8js/main/README.md
         https://github.com/liascript/CodeRunner
         https://github.com/LiaTemplates/Pyodide

icon: https://upload.wikimedia.org/wikipedia/commons/d/de/Logo_TU_Bergakademie_Freiberg.svg
-->

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/exercises/01_AnalogDigitalWandler.md#1)

# Analog-Digital-Wandler

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2021`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Einarbeitung und digitale Input/Output Operationen`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/01_AnalogDigitalWandler.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/01_AnalogDigitalWandler.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Abbildung von Wandlerergebnissen



## Filtern von Ergebnissen

```python       IllustrateFIR.py
from numpy import sin, arange, pi
from scipy.signal import lfilter, firwin
import matplotlib.pyplot as plt

#------------------------------------------------
# Create a signal for demonstration.
#------------------------------------------------
# 320 samples of (1000Hz + 15000 Hz) at 48 kHz
sample_rate = 48000.
nsamples = 320

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


plt.show()
plot(fig) # <- this is required to plot the fig also on the LiaScript canvas
```
@Pyodide.eval



Im Beispiel

$$
y(n) = \sum_{n=0}^{N} b_i x(n-i)
$$


```c          FIRimplementation.c
#define nc 11    // Anzahl der Filterkoeffizienten  
int i = 0, zeiger = 0;
float new_sample, y, circular_buffer[nc];

// Filterkoeffizienten b[0] = b_N, ..., b[nc - 1] = b_0
float b[nc] = {0.0637, 0, -0.1061, 0, 0.3183, 0.5, 0.3183, 0, -0.1061, 0, 0.0637};
float circular_buffer[i] = {0};

// Der folgende Code wird jedes Mal ausgeführt, wenn ein
// neuer Eingangswert (new_sample) zur Verfügung steht  
// Schreibe neuen Eingangswert in Buffer  

circular_buffer[zeiger] = new_sample;
// Inkrementiere Zeiger
modulo nc  zeiger = (zeiger + 1) % nc;

// Berechne neuen Ausgangswert  
y = 0;
for(i = 0; i < nc; i++){
  y += (b[i] * circular_buffer[(zeiger + i) % nc]);
}
```

## Einbettung der Simulation in die Entwicklungsumgebung

https://docs.platformio.org/en/latest/plus/debug-tools/simavr.html


## Aufgaben

- [ ] Vergleichen Sie die Werte des internen Temperatursensors des ATmega328 mit den Daten des Heizleiters [Link](https://www.mangolabs.de/product/steel-head-thermistor/). Nutzen Sie dafür ggf. den SerialPlotter der Arduino-Umgebung oder ein alternatives Visualsierungstool [Python](https://thepoorengineer.com/en/arduino-python-plot/).

- [ ] Verbinden Sie den Joystick mit dem Arduino über GND und +5V mit den zugehörigen Pins auf dem Arduino.
Vx und Vy entsprechen analogen Werten zwischen 0V und 5V der jeweiligen Achsen. SW entspricht einem
Druck auf den Joystick und ist ein digitaler Schalter.
Schreiben Sie eine Funktion, um den ADC zu initialisieren und eine Funktion, um die analogen Werte
des Joysticks auszulesen. 

- [ ] Entwerfen Sie einen Filter für die Glättung der Joystick-Daten. Experimentieren Sie mit unterschiedlichen Konfigurationen des Filters!
