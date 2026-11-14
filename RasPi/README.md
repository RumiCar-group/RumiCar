# RumiCar (Raspberry Pi 版)

Raspberry Pi Zero W のコンピュータモジュール（CM）で RumiCar を動かす Python のライブラリ（`RumiCar.py`）とサンプルプログラム（`Exercise-*`）です。
関数名・定数・値の意味・戻り値は Arduino 版のライブラリ [RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib) の v0.6.0 と同じです（`RumiCar.__version__` は `0.6.0`）。
CM を Arduino から Raspberry Pi に変えても、`RC_drive(FORWARD, 150)` が「全開の約 6 割（150/255）の信号を駆動モーターへ送る」という意味は変わりません。
プログラムは Arduino と同じく `setup()` と `loop()` で書き、`python3 RumiCar.py プログラム.py` で動かします（[動かし方](#動かし方)）。

## Setup

If you don't have RasPi monitor and keyboard, please check [Remote Access](docs/RemoteAccess.md).

### SMBus (I2C)
Used for VL53L0X.
```
sudo apt install python3-smbus
sudo raspi-config
# Interface Options -> I2C -> Yes
```

### GPIO
* [pigpio library](http://abyz.me.uk/rpi/pigpio/index.html) 使用
* [pinout](https://pinout.xyz/)

```
sudo apt-get update
sudo apt-get install pigpio python3-pigpio  # python-pigpio - python2 deprecated
sudo systemctl enable --now pigpiod
```

以下主要なGPIOをライブラリサイトより一部抜粋。

| PIN | Note  | GPIO |
| --- | ----- | ---- |
| 1   | 3V3   |      |
| 2   | 5V    |      |
| 3   | SDA   | 2    |
| 4   | 5V    |      |
| 5   | SCL   | 3    |
| 6   | GND   |      |
| 9   | GND   |      |
| 11  | AIN1  | 17   |
| 12  | BIN1  | 18   |
| 13  | AIN2  | 27   |
| 14  | GND   |      |
| 15  | SERVO | 22   |
| 16  | SHDN0 | 23   |
| 17  | 3V3   |      |
| 18  | SHDN1 | 24   |
| 20  | GND   |      |
| 22  | SHDN2 | 25   |
| 25  | GND   |      |
| 30  | GND   |      |
| 33  | BIN2  | 13   |
| 34  | GND   |      |
| 37  | SERVO | 26   |
| 39  | GND   |      |

## API

```python
from RumiCar import *

RC_setup()                  # センサーとモーターの初期化。setup() で 1 回呼ぶ
RC_steer(direc)             # 操舵: LEFT / CENTER / RIGHT（いちばん強く切る）            -> 1（引数が不正なら 0）
RC_steer(direc, ipwm)       # 操舵の強さを付けて: ipwm は 0〜255（下表）                  -> 1（引数が不正なら 0）
RC_drive(direc, ipwm)       # 走行: FREE / REVERSE / FORWARD / BRAKE、ipwm は 0〜255    -> 1（引数が不正なら 0）
RC_read(direc)              # 測距: LEFT / CENTER / RIGHT / REAR                     -> 下表
RC_params()                 # プログラムと同じフォルダの params.json の内容（無ければ {}）
RC_state(name)              # 今の処理状態の名前（英数字と _ で 24 文字まで）。変わったときに表示
```

| `RC_steer()` の強さ `ipwm` | 出力 |
|---|---|
| `255`（省略したとき） | いちばん強く切る。以前の `RC_steer(direc)` とまったく同じ出力 |
| `1`〜`254` | スローデケイ：`RIGHT` は AIN1 を HIGH にして AIN2 に `255 - ipwm`、`LEFT` はその逆 |
| `0` | `CENTER` と同じ（両入力 0、中立） |

* 強さは**信号の割合**（0〜255、補正なし）で、範囲外の値は 0〜255 に収めます。`CENTER` のときは使いません。戻り値は 1（`LEFT` / `CENTER` / `RIGHT`）か 0（それ以外の方向）で、以前と同じです。
* ハンドルはサーボではなく DC モーターなので、同じ強さでもどれだけ曲がるかは車体や電池で変わります。強さは「3 割増し」「半分」のような**目安**として使い、走らせながら反応を見て決めてください（Exercise-2.5 / 3.3）。
* 操舵の PWM は pigpio のソフトウェア PWM（500 Hz）です。駆動は GPIO 18 / 13 のハードウェア PWM（960 Hz）です。周波数は CM ごとに違ってかまいません（値の意味は同じです）。

| `RC_read()` の戻り値 | 意味 |
|---|---|
| `0`〜`200` | 距離（mm） |
| `-1` | タイムアウト（センサーが応答しない）。`REAR` では後方センサーが無い車両も `-1` |
| `-2` | 引数が不正（LEFT / CENTER / RIGHT / REAR 以外） |
| `-3` | 範囲外：20 cm より先には何もない（反射が弱くて測れない場合を含む） |

* RumiCar の搭載位置では ToF のレーザーが 20 cm より先で地面を捉えることがあるため、200 mm を超える値は信用せず `-3` にします。`-3` は「20 cm 以内に何もない」の意味で、走行のロジックでは**通常走行**（開放）として扱います。
* `-3` は数としては最も小さいので、`if d < 100:` にそのまま使うと「近い」と判定されます。比較の前に `if d == -3: d = 999` のように「遠い」へ置き換えてください（Exercise-3.1 / 3.2 を参照。左右の差を使う Exercise-3.3 は Arduino 版と同じく 250 に置き換えます）。`-1`（読めない）は `-3` と区別します。
* `RC_drive()` の値は**信号の割合**です。150 なら 150/255（約 59 %）のデューティの PWM を出します。CM によって PWM の周波数やモーターの特性が異なるため、同じ値でも速さは同じとは限りません。駆動はスローデケイで、停止や逆方向から走り出すときは 200/255 を 20 ms 出してから目標の値にします（RumiCar-lib と同じ）。`BRAKE` は両入力 HIGH の全ブレーキ（値は無視）、`FREE` は惰行です。
* 方向に `True` / `False` は使えません（不正な引数として扱います）。強さや速さに数でない値を渡したときは 0 として扱います。不正な引数は、そのつど止まらずに最初の 1 回だけ警告を表示します（Zero2Cam と同じ扱い）。
* 後方の VL53L0X（任意搭載。XSHUT 無しで I2C に直結）は `RC_setup()` が自動で検出し、`RC_read(REAR)` で読めます。無い車両では `-1` が返るので、同じプログラムがどちらの車両でもそのまま動きます。
* `RC_setup()` が終了時（Ctrl+C やプログラムのエラーを含む）にモーターを止める処理を登録するので、プログラムに後始末は要りません。`RC_setup()` より前に `RC_steer()` などを呼ぶとエラーで止まります。
* 実行には pigpiod（上記）が必要です。起動していなければ `RC_setup()` がエラーで止まります。

## 動かし方

プログラムは Arduino のスケッチと同じく、最初に 1 回だけ呼ばれる `setup()` と、くり返し呼ばれる `loop()` で書きます。

```python
from RumiCar import *

def setup():
    RC_setup()

def loop():
    d = RC_read(CENTER)
    ...
```

このファイルを `RumiCar.py` に渡して動かします。`RumiCar.py` が `setup()` を 1 回呼び、そのあと `loop()` を **50 ms ごと（1 秒に 20 回）** に呼びます。止めるときは Ctrl+C です。

```
cd ~/RasPi/Exercise-1.1
python3 ../RumiCar.py Exercise-1.1.py
```

* 周期は [RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam) の走行プログラム（`drive.py`）と同じなので、Zero2Cam 用やシミュレータ用に `setup()` / `loop()` で書いた Python のプログラムも、書き換えずに `python3 RumiCar.py drive.py` で動きます（回数で時間を数えるプログラムも同じ時間になります）。
* `loop()` の中で `time.sleep()` を使ってもかまいません（Exercise-2.x）。`loop()` が 50 ms より長くかかったときは、待たずに次の `loop()` を呼びます。
* `RC_params()` は、渡したプログラムと同じフォルダの `params.json` を読みます。プログラム名のあとに書いた引数は、プログラムの `sys.argv` に入ります。
* `setup()` は無くてもかまいません（`RC_setup()` を呼ぶ場所が無くなるので、モーターやセンサーを使うときは必要です）。`loop()` が無いとエラーになります。
* `misc/` のスクリプトは `while True:` を自分で書く形のままで、`python3 azure_d2c.py` のように直接動かします（`sys.path.append('..')` で `RumiCar.py` を読み込みます）。

### ライブラリの構成

`RumiCar.py` は 2 つの層に分かれています。

* **API の層**：`RC_*` の関数・定数・`setup()` / `loop()` の実行。引数を確かめて 0〜255 に収め、どの環境でも同じ戻り値を返します。
* **ハードウェアの層**：実際の出力。Raspberry Pi Zero W の車体を pigpio で動かす層（pigpio と VL53L0X はこの層の中で読み込みます）と、RumiCar-Zero2Cam 用の層（Zero2Cam ではモーターとセンサーは本体のプログラムが持つため、1 フレームごとに値を受け渡すだけ）があります。

このため `RumiCar.py` は pigpio が入っていない環境でも読み込めます。Zero2Cam 用の層は、RumiCar-Zero2Cam がこの `RumiCar.py` を自分で同等品を持たずにそのまま取り込んで使うためのものです。

## Exercises

Arduino 版（RumiCar-lib のスケッチ例）と同じ構成で、どれも `setup()` / `loop()` の形です。Exercise-1 はセンサーを読む、Exercise-2 はモーターを動かす、Exercise-3 はセンサーの値で走る、です。

```
cd ~/RasPi/Exercise-1.1
python3 ../RumiCar.py Exercise-1.1.py

cd ~/RasPi/Exercise-2.5
python3 ../RumiCar.py Exercise-2.5.py

# ...
```

| Exercise | 内容 |
|---|---|
| `Exercise-1.1` | 前方のセンサーの距離を表示する |
| `Exercise-1.2` | 3 つのセンサーの距離を 1 行に表示する |
| `Exercise-2.1` | 右・左に交互に操舵する |
| `Exercise-2.2` | 3 段階の速さ（255 / 200 / 150）で前進する |
| `Exercise-2.3` | 前進と後退を繰り返す |
| `Exercise-2.4` | 走行と操舵を組み合わせてジグザグに走る |
| `Exercise-2.5` | ハンドルの強さを変えてみよう：強さ 255（いちばん強い）と 128（半分）で右・左に切る |
| `Exercise-3.1` | 安全に止まる：前方のセンサーの距離で走行を決める |
| `Exercise-3.2` | 道なりに走る：3 つのセンサーで速さと向きを決める |
| `Exercise-3.3` | なめらかに走る車：3.2 の「左右どちらかに必ず切る」の代わりに、左右の差が 30 mm 以内ならまっすぐ、それより大きければ広い側へ切る。ハンドルの強さは 128 から始め、真ん中に近づかなければ 1.3 倍、真ん中を越えたら半分にする（40〜255）。強さは `Power` として表示 |

`misc/` には Azure IoT Hub とカメラ（OpenCV）の実験用スクリプトがあります（同じライブラリを使います）。

## Video Streaming
To enable camera, edit `/boot/firmware/config.txt` (or `/boot/config.txt`):
```
camera_auto_detect=0
start_x=1
```

Check camera specs:
```
sudo apt install v4l-utils
v4l2-ctl --all
```

On robot:
```
sudo apt-get install gstreamer1.0-tools gstreamer1.0-plugins-good

gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw, format=YUY2, width=640, height=480, framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink host=<ubuntu>.local port=1234
```
, where the `<ubuntu>` is hostname of your PC from the same LAN.

On PC:
```
gst-launch-1.0 -v udpsrc port=1234 ! "application/x-rtp, payload=26" ! rtpjpegdepay ! jpegdec ! autovideosink sync=false
```

General camera info:
* https://www.raspberrypi.com/documentation/accessories/camera.html
