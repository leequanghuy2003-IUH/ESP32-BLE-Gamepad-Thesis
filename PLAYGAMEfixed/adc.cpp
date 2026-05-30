/*************************************  DO AN TOT NGHIEP
 * ****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH
 * *********************************/
/**************************************  LE QUANG HUY
 * *******************************************/
/**********************************  NGUYEN LE THANH HIEU
 * ***************************************/

#include "lib.h"

namespace main {
namespace _ADC {

int PIN_percent = 0;

// Moving Average filter state
static const int NUM_SAMPLES = 20;
static int sampleIndex = 0;
static float samples[NUM_SAMPLES];
static float total = 0;
static float average = 0;
static bool firstRun = true;

// ── Init ──────────────────────────────────────────────────────────────────
void init() {
  pinMode(14, INPUT);
  for (int i = 0; i < NUM_SAMPLES; i++)
    samples[i] = 0;
}

// ── Main ──────────────────────────────────────────────────────────────────
void main() {
  // 1. Read raw ADC
  int rawADC = analogRead(14);
  float volt = rawADC * 3.3f / 4095.0f;

  // 2. Moving Average — populate fully on first run
  if (firstRun) {
    for (int i = 0; i < NUM_SAMPLES; i++)
      samples[i] = volt;
    total = volt * NUM_SAMPLES;
    firstRun = false;
  }

  // 3. Circular-buffer update (O(1))
  total -= samples[sampleIndex];
  samples[sampleIndex] = volt;
  total += volt;
  sampleIndex = (sampleIndex + 1 >= NUM_SAMPLES) ? 0 : sampleIndex + 1;

  average = total / NUM_SAMPLES;

  // 4. Clamp voltage to valid range
  float stableVolt = average;
  if (stableVolt < BATTERY_VOLTAGE_MIN)
    stableVolt = BATTERY_VOLTAGE_MIN;
  if (stableVolt > BATTERY_VOLTAGE_MAX)
    stableVolt = BATTERY_VOLTAGE_MAX;

  // 5. Convert to percentage
  int pct = (int)((stableVolt - BATTERY_VOLTAGE_MIN) * 100.0f /
                  BATTERY_VOLTAGE_RANGE);
  if (pct > 100)
    pct = 100;
  if (pct < 0)
    pct = 0;

  PIN_percent = pct;
}

} // namespace _ADC
} // namespace main