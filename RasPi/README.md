# RumiCar
2020年4月25日(土)に開催された「RumiCarハンズオン中継！自動運転アルゴリズムを楽しく手軽に体感しよう！#2 」で使用されたExerciseのラズパイ版です。

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

## Exercises

```
cd ~/RasPi/Exercise-1.1
python Exercise-1.1.py

cd ~/RasPi/Exercise-1.2
python Exercise-1.2.py

# ...
```

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
