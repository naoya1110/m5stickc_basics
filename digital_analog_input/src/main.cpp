#include <M5StickC.h>

int DIGITAL_PIN = GPIO_NUM_26;
int ANALOG_PIN = GPIO_NUM_36;
int digital_read;
int analog_read;

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(2);              // フォントサイズ
    M5.Lcd.setTextColor(BLUE);          // フォントカラー
    M5.Lcd.setRotation(3);              // 画面の向き
    Serial.begin(115200);
    pinMode(DIGITAL_PIN, INPUT); // DIGITAL_PIN(26)をデジタル入力に使用
    pinMode(ANALOG_PIN, ANALOG); // ANALOG_PIN(36)をアナログ入力に使用
}

void loop() {
    // 入力読み取り
    digital_read = digitalRead(DIGITAL_PIN);
    analog_read = analogRead(ANALOG_PIN);

    // シリアルプリント
    Serial.printf("digital %d   analog %d\n", digital_read, analog_read);

    // LCDに表示
    M5.Lcd.fillScreen(BLACK);           // LCDを黒で塗りつぶす
    M5.Lcd.setCursor(0, 0);             // カーソルを初期位置に移動
    M5.Lcd.printf("DIGITAL %d\n", digital_read);
    M5.Lcd.printf("ANALOG %d\n", analog_read);
    
    delay(200);
}