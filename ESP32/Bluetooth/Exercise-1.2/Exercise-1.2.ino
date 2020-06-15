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
  SerialBT.begin("RumiCar_ESP32");
}

void loop()
{
  SerialBT.print("Sensor0:");
  SerialBT.print(sensor0.readRangeSingleMillimeters());
  SerialBT.print("  Sensor1:");
  SerialBT.print(sensor1.readRangeSingleMillimeters());
  SerialBT.print("  Sensor2:");
  SerialBT.println(sensor2.readRangeSingleMillimeters());


if(SerialBT.available()){
  char action = SerialBT.read();

  
  if ( action == 'r' ){
    RC_steer(RIGHT);
    delay(500);
    RC_steer(CENTER);
  }else if ( action == 'l' ){
      RC_steer(LEFT);
      delay(500);
      RC_steer(CENTER);
  }else if ( action == 'f' ){
        RC_drive(FORWARD,200);
        delay(500);
        RC_drive(FREE,0);
  }else if ( action == 'b' ){
    RC_drive(REVERSE,200);
    delay(500);
    RC_drive(FREE,0);
  }
}
  

}
