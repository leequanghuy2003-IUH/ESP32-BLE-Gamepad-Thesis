/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include "lib.h"

namespace main{
  namespace _BLE{
    int buttons[13] = { BUTTON_1, BUTTON_2, BUTTON_4, BUTTON_5, BUTTON_7, BUTTON_8, BUTTON_11, BUTTON_12, BUTTON_9, BUTTON_10, BUTTON_13, BUTTON_14, BUTTON_15};
    int buttons_S[4] = { DPAD_UP, DPAD_LEFT, DPAD_DOWN, DPAD_RIGHT };
    BleGamepad Bluetooth_Game ("DATN", "Do An", 100);

    void init(){
      Bluetooth_Game.begin();
    }

    void main(){

    }
  }
}