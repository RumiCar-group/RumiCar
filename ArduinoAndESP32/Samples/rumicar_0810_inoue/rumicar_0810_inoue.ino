#include <RumiCar.h>

int before_s1;
int s0, s1, s2;
int ibound;

void setup()
{
  RC_setup();
  ibound = 250;
  before_s1 = 0;
}

// 255以上の値を出力しない。
int sat_value(unsigned int num){
  if(num >= 255){
    return 255;
  }
  
  return num;
}

void loop()
{

  s0=sensor0.readRangeSingleMillimeters();
  s1=sensor1.readRangeSingleMillimeters();
  s2=sensor2.readRangeSingleMillimeters();

  Serial.print("Sensor0:");
  Serial.print(s0);
  Serial.print("  Sensor1:");
  Serial.print(s1);
  Serial.print("  Sensor2:");
  Serial.println(s2);

  // NOTE: バック時の処理を開始
  if(s1<=100){
    if(s0 < s2-10 ){
      RC_steer(LEFT);
    }else if(s0 > s2+10){
      RC_steer(RIGHT);
    }

    RC_drive(REVERSE,sat_value(300-s1));
    // NOTE: ここでバック処理終了
    return;

  // NOTE: ここから前進処理を開始
  }else if (s1<=150){
    RC_drive(FORWARD,sat_value(s1+50));
  }else if (s1<=250){
    RC_drive(FORWARD,sat_value(s1*4/5));
  }else{
    RC_drive(FORWARD,255);
  }

  // NOTE: 前方の障害がほぼほぼ存在していないときは前進のみで終了
  if(s0 < s2+10 && s0 > s2-10){
    return;
  }

  // NOTE: 前進時のハンドル処理
  if(s0>s2){
    RC_steer(LEFT);
  }else if (s2>s0){
    RC_steer(RIGHT);
  }

}
