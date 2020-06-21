#include <Wire.h>
#include <VL53L0X.h>

#include "RumiCar.h"

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

void setup()
{
//  Timer0 の周期変更しているため，RC_delay()を使うこと
 RC_setup();
}

void loop()
{
//  始動後に低速へ
  RC_drive(FORWARD,80);     RC_delay(500);
  RC_drive(BRAKE,255);      RC_delay(500);
  RC_drive(REVERSE,80);     RC_delay(500);
  RC_drive(BRAKE,255);      RC_delay(500);
//  RC_drive(FREE,255);       RC_delay(1000);

  RC_drive(FORWARD,60);     RC_delay(666);
  RC_drive(BRAKE,255);      RC_delay(500);
  RC_drive(REVERSE,60);     RC_delay(666);
  RC_drive(BRAKE,255);      RC_delay(500);
//  RC_drive(FREE,255);       RC_delay(1000);

  RC_drive(FORWARD,40);     RC_delay(1000);
  RC_drive(BRAKE,255);      RC_delay(500);
  RC_drive(REVERSE,40);     RC_delay(1000);
  RC_drive(BRAKE,255);      RC_delay(500);
//  RC_drive(FREE,255);       RC_delay(1000);
}
