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

//Slave ESP32 MAC address

//String MACadd ="80:7D:3A:F4:6F:BA";
//uint8_t address[6] = {0x80, 0x7D, 0x3A, 0xF4, 0x6F, 0xBA };
/*
String name ="RumiCar;
char *pin = "1234";
bool connected;
*/
void setup()
{
  RC_setup();
  Serial.println("point1");
  SerialBT.begin("RumiCar");
  Serial.println("point2");
//  Serial.begin(115200);
  Serial.println("Start");
  SerialBT.begin("RumiCar");
  Serial.println(SerialBT.available());
}

void loop()
{

//Serial.println("OK");

if(SerialBT.available()){
  char action = SerialBT.read();
  Serial.print("char=");
  Serial.println(action);

  if ( action == 'r' ){
    RC_steer(RIGHT);
  }else if ( action == 'l' ){
      RC_steer(LEFT);
  }else if ( action == 'c' ){
      RC_steer(CENTER);
  }else if ( action == 'f' ){
        RC_drive(FORWARD,255);
  }else if ( action == 'b' ){
    RC_drive(REVERSE,255);
  }else if ( action == 'n' ){
    RC_drive(FREE,0);
  }else{
    RC_drive(FREE,0);
    RC_steer(CENTER);
  }

 
}
}
