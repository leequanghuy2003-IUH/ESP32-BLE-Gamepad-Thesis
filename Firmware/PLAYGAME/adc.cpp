/************************************  DO AN TOT NGHIEP  *****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH *********************************/
/**************************************  LE QUANG HUY  *******************************************/
/**********************************  NGUYEN LE THANH HIEU  ***************************************/

#include "lib.h"

namespace main{
  namespace _ADC{  // Child space of Main space, this space declare ADC
    int ADC_value;  // Variable save ADC value
    int voltage;  // Variable save volt change from ADC value
    int PIN_percent;  // Variable save percent of Battery
    
    // Biến lưu trữ cho bộ lọc trung bình
    const int NUM_SAMPLES = 20; // Số mẫu để lấy trung bình (tăng lên để mượt hơn)
    int sampleIndex = 0;
    float samples[NUM_SAMPLES];
    float total = 0;
    float average = 0;
    bool firstRun = true;

    void init(){  // Void set up of ADC
      pinMode(14,INPUT);  // Set up Pin of ADC
      // Khởi tạo mảng mẫu
      for (int i = 0; i < NUM_SAMPLES; i++) {
        samples[i] = 0;
      }
    }

    void main(){  // Void main of ADC
      int ADC = analogRead(14);  // Read ADC value
      
      // Chuyển đổi sang Volt (3.3V reference, 12-bit resolution 4095)
      // Tuy nhiên thực tế ESP32 ADC không tuyến tính hoàn toàn, hệ số có thể cần chỉnh
      // Giả sử mạch phân áp của bạn chuẩn
      float volt = ADC * 3.3 / 4095.0; 

      // --- BỘ LỌC TRUNG BÌNH TRƯỢT (MOVING AVERAGE) ---
      // Nếu chạy lần đầu, điền đầy mảng bằng giá trị hiện tại để tránh số nhảy từ 0 lên từ từ
      if (firstRun) {
        for(int i=0; i<NUM_SAMPLES; i++) samples[i] = volt;
        total = volt * NUM_SAMPLES;
        firstRun = false;
      }

      // Trừ đi mẫu cũ nhất
      total = total - samples[sampleIndex];
      // Lưu mẫu mới
      samples[sampleIndex] = volt;
      // Cộng mẫu mới vào tổng
      total = total + samples[sampleIndex];
      // Tăng chỉ số index (vòng tròn)
      sampleIndex = (sampleIndex + 1);
      if (sampleIndex >= NUM_SAMPLES) {
        sampleIndex = 0;
      }

      // Tính trung bình
      average = total / NUM_SAMPLES;
      
      // --- TÍNH PHẦN TRĂM TỪ GIÁ TRỊ TRUNG BÌNH ---
      // Sử dụng giá trị 'average' (đã lọc) thay vì 'volt' (nhiễu)
      float stableVolt = average;

      // Chặn dưới (1.9V là 0%)
      if (stableVolt < 1.9) stableVolt = 1.9; 
      
      // Tính phần trăm: (Volt hiện tại - 1.9) / (3.1 - 1.9) * 100
      // 3.1 - 1.9 = 1.2 (Khoảng dao động)
      int rawPercent = (int)((stableVolt - 1.90) * 100 / 1.2);
      
      // Chặn trên dưới
      if (rawPercent > 100) rawPercent = 100;
      if (rawPercent < 0) rawPercent = 0;

      // --- HYSTERESIS (CHỐNG NHẢY SỐ NHỎ) ---
      // Chỉ cập nhật PIN_percent nếu thay đổi >= 1% hoặc nếu là lần đầu
      // Tuy nhiên với bộ lọc trung bình tốt, có thể gán trực tiếp
      PIN_percent = rawPercent;
      
      // Debug
      // Serial.print("Raw: "); Serial.print(volt); 
      // Serial.print(" - Avg: "); Serial.print(stableVolt);
      // Serial.print(" - %: "); Serial.println(PIN_percent);
    }
  }
}