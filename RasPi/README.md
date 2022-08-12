# RumiCar
2020年4月25日(土)に開催された「RumiCarハンズオン中継！自動運転アルゴリズムを楽しく手軽に体感しよう！#2 」で使用されたExerciseのラズパイ版です。

## Setup

If you don't have RasPi monitor and keyboard, please check [Remote Access](docs/RemoteAccess.md).

### [pigpio](https://abyz.me.uk/rpi/pigpio/download.html)
```
sudo apt-get update
sudo apt-get install pigpio python3-pigpio  # python-pigpio - python2 deprecated

sudo systemctl enable --now pigpiod
```

### SMBus (I2C)
```
sudo apt install python3-smbus

sudo raspi-config
# Interface Options -> I2C -> Yes
```

## Exercises

```
cd ~/RasPi/Exercise-1.1
python Exercise-1.1.py

cd ~/RasPi/Exercise-1.2
python Exercise-1.2.py

# ...
```

## ラズパイZeroW GPIO

[pigpio library](http://abyz.me.uk/rpi/pigpio/index.html) 使用。  
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
