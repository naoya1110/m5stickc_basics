#include <M5StickC.h>

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;


void setup() {
    M5.begin();             // M5stickCの初期化
    M5.IMU.Init();          // IMUセンサの初期化
    M5.Lcd.setRotation(3);
    Serial.begin(115200);   // シリアル通信開始
}


void loop() {
    // IMUのデータを取得
    M5.IMU.getAccelData(&accX, &accY, &accZ);       // 加速度
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);     // 角速度

    // データをLCDに表示
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(40, 0);
    M5.Lcd.println("IMU TEST");
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.println("   X       Y       Z");
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf(" %5.2f   %5.2f   %5.2f G\n", accX, accY, accZ);
    M5.Lcd.printf("%6.2f  %6.2f  %6.2f o/s\n", gyroX, gyroY, gyroZ);

    // データをシリアルプリント
    Serial.printf("%5.2f %5.2f %5.2f %6.2f %6.2f %6.2f\n", accX, accY, accZ, gyroX, gyroY, gyroZ);

    delay(100);
}