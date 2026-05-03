# RumiCarライブラリのArduino共通ライブラリ化

> [!IMPORTANT]
> ## 📢 重要なお知らせ / Important Notice
>
> **RumiCarライブラリはArduino公式のLibrary Managerに登録されました．**  
> 今後は**Library Manager経由でのインストールを推奨**します．リポジトリ: [RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib)
>
> このディレクトリにある `RumiCar.zip` および `RumiCar/` フォルダ内のローカル版ライブラリは，**将来削除される予定**です．
>
> ---
>
> **The RumiCar library has been published to the Arduino Library Manager.**  
> Please use Library Manager-based installation. Repository: [RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib).
> The `RumiCar.zip` and `RumiCar/` folder in this directory are scheduled for removal in the future.

> [!NOTE]
> このREADMEには以前，「[Arduinoパッケージとして公式に登録＆利用できる](https://lang-ship.com/blog/work/arduino-ide-add-library-manager/)ようになればより嬉しくなりそうですが，有識者の皆さまに期待します^^;」と記載されていました．**2026年5月，その願いが実現しました．** 🎉

RumiCarのArduino向けライブラリです．

現時点ではExercise, Exercise-1.1~3.2に対応しています．Arduino Nano版のCMでは動作確認済み，ESP32版では未確認です．    
上級編のExerciseにも対応を計画中です．

## 推奨される導入方法 (Library Manager経由)

1. Arduino IDEを開く
1. メニューの「スケッチ」→「ライブラリをインクルード」→「ライブラリを管理...」を選択
1. 検索ボックスに `RumiCar` と入力
1. RumiCar by RumiCar-group を選択して「インストール」をクリック

## 旧来の導入方法 (.ZIPライブラリ，将来削除予定)

> [!WARNING]
> この方法は将来削除される予定です．新規の利用者は上記の「推奨される導入方法」をご利用ください．

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

- [VL53L0X library for Arduino](https://github.com/pololu/vl53l0x-arduino)のインストールは必要です．Library Manager経由でインストールする場合は，依存関係として自動的にインストールするか確認されます．
