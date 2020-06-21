#include <Wire.h>

#include <VL53L0X.h>

#include "RumiCar.h"

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

int iBuf = 0;

void setup()
{
  RC_setup();
}

void loop()
{
int ispeed = 255;
int idist1;
int sDist = 100;
int K_OFF = 64;
int KP = 200;
int KI = 20;
int sP;
long sI;
int sDrive;
int dDist;

  idist1=sensor1.readRangeSingleMillimeters();    // 距離計測
  dDist = idist1 - sDist;                         // 差分

  if ( idist1 < 300 ){                            // 30cm 以内のみ動作
    sP = (dDist * KP) / 100;                      // P項
    iBuf += (sP * KI);                            // 積分
    sI = iBuf >> 8;                               // I項
    sDrive = sP + sI;                             // 指令値
    constrain(sDrive,-255,255);                   // Limit
//    sDrive = sP;
    if ( dDist > 10 ){                            // Direction
      RC_drive(FORWARD,(sDrive + K_OFF));
    }else if (dDist < -10){
      RC_drive(REVERSE,((-sDrive) + K_OFF));
      }else{                                      // +-10mm 以内は停止
        RC_drive(BRAKE,ispeed);                   // Brake
        iBuf = 0;                                 // 積分クリア
      }
  }else{
      RC_drive(FREE,ispeed);                      // Free
      iBuf = 0;                                   // 積分クリア
  }
//  Serial.print("dDis:");      Serial.print(dDist);
//  Serial.print("  sP:");      Serial.print(sP);
//  Serial.print("  sI:");      Serial.print(sI);
//  Serial.print("  sDrive:");  Serial.print(sDrive);
//  Serial.println();
}
