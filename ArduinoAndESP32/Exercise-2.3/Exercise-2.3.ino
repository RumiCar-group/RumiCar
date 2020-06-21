#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{
  RC_drive(FORWARD,255);
  delay(500);
  RC_drive(REVERSE,255);
  delay(500);
}
