/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include "lib.h"

namespace main{
  namespace _INP{
    int buttonPins_bonus[5] = { 26, 33, 27, 25, 32};
    void init(){
      for (int i  = 0; i < 5; i++ ) {
        pinMode(buttonPins_bonus[i], INPUT_PULLUP);
      }
    }

    void main(){

    }
  }
}
