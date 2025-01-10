#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{
  Serial.println(sensor1.readRangeSingleMillimeters());
}
