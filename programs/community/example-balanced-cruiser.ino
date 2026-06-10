// Balanced Cruiser — コミュニティ投稿の例
// Apex Hunter (ノーマルFR) をベースに、どの車種でも無難に走るよう速度を控えめ・
// 壁マージンを広めにした安全寄りの設定。シミュレータの「🌐 みんなの投稿」に並びます。
// 改変して走らせ、納得したら自分の名前で GitHub に保存してみてください。

int TOP = 240;      // 直線の最高 PWM
int CRUISE = 200;   // 中速域の PWM
int SLOW = 125;     // コーナーの PWM
int TCAP = 180;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 620;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 420;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 390;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 188;   // 側方[mm] これ未満=壁から離れる

void setup() {
  RC_setup();
}

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

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

  int pwm = SLOW;
  if (C > D_OPEN)      pwm = TOP;
  else if (C > D_MID)  pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  RC_drive(FORWARD, pwm);
}
