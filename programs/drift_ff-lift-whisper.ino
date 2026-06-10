// Lift Whisper — ドリフト FF 用 最速プログラム  [Fable Racing Line]
// 戦略: リフトオフの流れを抑え、荷重移動を穏やかに。速度を抑えめにしてリアの破綻を防ぐ。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

int TOP = 246;      // 直線の最高 PWM
int CRUISE = 197;   // 中速域の PWM
int SLOW = 116;     // コーナーの PWM
int TCAP = 170;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 600;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 442;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 402;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 192;   // 側方[mm] これ未満=壁から離れる

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
