# RumiCar (Raspberry Pi 版)

Raspberry Pi Zero W のコンピュータモジュール（CM）で RumiCar を動かす Python のライブラリ（`RumiCar.py`）とサンプルプログラム（`Exercise-*`）です。
関数名・定数・値の意味・戻り値は Arduino 版のライブラリ [RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib) の v0.7.0 と同じです（`RumiCar.__version__` は `0.7.0`）。サンプルプログラムでの `RC_read()` の値の扱い（判定の順序、`LIMIT_MM`・`FAR_MM`）は、RumiCar-lib v0.7.1 のスケッチ例と同じです（v0.7.1 はスケッチ例と文書だけの変更で、API は v0.7.0 と同じです）。
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
RC_setReadLimit(limit_mm)   # RC_read の上限 [mm] を全方向に（1〜2000、0 = 上限なし）       -> 1
RC_setReadLimit(direc, limit_mm)  # 上限をその方向だけに: LEFT / CENTER / RIGHT / REAR  -> 1（方向が不正なら 0）
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
| `0`〜上限 | 距離（mm）。上限は既定で 2000 mm（VL53L0X の最大測距距離）、`RC_setReadLimit()` で決めた場合はその値 |
| `-1` | タイムアウト（センサーが応答しない）。`REAR` では後方センサーが無い車両も `-1` |
| `-2` | 引数が不正（LEFT / CENTER / RIGHT / REAR 以外） |
| `-3` | 範囲外：上限より先、対象物が無い（センサーは 8190 前後を返す）、反射が弱くて測れない |

値は次の順に扱います（Exercise-3.1〜3.3 はこの書き方です）。

1. **まず 0 以上かで分けます。** `0`〜上限は距離です。負の数はステータスで、距離ではありません。ステータスの処理は 1 つのブロックにまとめます。ステータスの扱いを変えても、距離の処理には手を入れずに済みます。
2. **`-1`（応答なし）は止まります。** `RC_drive(BRAKE, 0)` を出して、その回は判断しません（`RC_read(REAR)` は後方センサーが無い車両でも `-1` です）。
3. **`-3`（範囲外）は「その方向の上限より手前に測れた物が無い」という意味です。** 走っていればふつうに起きるステータスで、上限を短くすると前（中央）のセンサーはたいてい `-3` です。道が開けている証拠にはなりません（黒っぽい面、浅い角度で当たる面、ガラス、強い光でも `-3` になることがあります）。`-3` を「遠い」とみなすのは、プログラムの側の決めごとです。
4. **`-3` を「遠い」として比べるときは、プログラムの先頭の定数に置き換えます。** 例：`FAR_MM = 9999`（`RC_read()` が返すどの距離（最大 2000 mm）よりも大きく、Arduino の 16 ビットの `int` にも収まる値）。`-3` のまま比べてはいけません。数としては最も小さいので、`if d < 100:` では「とても近い」と判定されます。999 のような小さい値では、上限を決めていないときの 1000〜2000 mm の距離より小さくなり、左右を比べると逆へ切ります。
5. **`FAR_MM` は大小の比較（しきい値、どちらが広いか）にだけ使います。** 左右の差のように値の大きさを使う計算には、仮の距離を入れずに状況に分けて扱います（Exercise-3.3：両側に壁 → 真ん中を走る、片側の壁だけ → その壁から離れる、壁が無い → まっすぐ）。

```python
FAR_MM = 9999                   # 「遠い」。大小の比較にだけ使う

def loop():
    d = RC_read(CENTER)
    if d < 0:                   # ステータス (距離ではない)
        if d == -1:             # 応答なし → 止まる
            RC_drive(BRAKE, 0)
            return
        d = FAR_MM              # -3: 上限より手前に何も無い。「遠い」として比べる
    # ここから先の d は距離 [mm] (範囲外は FAR_MM)
```

* 表示（`print`）には `RC_read()` が返した値のまま出します。`-3` は距離ではなく範囲外です。
* 上限の判定は API の層で行うので、pigpio の車体でも Zero2Cam でも同じ値になります（[ライブラリの構成](#ライブラリの構成)）。
* `RC_drive()` の値は**信号の割合**です。150 なら 150/255（約 59 %）のデューティの PWM を出します。CM によって PWM の周波数やモーターの特性が異なるため、同じ値でも速さは同じとは限りません。駆動はスローデケイで、停止や逆方向から走り出すときは 200/255 を 20 ms 出してから目標の値にします（RumiCar-lib と同じ）。`BRAKE` は両入力 HIGH の全ブレーキ（値は無視）、`FREE` は惰行です。
* 方向に `True` / `False` は使えません（不正な引数として扱います）。強さや速さに数でない値を渡したときは 0 として扱います。不正な引数は、そのつど止まらずに最初の 1 回だけ警告を表示します（Zero2Cam と同じ扱い）。
* 後方の VL53L0X（任意搭載。XSHUT 無しで I2C に直結）は `RC_setup()` が自動で検出し、`RC_read(REAR)` で読めます。無い車両では `-1` が返るので、同じプログラムがどちらの車両でもそのまま動きます。
* `RC_setup()` が終了時（Ctrl+C やプログラムのエラーを含む）にモーターを止める処理を登録するので、プログラムに後始末は要りません。`RC_setup()` より前に `RC_steer()` などを呼ぶとエラーで止まります。
* 実行には pigpiod（上記）が必要です。起動していなければ `RC_setup()` がエラーで止まります。

### `RC_setReadLimit()`

VL53L0X は 2 m 先まで測れますが、レーザーはコーン状に広がるので、搭載位置によっては遠くで床を捉え、障害物のような距離を返します。どこまでの値を信じるかはライブラリではなくプログラムが決めます。`setup()` の中で、`RC_setup()` のすぐあとに `RC_setReadLimit()` を 1 回呼んでください（値の例と決め方は[上限の決め方](#上限の決め方)）。上限を超える値は、`RC_read()` が `-3`（範囲外）として返します。

| 項目 | 内容 |
|---|---|
| `RC_setReadLimit(limit_mm)` | 前・左・右・後ろのすべてに同じ上限を決める |
| `RC_setReadLimit(direc, limit_mm)` | その方向（`LEFT` / `CENTER` / `RIGHT` / `REAR`）だけ上限を決める |
| `limit_mm` | `1`〜`2000`（mm）。`0` は上限なし（既定の 2000 mm まで返す状態に戻す）。範囲外の値は 0〜2000 に収め、数でない値は 0 として扱います（`RC_steer()` の強さと同じ扱い） |
| 効果 | その方向の値が上限を超えると `RC_read()` が `-3` を返します。上限以下の値・`-1`・`-2` は変わりません |
| 既定 | 上限なし（2000 mm まで返す）。`RC_setup()` は上限を変えないので、`RC_setup()` より前に呼んでもかまいません |
| 戻り値 | `1`（設定した）、`0`（方向が不正。何も変えない） |

* 数値はプログラムの 1 か所、先頭の定数 `LIMIT_MM` にだけ書き、`setup()` で `RC_setReadLimit(LIMIT_MM)` と呼びます。走行のロジックのしきい値はその上限以下にし、そのことをコメントに書いておくと、あとで上限を変えるときに直すのはその定数だけになります（Exercise-3.3）。
* `REAR` は後方センサーが無い車両でも設定できます（`RC_read(REAR)` は `-1` のままです）。
* RumiCar-Zero2Cam が取り込む `RumiCar.py` は Zero2Cam の版ごとに固定されています。`drive.py` で `RC_setReadLimit()` を使えるのは、この 0.7.0 を取り込んだ Zero2Cam 0.7.0 以降です。

### 上限の決め方

この README で上限の値を書くのはこの節だけです。ほかの箇所は「`RC_setReadLimit()` で決めた上限」と名前で参照します。

* **推奨値の例：200 mm。** ハンズオンテキストでは、RumiCar モデル 32 型が床を捉える距離はおよそ 30 cm です。200 mm はそこから余裕を見た値です。Exercise-3.3 はこの値を使い、0.6.0 までの 200 mm での打ち切りとまったく同じ値を受け取ります。

  ```python
  LIMIT_MM = 200          # RC_read が信じる距離の上限 [mm]。これより先は RC_read が -3 (範囲外) を返す

  def setup():
      RC_setup()
      RC_setReadLimit(LIMIT_MM)
  ```

* **自分の車で確かめる。** 床を捉える距離は、車体・搭載の高さ・床で変わります。前に何も無い状態で車を床に置き、上限を決めずに `Exercise-1.2` で 3 つのセンサーの値を表示してください。`-3` の代わりに距離が表示されるところが、床を捉えている距離です。そこから余裕を見て上限を決めます。
* **必要なら方向ごとに。** 左右のセンサーは斜め前を向いているので、床を捉える距離が中央と違うことがあります（まだ実測していません）。違うときは `RC_setReadLimit(LEFT, …)` のように方向ごとに決めます。
* **params.json で変える場合。** プログラムを変えずに値を変えたいときは、`params.json` に書いて `RC_setReadLimit(RC_params().get("read_limit_mm", LIMIT_MM))` のように読む形も使えます（`params.json` に無いときは `LIMIT_MM`）。

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

* **API の層**：`RC_*` の関数・定数・`setup()` / `loop()` の実行。引数を確かめて 0〜255 に収め、`RC_read()` の上限（`RC_setReadLimit()`）を当てはめ、どの環境でも同じ戻り値を返します。
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
| `Exercise-3.3` | なめらかに走る車：値を距離とステータスに分け（`-1` は止まる）、状況に分けて切る。両側に壁 → 真ん中を走る（左右の差が 30 mm 以内ならまっすぐ）、片側の壁だけ → その壁から離れる、壁が無い → まっすぐ。ハンドルの強さは 128 から始め、ずれが縮まらなければ 1.3 倍、目標を越えたら半分にする（40〜255）。状況が変わった回（柱が現れた、壁が途切れたなど）は強さを前回と比べない。強さは `Power` として表示。上限は先頭の `LIMIT_MM` に書き、`setup()` で `RC_setReadLimit(LIMIT_MM)`。Arduino 版（RumiCar-lib v0.7.1）と同じ動き |

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
