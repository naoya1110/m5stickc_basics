#include <M5StickC.h>

const int N = 4000;
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float accX_list[N];
float accY_list[N];
float accZ_list[N];
unsigned long t_list[N];

unsigned long t_old = 0;
unsigned long t_now = 0;
unsigned long t_start = 0;
unsigned long dt = 0;



// float gyroX = 0.0F;
// float gyroY = 0.0F;
// float gyroZ = 0.0F;


void setup() {
    M5.begin();             // M5stickCの初期化
    M5.IMU.Init();          // IMUセンサの初期化
    M5.Mpu6886.SetAccelFsr(M5.Mpu6886.AFS_2G);
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextFont(4);
    M5.Lcd.setTextColor(BLUE);
    Serial.begin(115200);   // シリアル通信開始
    t_old = micros();
}


void loop() {

    M5.Lcd.fillScreen(BLACK);
    // M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Measuring");
    M5.Lcd.println("Acc");
    
    t_start = micros();

    for(int i=0; i<N; i++){
        // IMUのデータを取得
        // M5.IMU.getAccelData(&accX, &accY, &accZ);       // 加速度
        M5.Mpu6886.getAccelData(&accX,&accY,&accZ);
        // M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);     // 角速度
        t_now = micros();
        dt = t_now - t_start;
        
        
        accX_list[i]=accX;
        accY_list[i]=accY;
        accZ_list[i]=accZ;
        t_list[i]=dt;


    }




    // データをLCDに表示
    // M5.Lcd.fillScreen(BLACK);
    // M5.Lcd.setCursor(40, 0);
    // M5.Lcd.println("IMU TEST");
    // M5.Lcd.setCursor(0, 10);
    // M5.Lcd.println("   X       Y       Z");
    // M5.Lcd.setCursor(0, 20);
    // M5.Lcd.printf(" %5.2f   %5.2f   %5.2f G\n", accX, accY, accZ);
    // M5.Lcd.printf("%6.2f  %6.2f  %6.2f o/s\n", gyroX, gyroY, gyroZ);


    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Sending");
    M5.Lcd.println("Data");

    Serial.println("start");
    
    for(int i=0; i<N; i++){

        Serial.printf("%d %6.5f %6.5f %6.5f\n", 
                        t_list[i],
                        accX_list[i],
                        accY_list[i],
                        accZ_list[i]);
    }
    Serial.println("end");

    M5.Lcd.fillScreen(BLACK);
    // M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("MPU6886");
    M5.Lcd.println("+-2G");


    delay(3000);
}