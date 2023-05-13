#include <M5StickC.h>


void setup() {
    M5.begin();  // M5StickCの初期化
    M5.Lcd.setTextSize(2);              // フォントサイズ
    M5.Lcd.setTextColor(BLUE);          // フォントカラー
    M5.Lcd.setRotation(3);              // 画面の向き
    M5.Lcd.println("Press B btn");      // LCDに表示して改行
    M5.Lcd.println("for 700ms to");
    M5.Lcd.println("clear screen");
}

void loop() {
    M5.update();  // M5StickCの状態を取得

    if (M5.BtnA.wasReleased()) {            // Aボタンが押されたら
        M5.Lcd.print('A');                  // LCDにAを表示
    }
    else if (M5.BtnB.wasReleased()) {       // Bボタンが押されたら
        M5.Lcd.print('B');                  // LCDにBを表示
    }
    else if (M5.BtnB.wasReleasefor(700)) {  // Bボタンが700ms以上押されたら
        M5.Lcd.fillScreen(BLACK);           // LCDを黒で塗りつぶす
        M5.Lcd.setCursor(0, 0);             // カーソルを初期位置に移動
    }
}