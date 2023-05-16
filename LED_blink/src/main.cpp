#include <M5StickC.h>

int LED = GPIO_NUM_10;  // LEDのピン番号，赤10 or 赤外9

void setup(){
  M5.begin();
  pinMode(LED, OUTPUT);       // LEDピンを出力に設定
  digitalWrite(LED, HIGH);    // LED OFF (HIGHの時OFF)
}

void loop() {
  digitalWrite(LED, HIGH);    // LED OFF (HIGHの時OFF)
  delay(500);                 // 500ms待機
  
  digitalWrite(LED, LOW);     // LED ON (LOWの時ON)
  delay(500);                 // 500ms待機
}