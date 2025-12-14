#include <Wire.h>
#include "PCF8575.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- CẤU HÌNH PHẦN CỨNG ---

// 1. Cấu hình I2C và PCF8575
#define PCF_ADDRESS 0x20
PCF8575 pcf(PCF_ADDRESS);

// 2. Cấu hình OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 3. Cấu hình JOYSTICK (Theo sơ đồ của bạn)
#define JOY1_X_PIN 15
#define JOY1_Y_PIN 4
#define JOY2_X_PIN 13
#define JOY2_Y_PIN 12

// 4. Cấu hình CÁC NÚT TRỰC TIẾP (Direct Buttons)
#define BTN_L2_PIN   26
#define BTN_R2_PIN   33
#define BTN_L3_PIN   25 // Nút nhấn của Joystick Trái
#define BTN_R3_PIN   32 // Nút nhấn của Joystick Phải
#define BTN_MODE_PIN 27

void setup() {
  Serial.begin(115200);

  // Cấu hình các nút trực tiếp là INPUT (Do mạch đã có trở kéo lên 330 Ohm)
  pinMode(BTN_L2_PIN, INPUT);
  pinMode(BTN_R2_PIN, INPUT);
  pinMode(BTN_L3_PIN, INPUT);
  pinMode(BTN_R3_PIN, INPUT);
  pinMode(BTN_MODE_PIN, INPUT);

  // Khởi động I2C
  Wire.begin();

  // Khởi động PCF8575
  if (!pcf.begin()) {
    Serial.println("LOI: Khong tim thay PCF8575!");
  }

  // Khởi động OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("LOI: Khong tim thay OLED!");
    for(;;);
  }

  // Quay màn hình 180 độ (Sửa lỗi ngược)
  display.setRotation(2); 

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hardware Test");
  display.println("Full Check...");
  display.display();
  delay(1000);
}

void loop() {
  // --- BƯỚC 1: ĐỌC DỮ LIỆU ---

  // 1.1 Đọc Joystick
  int joy1X = analogRead(JOY1_X_PIN);
  int joy1Y = analogRead(JOY1_Y_PIN);
  int joy2X = analogRead(JOY2_X_PIN);
  int joy2Y = analogRead(JOY2_Y_PIN);

  // 1.2 Đọc PCF8575
  uint16_t pcfValues = pcf.read16();

  // 1.3 Đọc nút trực tiếp (Logic 0 là nhấn)
  int bL2 = digitalRead(BTN_L2_PIN);
  int bR2 = digitalRead(BTN_R2_PIN);
  int bL3 = digitalRead(BTN_L3_PIN);
  int bR3 = digitalRead(BTN_R3_PIN);
  int bMode = digitalRead(BTN_MODE_PIN);

  // --- BƯỚC 2: HIỂN THỊ LÊN OLED ---
  display.clearDisplay();

  // Hàng 1: Joystick 1 (Trái)
  display.setCursor(0, 0);
  display.print("J1:"); display.print(joy1X); display.print(","); display.print(joy1Y);

  // Hàng 2: Joystick 2 (Phải)
  display.setCursor(0, 10);
  display.print("J2:"); display.print(joy2X); display.print(","); display.print(joy2Y);

  // Hàng 3: Các nút trực tiếp (L2, R2, L3, R3, Mode)
  display.setCursor(0, 20);
  if (bL2 == LOW) display.print("L2 ");
  if (bR2 == LOW) display.print("R2 ");
  if (bL3 == LOW) display.print("L3 ");
  if (bR3 == LOW) display.print("R3 ");
  if (bMode == LOW) display.print("MODE");

  // Hàng 4: Các nút PCF8575 (P0 - P15)
  display.setCursor(0, 35);
  display.println("PCF Buttons:");
  display.setCursor(0, 45);
  
  bool anyPcf = false;
  for (int i = 0; i < 16; i++) {
    if ( !((pcfValues >> i) & 1) ) { // Nếu bit là 0 (Nhấn)
      display.print("P"); display.print(i); display.print(" ");
      anyPcf = true;
    }
  }
  if (!anyPcf) display.print("-");

  display.display();
  delay(50); // Delay nhỏ để màn hình mượt hơn
}