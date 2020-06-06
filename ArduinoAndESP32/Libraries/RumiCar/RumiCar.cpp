// RumiCar Libraly

void RC_setup()
{
  Serial.begin(9600);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(SHUT0, OUTPUT);
  pinMode(SHUT1, OUTPUT);
  pinMode(SHUT2, OUTPUT);

  digitalWrite(SHUT0, LOW);
  digitalWrite(SHUT1, LOW);
  digitalWrite(SHUT2, LOW);
  delay(150);
  Wire.begin();
  //sensor0
  pinMode(SHUT0, INPUT);
  delay(150);
  sensor0.init(true);
  delay(100);
  sensor0.setAddress((uint8_t)20);
  sensor0.setTimeout(500);
  //sensor1
  pinMode(SHUT1, INPUT);
  delay(150);
  sensor1.init(true);
  delay(100);
  sensor1.setAddress((uint8_t)21);
  sensor1.setTimeout(500);
  //sensor2
  pinMode(SHUT2, INPUT);
  delay(150);
  sensor2.init(true);
  delay(100);
  sensor2.setAddress((uint8_t)22);
  sensor2.setTimeout(500);

  //  sensor.init();
  //  sensor.setTimeout(500);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor0.setSignalRateLimit(0.1);
  sensor1.setSignalRateLimit(0.1);
  sensor2.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor0.setMeasurementTimingBudget(20000);
  sensor1.setMeasurementTimingBudget(20000);
  sensor2.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor0.setMeasurementTimingBudget(200000);
  sensor1.setMeasurementTimingBudget(200000);
  sensor2.setMeasurementTimingBudget(200000);
#endif

  sensor0.startContinuous();
  sensor1.startContinuous();
  sensor2.startContinuous();

#if defined ESP32
//ESP32の場合はピン番号ではなくチャンネルでPWMを行うのでチャンネルとして再設定
#define PWM_level 8
// 8の場合8bitの解像度でArduinoと同じESPは16bit迄行ける？
//モータのPWMのチャンネル、周波数の設定
ledcSetup(0, 490, PWM_level);
ledcSetup(1, 490, PWM_level);
ledcSetup(2, 960, PWM_level);
ledcSetup(3, 960, PWM_level);

//モータのピンとチャンネルの設定
ledcAttachPin(AIN1, 0);
ledcAttachPin(AIN2, 1);
ledcAttachPin(BIN1, 2);
ledcAttachPin(BIN2, 3);
//pin番号をチャンネル番号に上書き
AIN1 = 0;
AIN2 = 1;
BIN1 = 2;
BIN2 = 3;

#endif

  RC_analogWrite(AIN1, 0);
  RC_analogWrite(AIN2, 0);
  RC_analogWrite(BIN1, 0);
  RC_analogWrite(BIN2, 0);


}
//操舵の関数
int RC_steer (int direc ){
  if ( direc == RIGHT ){
    RC_analogWrite(AIN1,255);
    RC_analogWrite(AIN2,0);
  }else if ( direc == LEFT ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,255);
  }else if ( direc == CENTER ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,0);
  }else{
    return 0;
  }
}

//走行の関数
int RC_drive(int direc, int ipwm){
  if ( direc == FREE ){
    RC_analogWrite(BIN1,0);
    RC_analogWrite(BIN2,0);
  }else if ( direc == REVERSE ){
    RC_analogWrite(BIN1,0);
    RC_analogWrite(BIN2,ipwm);
  }else if ( direc == FORWARD ){
    RC_analogWrite(BIN1,ipwm);
    RC_analogWrite(BIN2,0);
  }else if ( direc == BRAKE ){
    RC_analogWrite(BIN1,ipwm);
    RC_analogWrite(BIN2,ipwm);
  }else{
    return 0;
  }
}
