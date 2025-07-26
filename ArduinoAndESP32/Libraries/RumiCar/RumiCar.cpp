// RumiCar Library

#include "RumiCar.h"
#include <Wire.h>

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

namespace
{
VL53L0X& getSensor(uint8_t pin)
{
  return pin == SHUT0 ? sensor0 : pin == SHUT1 ? sensor1 : sensor2;
}

void setSensorAddress(uint8_t pin, uint8_t address)
{
  // turn on the device via INPUT, which enables XSHUT pull-up resistor
  // , so it is equivalent to digitalWrite(pin, HIGH);
  pinMode(pin, INPUT);
  delay(150);

  auto& sensor = getSensor(pin);
  sensor.init(true);
  delay(100);

  // set address which stays until next reboot
  sensor.setAddress(address);
  sensor.setTimeout(500);

  // next device won't overwrite it
}

void initI2C()
{
  // turn off VL53L0X, so later turn them on one by one and set address
  digitalWrite(SHUT0, LOW);
  digitalWrite(SHUT1, LOW);
  digitalWrite(SHUT2, LOW);
  delay(150);

#if defined SDA0 && defined SCL0
  Wire.setSDA(SDA0);
  Wire.setSCL(SCL0);
#endif
  Wire.begin();

  // address needs to be set on each boot
  setSensorAddress(SHUT0, 20);
  setSensorAddress(SHUT1, 21);
  setSensorAddress(SHUT2, 22);
}
}

void RC_setup()
{
#if defined (ARDUINO_ARCH_SPRESENSE)
  Serial.begin(115200);
#else
  Serial.begin(9600);
#endif

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(SHUT0, OUTPUT);
  pinMode(SHUT1, OUTPUT);
  pinMode(SHUT2, OUTPUT);

  initI2C();

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor0.setSignalRateLimit(0.1);
  sensor1.setSignalRateLimit(0.1);
  sensor2.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor0.setMeasurementTimingBudget(20000);
  sensor1.setMeasurementTimingBudget(20000);
  sensor2.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor0.setMeasurementTimingBudget(200000);
  sensor1.setMeasurementTimingBudget(200000);
  sensor2.setMeasurementTimingBudget(200000);
#endif

  sensor0.startContinuous();
  sensor1.startContinuous();
  sensor2.startContinuous();

#if defined ESP32
  //ESP32の場合はピン番号ではなくチャンネルでPWMを行うのでチャンネルとして再設定
#define PWM_level 8
  // 8の場合8bitの解像度でArduinoと同じESPは16bit迄行ける？
  //モータのPWMのチャンネル、周波数の設定
  ledcSetup(0, 490, PWM_level);
  ledcSetup(1, 490, PWM_level);
  ledcSetup(2, 960, PWM_level);
  ledcSetup(3, 960, PWM_level);

  //モータのピンとチャンネルの設定
  ledcAttachPin(AIN1, 0);
  ledcAttachPin(AIN2, 1);
  ledcAttachPin(BIN1, 2);
  ledcAttachPin(BIN2, 3);
  //pin番号をチャンネル番号に上書き
  AIN1 = 0;
  AIN2 = 1;
  BIN1 = 2;
  BIN2 = 3;

#elif defined (ARDUINO_ARCH_SPRESENSE)

  analogWriteSetDefaultFreq(10000);

#endif

  RC_analogWrite(AIN1, 0);
  RC_analogWrite(AIN2, 0);
  RC_analogWrite(BIN1, 0);
  RC_analogWrite(BIN2, 0);

}
//操舵の関数
int RC_steer (int direc ){
  if ( direc == RIGHT ){
    RC_analogWrite(AIN1,255);
    RC_analogWrite(AIN2,0);
  }else if ( direc == LEFT ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,255);
  }else if ( direc == CENTER ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,0);
  }else{
    return 0;
  }
  return 1;
}

//走行の関数
int RC_drive(int direc, int ipwm){
  if ( direc == FREE ){
    RC_analogWrite(BIN1,0);
    RC_analogWrite(BIN2,0);
  }else if ( direc == REVERSE ){
    RC_analogWrite(BIN1,0);
    RC_analogWrite(BIN2,ipwm);
  }else if ( direc == FORWARD ){
    RC_analogWrite(BIN1,ipwm);
    RC_analogWrite(BIN2,0);
  }else if ( direc == BRAKE ){
    RC_analogWrite(BIN1,ipwm);
    RC_analogWrite(BIN2,ipwm);
  }else{
    return 0;
  }
  return 1;
}
