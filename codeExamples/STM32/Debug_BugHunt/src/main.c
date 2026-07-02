/*
 * ============================================================================
 *  Debug-Übung "Bug Hunt"  —  STM32 Nucleo-F401RE
 * ============================================================================
 *
 *  Ein kleines Programm wertet ein Feld von "Messwerten" aus: es berechnet
 *  Summe, Mittelwert und Maximum. Klingt harmlos — enthält aber DREI bewusst
 *  eingebaute Fehler, aufsteigend in der Schwierigkeit.
 *
 *  Deine Aufgabe: Finde sie MIT DEM DEBUGGER (Breakpoints, Watch, Watchpoints),
 *  nicht durch bloßes Draufstarren. Die erwarteten (korrekten) Ergebnisse für
 *  das Test-Array stehen unten in der Übungsbeschreibung.
 *
 *  Die globalen Ergebnisvariablen (sum, average, maximum) sind bewusst global,
 *  damit sie im Watch-Fenster durchgehend sichtbar sind. Die grüne LED (PA5)
 *  blinkt schnell, wenn alle Ergebnisse stimmen, und langsam, wenn nicht —
 *  ein erster Hinweis, dass etwas nicht passt.
 * ============================================================================
 */

#include "stm32f4xx_hal.h"

#define LED_PIN               GPIO_PIN_5
#define LED_GPIO_PORT         GPIOA
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

/* --- Testdaten -------------------------------------------------------------
 * Fünf Messwerte. Korrekte Auswertung:
 *   Summe    = 200 + 180 + 220 + 190 + 210 = 1000
 *   Mittel   = 1000 / 5 = 200
 *   Maximum  = 220
 */
#define N 5
const uint16_t messwerte[N] = { 200, 180, 220, 190, 210 };

/* Globale Ergebnisse — im Debugger gut beobachtbar. */
volatile uint8_t  sum      = 0;   /* BUG 1 steckt schon im TYP dieser Variable */
volatile uint16_t average  = 0;
volatile uint16_t maximum  = 0;   /* Endergebnis des Maximums (wird unten gesetzt) */

/*
 * Wertet das Messwert-Array aus.
 * Enthält drei Fehler — siehe Übungsblatt.
 */
__attribute__((noinline))
void auswerten(void)
{
  /* ---- BUG 1: Overflow -------------------------------------------------
   * `sum` ist uint8_t (max. 255). Die echte Summe ist 1000 -> läuft über.
   * Im Debugger: sum "kippt" beim Aufaddieren (…, 255, dann wieder klein).  */
  sum = 0;
  for (uint8_t i = 0; i < N; i++) {
    sum += messwerte[i];
  }

  /* Mittelwert aus (der fehlerhaften) Summe. */
  average = sum / N;

  /* ---- BUG 2: uninitialisierte lokale Variable -------------------------
   * `lokales_max` ist eine LOKALE (Stack-)Variable und wird NICHT gesetzt,
   * bevor sie im Vergleich benutzt wird. Auf dem Stack steht dann ein
   * beliebiger "Müllwert" vom vorherigen Funktionsaufruf.
   * Im Debugger: `print lokales_max` VOR der Schleife zeigt einen
   * unbestimmten Wert — ist der zufällig größer als alle Messwerte, bleibt
   * das Maximum falsch. (Anders als eine globale Variable wird der Stack
   * NICHT vom Startup-Code auf 0 genullt!)                                   */
  uint16_t lokales_max;   /* <-- fehlende Initialisierung = 0 */
  for (uint8_t i = 0; i < N; i++) {
    if (messwerte[i] > lokales_max) {
      lokales_max = messwerte[i];
    }
  }
  maximum = lokales_max;

  /* ---- BUG 3: Off-by-one -----------------------------------------------
   * Diese Prüf-Schleife läuft von 0 BIS EINSCHLIESSLICH N (i <= N) und
   * greift damit auf messwerte[5] zu — ein Element hinter dem Array.
   * Im Debugger: mit einem Watchpoint auf `guard` sieht man, dass der
   * Zugriff Nachbarspeicher liest; oder i erreicht den Wert N=5.            */
  volatile uint16_t guard = 0xBEEF;   /* "Wächter" direkt hinter dem Array-Zugriff */
  uint32_t pruefsumme = 0;
  for (uint8_t i = 0; i <= N; i++) {   /* <-- falsch: <= statt < */
    pruefsumme += messwerte[i];
  }
  (void)pruefsumme;
  (void)guard;
}

static uint8_t ergebnisse_korrekt(void)
{
  return (sum == 200) && (average == 200) && (maximum == 220);
}

static void LED_Init(void)
{
  LED_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin   = LED_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

int main(void)
{
  HAL_Init();
  LED_Init();

  auswerten();                          /* <-- guter Ort für den ersten Breakpoint */

  uint32_t blink = ergebnisse_korrekt() ? 100 : 800;  /* schnell = ok, langsam = Bug */

  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    HAL_Delay(blink);
  }
}

/* ---- Minimale ISR-Handler ------------------------------------------------- */

void SysTick_Handler(void) { HAL_IncTick(); }
void NMI_Handler(void)     { }
void HardFault_Handler(void) { while (1) { } }
