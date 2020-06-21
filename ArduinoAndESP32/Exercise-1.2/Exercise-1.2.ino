#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{
  Serial.print("Sensor0:");
  Serial.print(sensor0.readRangeSingleMillimeters());
  Serial.print("  Sensor1:");
  Serial.print(sensor1.readRangeSingleMillimeters());
  Serial.print("  Sensor2:");
  Serial.println(sensor2.readRangeSingleMillimeters());

}
