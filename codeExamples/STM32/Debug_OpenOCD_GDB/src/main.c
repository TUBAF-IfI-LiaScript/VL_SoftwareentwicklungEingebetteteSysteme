/*
 * Debug-Demo für STM32 Nucleo-F401RE — schlankes HAL-Beispiel.
 *
 * Das Programm blinkt die On-Board-LED (LD2 an PA5) und führt dabei einen
 * Zähler sowie eine Summe mit. Es ist bewusst so aufgebaut, dass sich der
 * Debug-Ablauf gut zeigen lässt:
 *
 *   - `counter` / `accumulator` : im Watch-Fenster beobachtbare Variablen,
 *      die sich bei jedem Durchlauf ändern.
 *   - compute_next()            : Ziel zum "Step Into" (in die Funktion
 *      hineinspringen) und zum Setzen eines Breakpoints.
 *
 * Vorschlag für die Vorführung:
 *   1. Breakpoint auf die Zeile `accumulator += compute_next(counter);`
 *   2. Programm läuft bis dorthin -> counter im Watch beobachten
 *   3. "Step Into" in compute_next() -> lokale Variable `result` ansehen
 *   4. "Continue" -> Breakpoint feuert erneut, Werte haben sich erhöht
 */

#include "stm32f4xx_hal.h"

#define LED_PIN               GPIO_PIN_5
#define LED_GPIO_PORT         GPIOA
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

/* Globale Variablen — bewusst global, damit sie im Debugger leicht im
 * Watch-Fenster sichtbar sind (lokale Variablen wären außerhalb ihres
 * Scopes nicht einsehbar). */
volatile uint32_t counter     = 0;
volatile uint32_t accumulator = 0;

/*
 * Kleine Berechnung als Breakpoint-/Step-Into-Ziel.
 * Enthält eine lokale Variable `result`, deren Wert man im Debugger
 * schrittweise entstehen sehen kann.
 *
 * __attribute__((noinline)) ist hier bewusst gesetzt: Ohne dieses Attribut
 * "inlined" der Compiler die winzige Funktion bei -Og/-O2 in main() hinein —
 * dann existiert sie im Binary nicht mehr als eigenständige Funktion und
 * `break compute_next` scheitert mit "Function not defined". Genau dieser
 * Effekt (Optimierung entfernt Symbole) ist ein Kernthema des Debuggens.
 */
__attribute__((noinline))
uint32_t compute_next(uint32_t n)
{
  uint32_t result = n * 2 + 1;   /* hier lohnt sich ein "Step Over" vs. "Step Into" */
  return result;
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

  while (1)
  {
    counter++;
    accumulator += compute_next(counter);   /* <-- guter Ort für einen Breakpoint */

    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    HAL_Delay(500);
  }
}

/* ---- Minimale ISR-Handler, damit das HAL-Timing (HAL_Delay) läuft -------- */

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
  while (1) { }   /* Endlosschleife — im Debugger sofort als "hängt hier" sichtbar */
}
