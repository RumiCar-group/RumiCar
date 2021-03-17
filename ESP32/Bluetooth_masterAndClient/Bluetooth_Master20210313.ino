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



String name ="RumiCar";
char *pin = "1234";
bool connected;

void setup()
{   
  RC_setup();
  SerialBT.begin("RumiCar", true);
//  SerialBT.setPin(pin);


  Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect(name);
  //connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(30000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }  
}

void loop()
{
char action;


int ispeed = 150;
int idist1;
idist1=sensor1.readRangeSingleMillimeters();
//Serial.println(idist1);
if ( idist1 < 300 ){
  if ( idist1 > 120 ){
    RC_drive(FORWARD,ispeed);
    action = 'f';
  }else if (idist1 < 80){
    RC_drive(REVERSE,ispeed);
    action = 'b';
  }else{
    RC_drive(BRAKE,ispeed);
    action = 'n';      
    }
 }else{
   RC_drive(FREE,ispeed);
   action = 'n';  
  }
 SerialBT.write(action);

if(sensor0.readRangeSingleMillimeters() < 150){
  RC_steer(RIGHT);
  action = 'r';
}else if (sensor2.readRangeSingleMillimeters() < 150){
  RC_steer(LEFT);
  action = 'l';
}else{
  RC_steer(CENTER);
  action = 'c';
}
 SerialBT.write(action);

/*
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
  */

}
