#include <M5StickC.h>

int LED = GPIO_NUM_10;
int PWMCH = 0;

void setup() {
  M5.begin();
  pinMode(LED, OUTPUT);
  ledcSetup(PWMCH, 1000, 8);  // PWMの周波数を1kHz，分解能を8ビット（256段階）に設定
  ledcAttachPin(LED, PWMCH);
}

void loop() {
  ledcWrite(PWMCH, 0);        //    0%(0.00V)
  delay(500);
  ledcWrite(PWMCH, 64);       //  25%(0.83V)
  delay(500);
  ledcWrite(PWMCH, 128);      //  50%(1.65V)
  delay(500);
  ledcWrite(PWMCH, 192);      //  75%(2.48V)
  delay(500);
  ledcWrite(PWMCH, 256);      // 100%(3.3V)
  delay(500);
}