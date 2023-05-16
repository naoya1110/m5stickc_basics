#include <M5StickC.h>

int PIN_G0 = GPIO_NUM_0;

void setup() {
    M5.begin();  // M5StickCの初期化
    pinMode(PIN_G0, OUTPUT);    // PIN_G0を出力に設定

}


void loop() {
  digitalWrite(PIN_G0, HIGH);   // PIN_G0に3.3Vを出力
  delay(500);                   // 500ms待機
  
  digitalWrite(PIN_G0, LOW);    // PIN_G0に0Vを出力
  delay(500);                   // 500ms待機
}