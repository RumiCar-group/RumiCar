// Rally Tamer — ドリフト 4WD 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: ドリフト四駆を抑え込む。滑り出す直前(PWM<220)のグリップ上限を攻めて速く安定。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ドリフト 4WD の物理特性 (config.js の DRIVE) を踏まえて】
//   ドリフト4WDも PWM>220 で四輪ドリフトに入るが、四駆ゆえグリップ回復が速く(release 6.0)挙動が穏やか。
//   滑り自体も車体ごと流れる質で、FRドリフトのような鋭い巻き込みは少ない。つまり一番「御しやすい滑り」。
//   → Grip Reaper同様コーナーは 220 未満に抑えグリップで走るが、4WDの安定性を活かして TCAP=205 と
//      FRドリフトよりかなり高く設定。滑り出す一歩手前のグリップ上限を攻め、ドリフト車の中では速い。
//   D_MID=400 / D_SIDE=174 と4WDらしく減速は遅め・壁は近めまで使い、安定性を速さに変換する。
// この設定の狙い: 滑らせず、しかし限界ギリギリのグリップを使う。安定した四駆の美点を最大限に引き出す速い堅実型。

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
