// RumiCar Libraly
#include <TimerOne.h>

// Private Variable
#define   UP_SPEED      (200)       // StartUP speed
#define   UP_TIME       (3)         // 30[ms]

volatile  int s_speed = 0;
volatile  int s_gear = FREE;
volatile  int s_steer = CENTER;
volatile  int s_steera = 255;
volatile  int s_sptim = 0;

// RumiCar delay:PWM 周波数変更しているためTimer0を調整
void RC_delay(unsigned long tim)
{
#if defined ESP32
  delay(tim);
#else
  delay(tim / 4);
#endif
}

//操舵の関数
int RC_steer (int direc ){
  // Save
  s_steer = direc;
  s_steera = 255;
}

int RC_steer2 (int direc ,int power){
  // Save
  s_steer = direc;
  s_steera = power;
}

//走行の関数
int RC_drive(int direc, int ipwm){
  //-- StartUP check
  if((s_speed == 0) ||
    (s_gear == FREE) ||
    (s_gear == BRAKE))
  {
      s_sptim = UP_TIME;
  }
  //-- No need StartUP
  if(ipwm > UP_SPEED){
      s_sptim = 0;   
  }
  // Save
  s_speed = ipwm;
  s_gear = direc;
}

//-- Motor Control Interrupt
void RC_run(){
//--  Steer Motor
  if ( s_steer == RIGHT ){
    RC_analogWrite(AIN1,s_steera);
    RC_analogWrite(AIN2,0);
  }else if ( s_steer == LEFT ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,s_steera);
  }else if ( s_steer == CENTER ){
    RC_analogWrite(AIN1,0);
    RC_analogWrite(AIN2,0);
  }
//--  Run Motor
  if ( s_gear == FREE ){
    RC_analogWrite(BIN1,0);
    RC_analogWrite(BIN2,0);
  }else if ( s_gear == REVERSE ){
    if(s_sptim == 0){
      RC_analogWrite(BIN1,0);
      RC_analogWrite(BIN2,s_speed);
    }else{
      RC_analogWrite(BIN1,0);
      RC_analogWrite(BIN2,UP_SPEED);
    }
  }else if ( s_gear == FORWARD ){
    if(s_sptim == 0){
      RC_analogWrite(BIN1,s_speed);
      RC_analogWrite(BIN2,0);
    }else{
      RC_analogWrite(BIN1,UP_SPEED);
      RC_analogWrite(BIN2,0);
    }
  }else if ( s_gear == BRAKE ){
    RC_analogWrite(BIN1,s_speed);
    RC_analogWrite(BIN2,s_speed);
  }
//--  Timer
  if(s_sptim != 0){ s_sptim --; }
}

//--  Setup RumiCar
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
#else   // Arduino
  // Timer0 変更
   TCCR0B &= B11111000;
  //  TCCR0B |= B00000011;                      // 64分周=  16M/  64/255=980Hz  #default
    TCCR0B |= B00000100;                      // 256分周= 16M/ 256/255=245Hz
  //  TCCR0B |= B00000101;                      // 1024分周=16M/1024/255=61Hz
  
  // Timer2 変更
   TCCR2B &= B11111000;
  //  TCCR2B |= B00000100;                      // 128分周=  16M/  128/255=490Hz  #default
  //  TCCR2B |= B00000101;                      // 256分周= 16M/ 256/255=245Hz
    TCCR2B |= B00000110;                      // 512分周= 16M/ 512/255=122Hz
  //  TCCR2B |= B00000111;                      // 2048分周= 16M/ 2048/255=30Hz
  
  // Arduino nano の場合
  // 分周比を変えてPWM周期を変える，Timer0のdelay()時間がずれるから注意
  // Timer1/2 はPWM周期変えてもdelay()に影響しない
  // RC_delay() を使うこと
#endif

  RC_analogWrite(AIN1, 0);
  RC_analogWrite(AIN2, 0);
  RC_analogWrite(BIN1, 0);
  RC_analogWrite(BIN2, 0);

//----  Timer One
  Timer1.initialize(10000);           // 10[ms]
  Timer1.attachInterrupt(RC_run);     // Motor Control
}
