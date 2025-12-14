/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include "lib.h"

namespace main{
  namespace _CONFIG{
    void init(){
      Serial.begin(115200);
      main::_ADC::init();
      main::_INP::init();
      main::_PCF::init();
      main::_JOY::init();
      main::_OLED::init();
      //main::_OLED::init2();
      main::_BLE::init();
      main::_ADC::main();
    }
  }
}