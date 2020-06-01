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
  RC_setup();
}

void loop()
{
  RC_drive(FORWARD,100);  RC_delay(1000);
  RC_drive(BRAKE,255);    RC_delay(500);

  RC_drive(REVERSE,80);   RC_steer2(LEFT,255);    RC_delay(1000);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);
  RC_drive(FORWARD,80);   RC_steer2(CENTER,255);  RC_delay(400);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);
  RC_drive(REVERSE,80);   RC_steer2(LEFT,255);    RC_delay(400);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);

  RC_drive(REVERSE,80);   RC_steer2(RIGHT,255);   RC_delay(800);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);
  RC_drive(FORWARD,80);   RC_steer2(CENTER,255);  RC_delay(400);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);
  RC_drive(REVERSE,80);   RC_steer2(RIGHT,255);   RC_delay(600);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);

  RC_drive(FORWARD,50);   RC_steer2(CENTER,255);  RC_delay(300);
  RC_drive(BRAKE,255);    RC_steer2(CENTER,255);  RC_delay(500);

  RC_delay(30000);
}
