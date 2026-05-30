/*************************************  DO AN TOT NGHIEP
 * *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH
 * **********************************/
/**************************************  LE QUANG HUY
 * ********************************************/
/**********************************  NGUYEN LE THANH HIEU
 * ****************************************/

#include "lib.h"

namespace main {
namespace _ACTION {

// ── State ───────────────────────────────────────────────────────────────────
BLE_Status status_BLE = BLE_DISCONNECTED;
unsigned long millis_ADC = 0;

// PCF8575 buttons (8 action + 4 D-pad)
static int switchState[TOTAL_BUTTONS] = {};
static int oldswitchState[TOTAL_BUTTONS] = {};

// Separate debounce timers for PCF action buttons and D-pad
static unsigned long millis_button = 0;
static unsigned long millis_button1 = 0;

// Bonus GPIO buttons — own debounce timer + edge-detection state
static int bonusState[NUM_BONUS_BUTTONS];
static int oldBonusState[NUM_BONUS_BUTTONS];
static unsigned long millis_bonus = 0;

// OLED dirty-flag
static int lastOledPercent = -1;
static BLE_Status lastOledStatus = BLE_DISCONNECTED;

// Low-battery warning flag (resets if charged above threshold + 5 %)
static bool lowBatteryWarned = false;

// ── Helpers ─────────────────────────────────────────────────────────────────

/** Send battery BLE notification only when connected. */
static inline void sendBattery() {
  if (main::_BLE::Bluetooth_Game.isConnected()) {
    main::_BLE::Bluetooth_Game.setBatteryLevel(main::_ADC::PIN_percent);
  }
}

// ── Main ────────────────────────────────────────────────────────────────────
void main() {

  // ── 1. Battery: refresh every 5 s ──────────────────────────────────────
  if (millis() - millis_ADC >= BATTERY_UPDATE_INTERVAL_MS) {
    main::_ADC::main();
    sendBattery();
    millis_ADC = millis();
  }

  // ── 2. OLED: update only when content changes (dirty flag) ─────────────
  bool oledDirty = (main::_ADC::PIN_percent != lastOledPercent) ||
                   (status_BLE != lastOledStatus);
  if (oledDirty) {
    main::_OLED::status(status_BLE);
    main::_OLED::main(main::_ADC::PIN_percent);
    lastOledPercent = main::_ADC::PIN_percent;
    lastOledStatus = status_BLE;
  }

  // ── 3. Low-battery visual warning ──────────────────────────────────────
  if (main::_ADC::PIN_percent < BATTERY_LOW_PERCENT && !lowBatteryWarned) {
    // Flash OLED 3× to warn player
    for (int i = 0; i < 3; i++) {
      main::_OLED::main(main::_ADC::PIN_percent); // on
      delay(180);
      main::_OLED::status(BLE_DISCONNECTED); // blank status (flicker)
      delay(180);
    }
    lowBatteryWarned = true;
  }
  if (main::_ADC::PIN_percent > BATTERY_LOW_PERCENT + 5) {
    lowBatteryWarned = false; // Reset if charged
  }

  // ── 4. BLE-connected block ─────────────────────────────────────────────
  if (main::_BLE::Bluetooth_Game.isConnected()) {

    // 4a. Detect first connection event
    if (status_BLE == BLE_DISCONNECTED) {
      status_BLE = BLE_CONNECTED;
      // Send current battery level immediately on connect
      sendBattery();
    }

    // 4b. Joysticks — read every loop (no debounce needed for analog)
    main::_JOY::main();
    main::_BLE::Bluetooth_Game.setLeftThumb(main::_JOY::value2,
                                            main::_JOY::value1);
    main::_BLE::Bluetooth_Game.setRightThumb(main::_JOY::value4,
                                             main::_JOY::value3);

    // 4c. 8 PCF8575 action buttons — debounced + edge detection
    if (millis() - millis_button > DEBOUNCE_DELAY_MS) {
      millis_button = millis();
      for (int i = 0; i < NUM_PCF_BUTTONS; i++) {
        switchState[i] =
            main::_PCF::pcf8575.digitalRead(main::_PCF::buttonPins[i]);
      }
    }
    for (int i = 0; i < NUM_PCF_BUTTONS; i++) {
      if (switchState[i] == LOW && oldswitchState[i] == HIGH)
        main::_BLE::Bluetooth_Game.press(main::_BLE::buttons[i]);
      else if (switchState[i] == HIGH && oldswitchState[i] == LOW)
        main::_BLE::Bluetooth_Game.release(main::_BLE::buttons[i]);
      oldswitchState[i] = switchState[i];
    }

    // 4d. 4 PCF8575 D-pad buttons — debounced + edge detection
    if (millis() - millis_button1 > DEBOUNCE_DELAY_MS) {
      millis_button1 = millis();
      for (int j = 0; j < NUM_DPAD_BUTTONS; j++) {
        switchState[j + NUM_PCF_BUTTONS] = main::_PCF::pcf8575.digitalRead(
            main::_PCF::buttonPins[j + NUM_PCF_BUTTONS]);
      }
    }
    for (int j = 0; j < NUM_DPAD_BUTTONS; j++) {
      int idx = j + NUM_PCF_BUTTONS;
      if (switchState[idx] == LOW && oldswitchState[idx] == HIGH)
        main::_BLE::Bluetooth_Game.setHat1(main::_BLE::buttons_S[j]);
      else if (switchState[idx] == HIGH && oldswitchState[idx] == LOW)
        main::_BLE::Bluetooth_Game.setHat1();
      oldswitchState[idx] = switchState[idx];
    }

    // 4e. 5 bonus GPIO buttons — debounced + edge detection
    //     (previously had no debounce → fixed!)
    if (millis() - millis_bonus > DEBOUNCE_DELAY_MS) {
      millis_bonus = millis();
      for (int k = 0; k < NUM_BONUS_BUTTONS; k++) {
        bonusState[k] = digitalRead(main::_INP::buttonPins_bonus[k]);
      }
    }
    for (int k = 0; k < NUM_BONUS_BUTTONS; k++) {
      int bleIdx = k + NUM_PCF_BUTTONS; // maps to buttons[8..12]
      if (bonusState[k] == LOW && oldBonusState[k] == HIGH)
        main::_BLE::Bluetooth_Game.press(main::_BLE::buttons[bleIdx]);
      else if (bonusState[k] == HIGH && oldBonusState[k] == LOW)
        main::_BLE::Bluetooth_Game.release(main::_BLE::buttons[bleIdx]);
      oldBonusState[k] = bonusState[k];
    }

  } else {
    // ── 5. Disconnected ─────────────────────────────────────────────────
    status_BLE = BLE_DISCONNECTED;
  }
}

} // namespace _ACTION
} // namespace main
