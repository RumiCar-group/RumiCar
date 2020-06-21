#include <Wire.h>

#include <VL53L0X.h>

#include "RumiCar.h"

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

void setup()
{
  RC_setup();
}

void loop()
{
  RC_drive(FORWARD,60);
  RC_steer2(RIGHT,200);   RC_delay(500);
  RC_drive(BRAKE,255);    RC_delay(500);

  RC_drive(REVERSE,60);   
  RC_steer2(LEFT,200);    RC_delay(500);
  RC_drive(BRAKE,255);    RC_delay(500);
}
