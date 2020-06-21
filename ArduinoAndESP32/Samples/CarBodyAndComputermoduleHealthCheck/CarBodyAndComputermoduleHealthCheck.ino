#include <Wire.h>

#include <VL53L0X.h>

#include"RumiCar.h"

VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;

void setup()
{

 RC_setup();

  RC_steer(RIGHT);
  delay(500);
  RC_steer(LEFT);
  delay(500);
  RC_steer(CENTER);
  RC_drive(FORWARD,255);
  delay(500);
  RC_drive(REVERSE,255);
  delay(500);
  RC_drive(FREE,0);

}

void loop()
{


  Serial.print("Sensor0:");
  Serial.print(sensor0.readRangeSingleMillimeters());
  if (sensor0.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  Serial.print("  Sensor1:");
  Serial.print(sensor1.readRangeSingleMillimeters());
  if (sensor1.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  Serial.print("  Sensor2:");
  Serial.print(sensor2.readRangeSingleMillimeters());
  if (sensor2.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  Serial.println();
}
