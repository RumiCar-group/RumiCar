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
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;


void setup()
{
  RC_setup();

  SerialBT.begin("RumiCar");

}

void loop()
{

int ispeed = 150;

if(SerialBT.available()){
  char action = SerialBT.read();


  if ( action == 'r' ){
    RC_steer(RIGHT);
  }else if ( action == 'l' ){
      RC_steer(LEFT);
  }else if ( action == 'c' ){
      RC_steer(CENTER);
  }else if ( action == 'f' ){
        RC_drive(FORWARD,ispeed);
  }else if ( action == 'b' ){
    RC_drive(REVERSE,ispeed);
  }else if ( action == 'n' ){
    RC_drive(FREE,0);
  }else{
    RC_drive(FREE,0);
    RC_steer(CENTER);
  }

 
}
}
