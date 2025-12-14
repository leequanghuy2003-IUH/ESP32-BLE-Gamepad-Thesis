/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/**************************************  LE QUANG HUY  *******************************************/
/**********************************  NGUYEN LE THANH HIEU  ***************************************/

#include "lib.h"

namespace main{
  namespace _ACTION{
    String status_BLE = "DISCONNECT";
    int switchState[17];
    int oldswitchState[17];
    unsigned long millis_ADC;
    unsigned long millis_button;
    unsigned long millis_button1;
    unsigned long millis_button2;

    void main(){
      // 1. LOGIC PIN: Cập nhật mỗi 5 giây để nhanh hơn
      if ( millis() - millis_ADC >= 5000 ){
        main::_ADC::main(); // Đọc lại giá trị pin mới nhất
        
        if (main::_BLE::Bluetooth_Game.isConnected()) {
           main::_BLE::Bluetooth_Game.setBatteryLevel(main::_ADC::PIN_percent);
        }
        millis_ADC = millis();
      }

      // 2. OLED: Hiển thị trạng thái và Pin
      main::_OLED::status(status_BLE);
      // Quan trọng: Truyền PIN_percent mới nhất vào hàm vẽ OLED
      main::_OLED::main(main::_ADC::PIN_percent);

      if (main::_BLE::Bluetooth_Game.isConnected()) {
        // 3. XỬ LÝ KHI VỪA KẾT NỐI (Gửi pin ngay lập tức)
        if (status_BLE == "DISCONNECT") { 
           status_BLE = "CONNECT";
           main::_ADC::main(); // Đọc pin ngay
           main::_BLE::Bluetooth_Game.setBatteryLevel(main::_ADC::PIN_percent); // Gửi ngay
        }

        main::_JOY::main();
        main::_BLE::Bluetooth_Game.setLeftThumb(main::_JOY::value2, main::_JOY::value1);
        main::_BLE::Bluetooth_Game.setRightThumb(main::_JOY::value4, main::_JOY::value3);

        if (millis() - millis_button > 20) {
          millis_button = millis();
          for (int i = 0; i < 8; i++) {
            switchState[i] = main::_PCF::pcf8575.digitalRead(main::_PCF::buttonPins[i]);
          }
        }
        
        for (int i = 0; i < 8; i++) {
          if (switchState[i] == LOW && oldswitchState[i] == HIGH) {
            main::_BLE::Bluetooth_Game.press(main::_BLE::buttons[i]);
          }
          else if (switchState[i] == HIGH && oldswitchState[i] == LOW) {
            main::_BLE::Bluetooth_Game.release(main::_BLE::buttons[i]);
          }
          oldswitchState[i] = switchState[i];
        }

        if (millis() - millis_button1 > 20) {
          millis_button1 = millis();
          for (int j = 0; j < 4; j++) {
            switchState[j+8] = main::_PCF::pcf8575.digitalRead(main::_PCF::buttonPins[j+8]);
          }
        }
        
        for (int j = 0; j < 4; j ++) {
          if (switchState[j+8] == LOW && oldswitchState[j+8] == HIGH) {
            main::_BLE::Bluetooth_Game.setHat1(main::_BLE::buttons_S[j]);
          }
          else if (switchState[j+8] == HIGH && oldswitchState[j+8] == LOW) {
            main::_BLE::Bluetooth_Game.setHat1();
          }
          oldswitchState[j+8] = switchState[j+8];
        }

        for (int k = 0; k < 5; k ++) {
          if (!digitalRead(main::_INP::buttonPins_bonus[k])){
            main::_BLE::Bluetooth_Game.press(main::_BLE::buttons[k + 8]);
          }
          else {
            main::_BLE::Bluetooth_Game.release(main::_BLE::buttons[k+8]);
          }
        }        
      }
      else {
        status_BLE = "DISCONNECT";
      }
    }  
  }
}
