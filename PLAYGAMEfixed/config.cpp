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
namespace _CONFIG {

void init() {
  Serial.begin(115200);
  Serial.println(F("\n====== PLAYGAME Controller Boot ======"));

  main::_ADC::init();
  main::_INP::init();
  main::_PCF::init(); // Will halt here if PCF8575 not found
  main::_JOY::init();
  main::_OLED::init();
  main::_BLE::init();

  // Pre-populate Moving Average with a real reading so % is accurate at boot
  main::_ADC::main();

  Serial.println(F("[CONFIG] All modules initialised. Entering main loop."));
}

} // namespace _CONFIG
} // namespace main