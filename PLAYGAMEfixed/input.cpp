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
namespace _INP {

// const → stored in Flash, readable from RAM, cannot be accidentally modified
const int buttonPins_bonus[5] = {26, 33, 27, 25, 32};

void init() {
  for (int i = 0; i < NUM_BONUS_BUTTONS; i++) {
    pinMode(buttonPins_bonus[i], INPUT_PULLUP);
  }
}

} // namespace _INP
} // namespace main
