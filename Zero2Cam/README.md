# Raspberry Pi Zero 2 W ＋ カメラ版（RumiCar Zero2Cam）

[English](README.en.md) | **日本語**

Raspberry Pi Zero 2 W と Camera Module 2 を搭載した RumiCar のプログラムは、別リポジトリ **[RumiCar-group/RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam)** にあります。このフォルダは案内だけで、プログラムは入っていません。

| | |
|---|---|
| プログラムと使い方 | [RumiCar-Zero2Cam の README](https://github.com/RumiCar-group/RumiCar-Zero2Cam#readme)（日本語） |
| 配布物（SD カードイメージ・PC アプリ） | [Releases](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) |
| 仕様書 | [docs/RumiCar-Zero2Cam 仕様書.docx](https://github.com/RumiCar-group/RumiCar-Zero2Cam/tree/main/docs) |

## どんなものか

- 電源を入れるとすぐ走り出す自動運転です（前方 3 つの ToF センサーで壁や障害物を避けます。Arduino 版と同じ使い方）。
- 車が Wi-Fi のアクセスポイントになります。PC を車の Wi-Fi につなぎ、走行管理アプリ（Windows）で状態と車載カメラの画像を見る、自動／手動／収録のみを切り替える、非常停止、矢印キーでの手動運転ができます。
- 受信した画像と状態は PC に収録されます。収録フォルダを Claude のデスクトップアプリに読ませて判断データを改良し、走行管理アプリから車に送って切り替えられます（車を止めずに更新できます）。
- プログラムのビルドは要りません。SD カードイメージを書き込むだけで動きます。

## 用意するもの

| もの | 備考 |
|---|---|
| RumiCar の車体 ＋ Raspberry Pi Zero 2 W のコンピュータモジュール | 配線は **ラズパイ Zero W 版 CM と同じ** です（[開発用資料](../%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99)の「結線図(Raspberry Pi Zero W).png」、[RasPi/README.md](../RasPi/README.md) のピン表）。操舵 GPIO 17／27、駆動 GPIO 18／13（ハードウェア PWM）、ToF の XSHUT GPIO 23／24／25、I2C |
| Camera Module 2 | Zero 2 W のカメラコネクタに接続（Zero 用の細いフラットケーブル） |
| microSD カード | 8 GB 以上 |
| Windows PC（64bit、Windows 10/11、Wi-Fi 付き） | 走行管理アプリを動かします |

## 始め方

1. [Releases](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) の最新版から `rumicar-zero2cam-<版>-<日付>.img.xz`（SD カードイメージ）と `RumiCarDriveManager-<版>-win-x64.zip`（PC アプリ）をダウンロードします。
2. [Raspberry Pi Imager](https://www.raspberrypi.com/software/) で `.img.xz` を SD カードに書き込みます（「OS customisation settings を適用しますか」は **いいえ**。設定はイメージに入っています）。
3. 車を台に載せて電源を入れます。初回は 2〜3 分で自動再起動し、そのあと走り出します。
4. PC の Wi-Fi で `RumiCar`（パスワードなし）に接続し、zip を展開して `RumiCarDriveManager.exe` を起動、「接続」を押します。

詳しい手順と画面の説明、トラブルシューティングは [RumiCar-Zero2Cam の README](https://github.com/RumiCar-group/RumiCar-Zero2Cam#readme) にあります。

## 他のコンピュータモジュールとの違い

| | Arduino Nano／ESP32 | Raspberry Pi Zero W（[RasPi](../RasPi) フォルダ） | **Zero 2 W ＋ カメラ（Zero2Cam）** |
|---|---|---|---|
| 言語 | C++（Arduino IDE） | Python | C++（車載）／C#（PC の走行管理アプリ） |
| プログラムの入れ方 | USB で書き込み | SD カードの OS に Python を置く | SD カードイメージを書くだけ（ビルド不要）。走行ロジックは PC から送って差し替え |
| カメラ | なし | 任意（映像の配信例） | 標準（320×240、約 20 コマ/秒を PC に送信・収録） |
| PC との接続 | USB シリアル／Bluetooth | SSH | Wi-Fi（車がアクセスポイント） |
| 配線 | Arduino 版の結線図 | Zero W 版の結線図 | Zero W 版と同じ ＋ カメラ |

Issue・プルリクエストは [RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam/issues) へお願いします（日本語・英語どちらでも）。
