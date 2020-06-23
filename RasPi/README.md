# RumiCar
2020年4月25日(土)に開催された「RumiCarハンズオン中継！自動運転アルゴリズムを楽しく手軽に体感しよう！#2 」で使用されたExerciseのラズパイ版です。

# ラズパイZeroW GPIO

[pigpio library](http://abyz.me.uk/rpi/pigpio/index.html) 使用。  
以下主要なGPIOをライブラリサイトより一部抜粋。

| GPIO | PIN |       |
| ---- | --- | ----- |
|      | 1   | 3V3   |
|      | 2   | 5V    |
| 2    | 3   | SDA   |
|      | 4   | 5V    |
| 3    | 5   | SCL   |
|      | 6   | GND   |
|      | 9   | GND   |
| 17   | 11  | AIN1  |
| 18   | 12  | BIN1  |
| 27   | 13  | AIN2  |
|      | 14  | GND   |
| 22   | 15  | SERVO |
| 23   | 16  | SHDN0 |
|      | 17  | 3V3   |
| 24   | 18  | SHDN1 |
|      | 20  | GND   |
| 25   | 22  | SHDN2 |
|      | 25  | GND   |
|      | 30  | GND   |
| 13   | 33  | BIN2  |
|      | 34  | GND   |
| 26   | 37  | SERVO |
| 26   | 39  | GND   |

