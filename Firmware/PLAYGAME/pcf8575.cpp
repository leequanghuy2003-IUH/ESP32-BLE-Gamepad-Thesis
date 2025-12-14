/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include "lib.h"

namespace main{
  namespace _PCF{
    PCF8575 pcf8575(0x20);
    int buttonPins[12] = { P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11 };

    void init(){
      for (int i = 0; i< 12; i++) {
        pcf8575.pinMode(buttonPins[i], INPUT);
      }
      pcf8575.begin();
    }

    void main(){
      
    }
  }
}