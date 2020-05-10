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
int ispeed = 255;
int idist1;
idist1=sensor1.readRangeSingleMillimeters();
if ( idist1 < 300 ){
  if ( idist1 > 120 ){
    RC_drive(FORWARD,ispeed);
  }else if (idist1 < 80){
    RC_drive(REVERSE,ispeed);;
    }else{
    RC_drive(BRAKE,ispeed);  
    }
}else{
    RC_drive(FREE,ispeed);  
  }
}
