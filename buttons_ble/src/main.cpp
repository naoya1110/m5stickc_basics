#include <M5StickC.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

String name = "Naoya's M5StickC"; // デバイス名を設定，適宜変更

void setup() {
    M5.begin();  // M5StickCの初期化
    M5.Lcd.setTextSize(1);              // フォントサイズ
    M5.Lcd.setTextColor(BLUE);          // フォントカラー
    M5.Lcd.setRotation(3);              // 画面の向き
    M5.Lcd.println("Press B btn");      // LCDに表示して改行
    M5.Lcd.println("for 700ms to");
    M5.Lcd.println("clear screen");
    Serial.begin(115200);

    // Bluetooth接続開始
    SerialBT.begin(name); // 接続名を指定して初期化。
    // MACアドレスの取得と表示
    uint8_t macBT[6];
    esp_read_mac(macBT, ESP_MAC_BT);  // MACアドレス取得
    M5.Lcd.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
    Serial.print("BLE mode");
    SerialBT.printf("BLE started\n");
}


void loop() {
    M5.update();  // M5StickCの状態を取得

    if (M5.BtnA.wasReleased()) {            // Aボタンが押されたら
        M5.Lcd.print('A');                  // LCDにAを表示
        Serial.print("A");                  // Aをシリアルプリント
        SerialBT.printf("A\n");             // BLEでAを送信
    }
    else if (M5.BtnB.wasReleased()) {       // Bボタンが押されたら
        M5.Lcd.print('B');                  // LCDにBを表示
        Serial.print("B");                  // Bをシリアルプリント
        SerialBT.printf("B\n");             // BLEでBを送信
    }
    else if (M5.BtnB.wasReleasefor(700)) {  // Bボタンが700ms以上押されたら
        M5.Lcd.fillScreen(BLACK);           // LCDを黒で塗りつぶす
        M5.Lcd.setCursor(0, 0);             // カーソルを初期位置に移動
        Serial.println("\nCleared");        // Clearedをシリアルプリント
        SerialBT.printf("Cleared\n");       // BLEでClearedを送信
    }
}