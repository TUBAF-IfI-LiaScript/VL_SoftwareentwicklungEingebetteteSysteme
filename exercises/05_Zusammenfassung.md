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

[![LiaScript](https://raw.githubusercontent.com/LiaScript/LiaScript/master/badges/course.svg)](https://liascript.github.io/course/?https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/main/exercises/05_Zusammenfassung.md#1)

# Verwendung der IDE

| Parameter                | Kursinformationen                                                                                                                                                                    |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Veranstaltung:**       | `Übungen Digitale Systeme`                                                                                                                                                      |
| **Semester**             | `Sommersemester 2021`                                                                                                                                                                |
| **Hochschule:**          | `Technische Universität Freiberg`                                                                                                                                                    |
| **Inhalte:**             | `Zusammenfassung und Ausblick`                                                                                            |
| **Link auf den GitHub:** | [https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/05_Zusammenfassung.md](https://github.com/TUBAF-IfI-LiaScript/VL_DigitaleSysteme/blob/main/lectures/05_Zusammenfassung.md) |
| **Autoren**              | @author                                                                                                                                                                              |

![](https://media.giphy.com/media/3gttGAxMSSofe/giphy-downsized.gif)

---

## Fehlertoleranz 

> Wir wollen ein System entwickeln, dass mit dem Entwicklerrechner kommuniziert und Steuergrößen empfängt. Sollte der Datenempfang länger als 100ms ausbleiben, so soll das System in einen fail-Safe Zustand wechseln.

```c
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <avr/wdt.h>  // Watchdog für echten Reset

TaskHandle_t monitorTaskHandle = NULL;

// 📥 Empfangs-Task: wartet auf serielle Daten
void SerialReceiver(void *pvParameters) {
  for (;;) {
    if (Serial.available()) {
      uint8_t val = Serial.read();
      Serial.print("Empfangen: ");
      Serial.println(val);

      // Sende Signal an MonitorTask
      xTaskNotifyGive(monitorTaskHandle);
    }
    vTaskDelay(pdMS_TO_TICKS(10));  // alle 10 ms prüfen
  }
}

// 🕵️ Überwachungs-Task: wartet auf Empfangssignale
void MonitorTask(void *pvParameters) {
  for (;;) {
    uint32_t notified = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(130));

    if (notified > 0) {
      Serial.println("Monitor: OK ✅");
    } else {
      Serial.println("❌ Timeout!");

      // 🧨 Watchdog aktivieren → Reset in 15 ms
      wdt_enable(WDTO_15MS);
      while (1);  // Warten auf Reset
    }
  }
}

void setup() {
  // 🛑 WDT absichern (falls vorher aktiv war)
  wdt_disable();

  Serial.begin(9600);
  while (!Serial);  // optional für Leonardo etc.

  // Tasks starten
  xTaskCreate(SerialReceiver,"SerialReceiver",128, NULL, 1, NULL);
  xTaskCreate(MonitorTask,   "Monitor",       128, NULL, 2, &monitorTaskHandle);
}

void loop() {
  // bleibt leer unter FreeRTOS
}
```

> Frage 1: Warum ist der Watchdog hier notwendig?
> Frage 2: Warum sind die `xTaskCreate()` Aufrufe problematisch?
> Frage 3: Warum warten wir 130 ms im `ulTaskNotifyTake()`?

## Organisation der Prüfungen

> Die Veranstaltung wird mit einer mündliche Prüfung abgeschlossen, die aus zwei Teilen besteht:

1. eigenes Implementierungsbeispiel (ATmega328, ATmega4809 mittels Assembler oder avrlibc, STM32F4 mittels HAL) ODER Edrys Beispiellabor aus dem Praktikumsteil
2. einer Diskussion zu den Themenfeldern der Vorlesung

Das konkrete Beispiel können Sie selbst wählen. Melden Sie sich bitte zur Abstimmung eines Termines.

## Diskussion des vergangenen Aufgabenblattes

> Hier sind Sie gefragt ...
