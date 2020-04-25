#include <Wire.h>
#include <VL53L0X.h>

#define SENSOR_ELEMENT_COUNT 3  // 使用するセンサーの数
#define BUFFER_ELEMENT_COUNT 20 // 使用する距離バッファの数
#define ADDRESS_BASE (uint8_t)20 // 各距離センサに用いるアドレスの基底値

/**
   各センサのXSHUTへ接続されているGPIOの配列
*/
const int SHUT[SENSOR_ELEMENT_COUNT] = {14, 15, 16}; // SHUTはディジタルピン。A0はD14、以降同じなので注意すること
VL53L0X DistanceSensors[SENSOR_ELEMENT_COUNT]; // 使用するセンサークラス配列

// Aが操舵、Bが走行
#define AIN1 3
#define AIN2 11
#define BIN1 5
#define BIN2 6

// #define LONG_RANGE
#define HIGH_SPEED
// #define HIGH_ACCURACY

const byte HALT = 0;
const byte FORTH = 1;
const byte BACK = 255;
const int POWER_ATTENUATION = 18; // 減衰(加減)値

byte Distances[SENSOR_ELEMENT_COUNT][BUFFER_ELEMENT_COUNT];
byte Vector = 1; // デフォルトは中央
int DistancesCounter = 0;


void setup()
{
  // Serial.begin(9600);
  Serial.begin(38400);

  // まず全てのGPIOをLOW
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  for (int i = 0; i < SENSOR_ELEMENT_COUNT; i++)
  {
    pinMode(SHUT[i], OUTPUT);
    digitalWrite(SHUT[i], LOW);
  }

  delay(150);
  Wire.begin();

  for (int i = 0; i < SENSOR_ELEMENT_COUNT; i++) {
    // センサを初期化
    pinMode(SHUT[i], INPUT);
    delay(150);
    if (DistanceSensors[i].init(true) == true)
    {
      delay(100);
      int address = ADDRESS_BASE + i;
      DistanceSensors[i].setAddress(address);
      DistanceSensors[i].setTimeout(500);

#if defined LONG_RANGE
      // lower the return signal rate limit (default is 0.25 MCPS)
      DistanceSensors[i].setSignalRateLimit(0.1);
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      DistanceSensors[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      DistanceSensors[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
      DistanceSensors[i].setMeasurementTimingBudget(20000);   // reduce timing budget to 20 ms (default is about 33 ms)
#elif defined HIGH_ACCURACY
      DistanceSensors[i].setMeasurementTimingBudget(200000);  // increase timing budget to 200 ms
#endif

      DistanceSensors[i].startContinuous(10);
    }
    else
    {
      Serial.println("Sensor " + String(i) + " error");
    }
  }
}


void loop()
{
  byte motions[SENSOR_ELEMENT_COUNT] = {HALT, HALT, HALT};
  byte powers[SENSOR_ELEMENT_COUNT] = {0, 0, 0};

  for (int i = 0; i < SENSOR_ELEMENT_COUNT; i++) {
    int distance = DistanceSensors[i].readRangeSingleMillimeters() / 10;
    switch (distance) {
      case 0 ... 7:
        powers[i]= 255 - distance * POWER_ATTENUATION;
        motions[i] = BACK;
        break;
      case 13 ... 20:
        powers[i]= 255 + (distance-20) * POWER_ATTENUATION;
        motions[i] = FORTH;
        break;
      default:
        motions[i]=HALT;
    }
    setDistanceToArray(i,distance); // 各センサの距離をループバッファにストア
  }

  byte vector = decideVector(); // クルマの移動方向(ステアリング)
  byte motion = motions[vector]; // 動作コマンド
  byte power = powers[vector]; // モーターパワー
  command2car(vector, motion, power); // クルマをコントロールする

  // String strVal = "vector:" + String(vector) + " / vector_distance:" + String(Distances[vector][DistancesCounter]) + " / motion:" + String(motion);
  // Serial.println(strVal);
}


// クルマの移動方向(配列の添字)の決定
byte decideVector(){
  int vectorDistance = Distances[Vector][DistancesCounter]; // 現在選択中のセンサ個体の距離を取得
  if (vectorDistance == 255 || aggregationOfOperatingTiming(Vector) == 0) { // 完全にセンシング範囲から外れている、または一定期間以上に渡って停止していた場合のみ、対象とする
    // 最短距離のセンサ個体を求める
    for (int i = 0; i < SENSOR_ELEMENT_COUNT; i++) {
      int distance = Distances[i][DistancesCounter];
      if (distance < vectorDistance) {
        Vector = i;
        vectorDistance = distance;
      }
    } 
  }
  updateDistancesCounter(); // ループバッファのインデックス変数(DistancesCounter)のアップデート
  return(Vector);
}


// ループバッファのインデックス変数(DistancesCounter)のアップデート
void updateDistancesCounter(){
  DistancesCounter++;
  if (DistancesCounter >= BUFFER_ELEMENT_COUNT){DistancesCounter=0;}
}


// 各センサの距離をループバッファにストア
void setDistanceToArray(int sensorElement, int distance){
  if (distance>20) {
    Distances[sensorElement][DistancesCounter]=255;
  } else {
    Distances[sensorElement][DistancesCounter]=(byte)distance;
  }
}


// クルマの稼働時間の集計
int aggregationOfOperatingTiming(int sensorElement){
  int operatingTiming = 0;
  for (int i = 0; i < BUFFER_ELEMENT_COUNT; i++) {
    switch (Distances[sensorElement][i]) {
      case 8 ... 12:
      case 255:
        break;
      default:
        operatingTiming++;
    }
  }
  return(operatingTiming);
}


// クルマをコントロールする
void command2car(byte vector, byte motion, byte power) {
   // ステアリング
  switch (vector) {
    case 0: //左
      analogWrite(AIN1, 0);
      analogWrite(AIN2, 255);
      break;
    case 2: //右
      analogWrite(AIN1, 255);
      analogWrite(AIN2, 0);
      break;
    default: //中央
      analogWrite(AIN1, 0);
      analogWrite(AIN2, 0);
  }
  // 動作コマンド ＆ モーターパワー
  switch (motion) {
    case HALT:
      analogWrite(BIN1, 255);
      analogWrite(BIN2, 255);
      break;
    case FORTH:
      analogWrite(BIN1, power);
      analogWrite(BIN2, 0);
      break;
    case BACK:
      analogWrite(BIN1, 0);
      analogWrite(BIN2, power);
      break;
    default:
      analogWrite(BIN1, 0);
      analogWrite(BIN2, 0);
  }
}
