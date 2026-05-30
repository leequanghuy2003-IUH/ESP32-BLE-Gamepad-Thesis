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
namespace _PCF {

PCF8575 pcf8575(0x20);

// P0-P11 for 8 action buttons + 4 D-pad buttons
const int buttonPins[12] = {P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11};

void init() {
  // Configure all pins as INPUT before begin()
  for (int i = 0; i < 12; i++) {
    pcf8575.pinMode(buttonPins[i], INPUT);
  }

  // Begin I2C communication — halt with blink if device not found
  if (!pcf8575.begin()) {
    Serial.println(
        F("[PCF8575] Init FAILED – check wiring and I2C address 0x20"));
    while (true) {
      delay(500);
    } // Halt — Serial Monitor shows the error
  }
  Serial.println(F("[PCF8575] Init OK"));
}

} // namespace _PCF
} // namespace main