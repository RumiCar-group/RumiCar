/* This example shows how to get single-shot range
  measurements from the VL53L0X. The sensor can optionally be
  configured with different ranging profiles, as described in
  the VL53L0X API user manual, to get better performance for
  a certain application. This code is based on the four
  "SingleRanging" examples in the VL53L0X API.

  The range readings are in units of mm. */

#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{
  RC_drive(FORWARD,255);
  RC_steer(RIGHT);
  delay(500);
  RC_steer(LEFT);
  delay(500);
}
