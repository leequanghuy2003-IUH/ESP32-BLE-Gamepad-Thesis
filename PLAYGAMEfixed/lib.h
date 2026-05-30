/*************************************  DO AN TOT NGHIEP
 * ****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH
 * *********************************/
/**************************************  LE QUANG HUY
 * *******************************************/
/**********************************  NGUYEN LE THANH HIEU
 * ***************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <BleGamepad.h>
#include <PCF8575.h>
#include <Wire.h>
#include <string.h>

// ─── Display constants
// ────────────────────────────────────────────────────────
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ─── Joystick pins
// ────────────────────────────────────────────────────────────
#define L_VRY_JOYSTICK 15
#define L_VRX_JOYSTICK 4
#define R_VRY_JOYSTICK 13
#define R_VRX_JOYSTICK 12

// ─── Button counts
// ────────────────────────────────────────────────────────────
#define NUM_PCF_BUTTONS 8
#define NUM_DPAD_BUTTONS 4
#define NUM_BONUS_BUTTONS 5
#define TOTAL_BUTTONS 17 // 8 + 4 + 5

// ─── Timing constants (ms)
// ────────────────────────────────────────────────────
#define DEBOUNCE_DELAY_MS 20
#define BATTERY_UPDATE_INTERVAL_MS 5000
#define SLEEP_TIMEOUT_MS 30000 // 30 s idle before deep sleep

// ─── Battery thresholds
// ───────────────────────────────────────────────────────
#define BATTERY_VOLTAGE_MIN 1.9f
#define BATTERY_VOLTAGE_MAX 3.1f
#define BATTERY_VOLTAGE_RANGE 1.2f // MAX - MIN
#define BATTERY_LOW_PERCENT 15     // Warn below 15 %

// ─── Joystick dead zone
// ───────────────────────────────────────────────────────
#define JOYSTICK_DEAD_ZONE 150 // ±150 ADC counts (~3.7 % of 4095)
#define JOYSTICK_CENTER 2048

// ─── BLE Status
// ───────────────────────────────────────────────────────────────
namespace main {
enum BLE_Status { BLE_DISCONNECTED = 0, BLE_CONNECTED = 1 };
}

// ─── Forward declarations
// ─────────────────────────────────────────────────────
namespace main {

namespace _ACTION {
extern BLE_Status status_BLE;
extern unsigned long millis_ADC;
void main();
} // namespace _ACTION

namespace _CONFIG {
void init();
}

namespace _BLE {
extern BleGamepad Bluetooth_Game;
extern const int buttons[13];
extern const int buttons_S[4];
void init();
} // namespace _BLE

namespace _JOY {
extern int value1, value2, value3, value4;
void init();
void main();
} // namespace _JOY

namespace _OLED {
void init();
void status(BLE_Status ble_status);
void main(int percent);
void testdrawroundrect();
} // namespace _OLED

namespace _PCF {
extern const int buttonPins[12];
extern PCF8575 pcf8575;
void init();
} // namespace _PCF

namespace _INP {
extern const int buttonPins_bonus[5];
void init();
} // namespace _INP

namespace _ADC {
extern int PIN_percent;
void init();
void main();
} // namespace _ADC

} // namespace main