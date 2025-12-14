/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include "lib.h"

namespace main{
  namespace _JOY{
    int value1;
    int value2;
    int value3;
    int value4;

    void init(){
      pinMode( L_VRY_JOYSTICK,INPUT );
      pinMode( L_VRX_JOYSTICK,INPUT );
      pinMode( R_VRY_JOYSTICK,INPUT );
      pinMode( R_VRX_JOYSTICK,INPUT );
    }

    void main(){
      value1 = map(analogRead(L_VRY_JOYSTICK), 0, 4095, 32737, 0);
      value2 = map(analogRead(L_VRX_JOYSTICK), 0, 4095, 0, 32737);

      value3 = map(analogRead(R_VRY_JOYSTICK), 0, 4095, 32737, 0);
      value4 = map(analogRead(R_VRX_JOYSTICK), 0, 4095, 0, 32737);     
    }
  }
}