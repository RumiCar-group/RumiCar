#include <RumiCar.h>

void setup()
{
  RC_setup();
}

void loop()
{

int ispeed = 255;
int idist1;
idist1=sensor1.readRangeSingleMillimeters();
Serial.println(idist1);
if ( idist1 < 300 ){
  if ( idist1 > 120 ){
    RC_drive(FORWARD,ispeed);
  }else if (idist1 < 80){
    RC_drive(REVERSE,ispeed);;
    }else{
      RC_drive(BRAKE,ispeed);  
    }
  }else{
    RC_drive(FREE,ispeed);  
  }
}
