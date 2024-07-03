#ifndef RumiCar_h
#define RumiCar_h

// RumiCar include
//操舵用の設定
#define LEFT   0
#define CENTER 1
#define RIGHT  2

//走行用の設定
#define FREE    0
#define REVERSE 1
#define FORWARD 2
#define BRAKE   3

#if defined ESP32
#define RC_analogWrite ledcWrite
#define SHUT0 19
#define SHUT1 18
#define SHUT2 05

//Aが操舵、Bが走行
#define AIN1 04
#define AIN2 26
#define BIN1 27
#define BIN2 25

#elif defined (ARDUINO_ARCH_SPRESENSE)
#define RC_analogWrite analogWrite
#define SHUT0 2
#define SHUT1 7
#define SHUT2 41

//Aが操舵、Bが走行
#define AIN1 6
#define AIN2 5
#define BIN1 9
#define BIN2 3

#else
#define RC_analogWrite analogWrite
//SHUTはディジタルピン,A0はD14,以降同じ、注意すること
#define SHUT0 14
#define SHUT1 15
#define SHUT2 16

//Aが操舵、Bが走行
#define AIN1 03
#define AIN2 11
#define BIN1 05
#define BIN2 06
#endif

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

#define HIGH_SPEED
//#define HIGH_ACCURACY

#include <VL53L0X.h>
extern VL53L0X sensor0;
extern VL53L0X sensor1;
extern VL53L0X sensor2;

void RC_setup();  //RumiCarのセンサとモータの初期化
int RC_steer (int direc );  //操舵の関数
int RC_drive(int direc, int ipwm);  //走行の関数

#endif /* RumiCar_h */
