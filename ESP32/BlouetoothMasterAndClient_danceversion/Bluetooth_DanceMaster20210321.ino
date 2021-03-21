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

  part1();
  part2();
  part3();
  part4();
  part5();
  part6();
  finish();

while(1);

  /*
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

*/

}

void part1(){
//右、左、を２回、を２回
int i, j;
  for(j=0;j<2;j++){
    for(i=0;i<2;i++){
//      analogWrite(AIN1,vola);
      RC_steer(RIGHT);
      SerialBT.write('r');
      delay(500);
//      analogWrite(AIN1,0);
      RC_steer(CENTER);
      SerialBT.write('c');
      delay(500);
      }
    for(i=0;i<2;i++){
//      analogWrite(AIN2,vola);
      RC_steer(LEFT);
      SerialBT.write('l');
      delay(500);
//      analogWrite(AIN2,0);
      RC_steer(CENTER);
      SerialBT.write('c');
      delay(500);
      }
  }
}
void part2(){
  //前に行って戻るを２回
  int ispeed=150;
  int dtime = 300;
  for(int i=0;i<2;i++){
//    analogWrite(BIN1,volb);
    RC_drive(FORWARD,ispeed);
    SerialBT.write('f');
    delay(dtime);
//    analogWrite(BIN1,0);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
    delay(dtime);
//    analogWrite(BIN2,volb);
    RC_drive(REVERSE,ispeed);
    SerialBT.write('b');
    delay(dtime);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN2,0);
    delay(dtime);
  }
  alloff();
}
void part3(){
  int i;
  int dtime = 500;
  int ispeed = 150;
  //ハンドルを右に切り前進と後進を２回
  RC_steer(RIGHT);
  SerialBT.write('r');
//  analogWrite(AIN1,vola);
  for(i=0;i<2;i++){
    RC_drive(FORWARD,ispeed);
    SerialBT.write('f');
//    analogWrite(BIN1,volc);
    delay(dtime);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN1,0);
    delay(dtime);
    RC_drive(REVERSE,ispeed);
    SerialBT.write('b');
//    analogWrite(BIN2,volc);
    delay(dtime);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN2,0);
    delay(dtime);
  }
  alloff();
  RC_steer(LEFT);
  SerialBT.write('l');
//  analogWrite(AIN1,0);
//  analogWrite(AIN2,vola);
  for(i=0;i<2;i++){
    RC_drive(FORWARD,ispeed);
    SerialBT.write('f');
//    analogWrite(BIN1,volc);
    delay(dtime);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN1,0);
    delay(dtime);
    RC_drive(REVERSE,ispeed);
    SerialBT.write('b');
//    analogWrite(BIN2,volc);
    delay(dtime);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN2,0);
    delay(dtime);
  }
  alloff();
}


void part4(){
  int i;
  int loop = 4;
  int dtime = 400;
  int ispeed = 150;
  RC_drive(FORWARD,ispeed);
  SerialBT.write('f');
//  analogWrite(BIN1,volb);
  for(i=0;i<loop;i++){
    RC_steer(RIGHT);
    SerialBT.write('r');
//    analogWrite(AIN1,vola);
    delay(dtime);
    RC_steer(LEFT);
    SerialBT.write('l');
//    analogWrite(AIN1,0);
//    analogWrite(AIN2,vola);
    delay(dtime);
    RC_steer(CENTER);
    SerialBT.write('c');
//    analogWrite(AIN2,0);
  }
  alloff();
  RC_drive(REVERSE,ispeed);
  SerialBT.write('b');
//  analogWrite(BIN2,volb);
  for(i=0;i<loop;i++){
    RC_steer(RIGHT);
    SerialBT.write('r');
//    analogWrite(AIN1,vola);
    delay(dtime);
    RC_steer(LEFT);
    SerialBT.write('l');
//    analogWrite(AIN1,0);
//    analogWrite(AIN2,vola);
    delay(dtime);
    RC_steer(CENTER);
    SerialBT.write('c');
//    analogWrite(AIN2,0);
  }
  alloff();
}
void part5(){
  int i;
  int ispeed = 150;
    for(i=0;i<10;i++){
      RC_steer(RIGHT);
      SerialBT.write('r');
      RC_drive(FORWARD,ispeed);
      SerialBT.write('f');
//    analogWrite(AIN1,vola);
//    analogWrite(BIN1,volc);
    delay(1000);
    RC_steer(LEFT);
    SerialBT.write('l');
    RC_drive(REVERSE,ispeed);
    SerialBT.write('b');
//    analogWrite(AIN1,0);
//    analogWrite(AIN2,vola);
//    analogWrite(BIN2,volc);
//    analogWrite(BIN1,0);
    delay(1000);
    RC_steer(CENTER);
    SerialBT.write('c');
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN2,0);
//    analogWrite(AIN2,0);
  }
  alloff();
}

void part6(){
  int i;
  int ispeed = 150;
  for(i=0;i<5;i++){
    RC_steer(RIGHT);
    SerialBT.write('r');
    RC_drive(FORWARD,ispeed);
    SerialBT.write('f');
//    analogWrite(AIN1,vola);
//    analogWrite(BIN1,volc);
    delay(500);
    RC_steer(LEFT);
    SerialBT.write('l');
//    analogWrite(AIN2,vola);
//    analogWrite(AIN1,0);
    delay(450);
    RC_steer(CENTER);
    SerialBT.write('c');
    RC_drive(REVERSE,ispeed);
    SerialBT.write('b');
//    analogWrite(BIN1,0);
//    analogWrite(AIN2,0);
//    analogWrite(BIN2,volc);
    delay(1000);
    RC_drive(FORWARD,ispeed);
    SerialBT.write('f');
//    analogWrite(BIN1,volc);
//    analogWrite(BIN2,0);
    delay(200);
    RC_drive(BRAKE,ispeed);
    SerialBT.write('n');
//    analogWrite(BIN1,0);

  }
  alloff();
}
void finish(){
  int i;
  for(i=0;i<10;i++){
    RC_steer(RIGHT);
    SerialBT.write('r');
//    analogWrite(AIN1,vola);
    delay(100);
    RC_steer(LEFT);
    SerialBT.write('l');
//    analogWrite(AIN1,0);
//    analogWrite(AIN2,vola);
    delay(100);
    RC_steer(CENTER);
    SerialBT.write('c');
//    analogWrite(AIN2,0);
    }
  alloff();
  RC_steer(RIGHT);
  SerialBT.write('r');
//  analogWrite(AIN1,vola);
  delay(500);
  RC_steer(LEFT);
  SerialBT.write('l');
//  analogWrite(AIN2,vola);
//  analogWrite(AIN1,0);
  delay(500);
  alloff();
}
void alloff(){
  RC_steer(CENTER);
  SerialBT.write('c');
  RC_drive(BRAKE,150);
  SerialBT.write('n');
//  analogWrite(AIN1,0);
//  analogWrite(AIN2,0);
//  analogWrite(BIN1,0);
//  analogWrite(BIN2,0);
  delay(1000);
}
