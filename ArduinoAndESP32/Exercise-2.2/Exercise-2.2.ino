#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{
  RC_drive(FORWARD,255);
  delay(500);
  RC_drive(FORWARD,200);
  delay(500);
  RC_drive(FORWARD,150);
  delay(500);
}
