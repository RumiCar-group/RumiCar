// Rally Tamer — ドリフト 4WD 用 最速プログラム  [Fable Racing Line]
// 戦略: ドリフト四駆を抑え込む。滑り出す直前(PWM<220)のグリップ上限を攻めて速く安定。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

int TOP = 255;      // 直線の最高 PWM
int CRUISE = 218;   // 中速域の PWM
int SLOW = 145;     // コーナーの PWM
int TCAP = 205;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 640;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 400;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 374;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 174;   // 側方[mm] これ未満=壁から離れる

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
