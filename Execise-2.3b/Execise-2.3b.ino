/* This example shows how to get single-shot range
  measurements from the VL53L0X. The sensor can optionally be
  configured with different ranging profiles, as described in
  the VL53L0X API user manual, to get better performance for
  a certain application. This code is based on the four
  "SingleRanging" examples in the VL53L0X API.

  The range readings are in units of mm. */

#include <Wire.h>

#include <VL53L0X.h>

#include "RumiCar.h"

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

void setup()
{
// Timer0 変更
 TCCR0B &= B11111000;
//  TCCR0B |= B00000001;                      // 1分周=   16M/   1/256=62.5kHz
//  TCCR0B |= B00000010;                      // 8分周=   16M/   8/256=7.8kHz
  TCCR0B |= B00000011;                      // 64分周=  16M/  64/256=976Hz  #default
//  TCCR0B |= B00000100;                      // 256分周= 16M/ 256/256=244Hz
//  TCCR0B |= B00000101;                      // 1024分周=16M/1024/256=61Hz

// Arduino nano の場合
// 分周比を変えてPWM周期を変える，Timer0のdelay()時間がずれるから注意
// Timer1/2 はPWM周期変えてもdelay()に影響しない

 RC_setup();
}

void loop()
{
  loop0();
//  loop1();
//  loop2();
}
void loop0()
{
//  Normal
  RC_drive(FORWARD,255);    delay(500);
  RC_drive(REVERSE,255);    delay(500);
}

void loop1()
{
//  始動後に低速へ
  RC_drive(FORWARD,255);    delay(100);
  RC_drive(FORWARD,64);     delay(400);
  RC_drive(REVERSE,255);    delay(100);
  RC_drive(REVERSE,64);     delay(400);
}

void loop2()
{
//  どこまで走るか，周波数変えてみると結果が変わる
  RC_drive(FORWARD,255);    delay(100);
  RC_drive(FORWARD,150);    delay(300);
  RC_drive(FORWARD,120);    delay(300);
  RC_drive(FORWARD,90);     delay(300);
  RC_drive(FORWARD,60);     delay(300);
  RC_drive(FORWARD,40);     delay(300);
  RC_drive(FORWARD,20);     delay(300);

  RC_drive(REVERSE,255);    delay(100);
  RC_drive(REVERSE,150);    delay(300);
  RC_drive(REVERSE,120);    delay(300);
  RC_drive(REVERSE,90);     delay(300);
  RC_drive(REVERSE,60);     delay(300);
  RC_drive(REVERSE,40);     delay(300);
  RC_drive(REVERSE,20);     delay(300);
}
