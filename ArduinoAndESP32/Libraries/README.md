# RumiCarライブラリのArduino共通ライブラリ化

RumiCarのArduino向けライブラリです．

現時点ではExercise, Exercise-1.1~3.2に対応しています．Arduino Nano版のCMでは動作確認済み，ESP32版では未確認です．    
上級編のExerciseにも対応を計画中です．

## 導入方法

1. Arduino IDEを開く
1. スケッチ > ライブラリをインクルード > .ZIP形式のライブラリをインストール...
1. 本ディレクトリにある RumiCar.zip を選択する
1. 「ライブラリが追加されました。「ライブラリをインクルード」メニューを確認してください。」と表示されるのを確認する．

## 使用方法

ご自身の.inoファイルの先頭に，
```
#include <RumiCar.h>
```
と記述してください．

スケッチ > ライブラリをインクルード > RumiCar を選択することもできます．


## 注意事項

- ./RumiCar.cpp, ./RumiCar.h を更新した際には，このディレクトリの RumiCar.zip も作り直す必要があることに注意してください．
- [VL53L0X library for Arduino](https://github.com/pololu/vl53l0x-arduino)のインストールは必要です．
- [Arduinoパッケージとして公式に登録＆利用できる](https://lang-ship.com/blog/work/arduino-ide-add-library-manager/)ようになればより嬉しくなりそうですが，有識者の皆さまに期待します^^;

