// Quattro Blitz — ノーマル 4WD 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: 四駆のトラクションと安定性で最速。全開区間を最大化し、コーナーも高めの速度で。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ノーマル 4WD の物理特性 (config.js の DRIVE) を踏まえて】
//   4WDは四輪で路面を掴むため発進空転がほぼ無く(spin 0)、加速が最強(accel 1.25)・最高速も上(maxSpeed 1.06)。
//   アンダー/オーバーが小さく挙動が最も素直で安定(us 0.30, os 0)。つまり「速く走らせても破綻しにくい」。
//   → この強みをそのまま速さに変換: TOP=255(全開)、CRUISE=235、TCAP=225 と全車中で最も高く設定。
//      旋回中もアクセルをあまり抜かず、トラクションを信じてコーナーを高い速度で通過する。
//   D_MID=395 / D_TURN=372 と減速判定は最も遅め(ブレーキを我慢)。安定しているから攻められる。
//   D_SIDE=172 と壁マージンは最小: 正確に走れるので壁ギリギリまで使い、最短・最速ラインを取る。
// この設定の狙い: 6車種で最速。トラクションと安定性を全面的に攻めに使い、平均速度を最大化する本命。

int TOP = 255;      // 直線の最高 PWM
int CRUISE = 235;   // 中速域の PWM
int SLOW = 150;     // コーナーの PWM
int TCAP = 225;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 645;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 395;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 372;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 172;   // 側方[mm] これ未満=壁から離れる

void setup() {
  RC_setup();
}

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); // 左
  int C = sensor1.readRangeSingleMillimeters(); // 中央
  int R = sensor2.readRangeSingleMillimeters(); // 右

  // 操舵 (左右3値): 前が詰まれば広い方へ全力、側方が近ければ離れる
  int turning = 1;
  if (C < D_TURN) {
    if (L > R) RC_steer(LEFT);
    else       RC_steer(RIGHT);
  } else if (R < D_SIDE) {
    RC_steer(LEFT);
  } else if (L < D_SIDE) {
    RC_steer(RIGHT);
  } else {
    RC_steer(CENTER);
    turning = 0;
  }

  // 速度: 前方が開けているほど速く。旋回中は TCAP までアクセルを抜く。
  int pwm = SLOW;
  if (C > D_OPEN)      pwm = TOP;
  else if (C > D_MID)  pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  RC_drive(FORWARD, pwm);
}
