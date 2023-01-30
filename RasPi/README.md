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
* https://www.raspberrypi.com/documentation/accessories/camera.html
* https://wiki.marcluerssen.de/index.php?title=Raspberry_Pi/Camera_streaming
* https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119415572.app2
* https://github.com/george-hawkins/pololu-romi-jetbot/blob/master/pi-camera-notes.md

On RasPi:
```
raspivid -a 1023 -md 5 -fps 10 -t 0 -n -l -o tcp://0.0.0.0:5000
# -a 1023 : enable all annotations
# -md 5 : 1296x730, 1-49fps, full FoV, 2x2 binning
# -t 0 : no timeout
# -n : no preview window
# -l : listen for tcp connection
# -o : output to
```

On PC with SMPlayer (or similar): 
* Open → URL: `tcp://rcar.local:5000`