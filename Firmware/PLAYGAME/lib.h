/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/************************************  TEN SINH VIEN 1   *****************************************/
/************************************  TEN SINH VIEN 2   *****************************************/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BleGamepad.h>
#include <PCF8575.h>
#include <string.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define NUM_BUTTONS 12

#define L_VRY_JOYSTICK 15
#define L_VRX_JOYSTICK 4

#define R_VRY_JOYSTICK 13
#define R_VRX_JOYSTICK 12

namespace main{
  namespace _ACTION{
    extern String status_BLE;
    extern unsigned long millis_ADC;
    void main();
  }

  namespace _CONFIG{
    void init();
  }

  namespace _BLE{
    extern BleGamepad Bluetooth_Game;
    extern int buttons[13];
    extern int buttons_S[4];
    void init();
    void main();
  }

  namespace _JOY{
    extern int value1;
    extern int value2;
    extern int value3;
    extern int value4;
    void init();
    void main();
  }

  namespace _OLED{
    void init();
    void init2();
    void status(String text);
    void main(int percent);
    void testdrawroundrect();
    void drawBatteryIndicator(int percentage);
  }

  namespace _PCF{
    extern int buttonPins[12];
    extern PCF8575 pcf8575;
    void init();
    void main();
  }

  namespace _INP{
    extern int buttonPins_bonus[5];
    void init();
    void main();
  }

  namespace _ADC{
    extern int PIN_percent;
    void init();
    void main();
  }
}