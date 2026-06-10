// Steady Nose — ノーマル FF 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: 前輪駆動の安定したブレーキを活かし、最も早く減速。アンダーを出さず確実に通す。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ノーマル FF の物理特性 (config.js の DRIVE) を踏まえて】
//   FFは前輪が「駆動」と「操舵」を兼ねる。コーナーでアクセルを踏むと前輪が駆動力に取られて
//   曲がる力が不足し、強いアンダーステア=外へ膨らむ(us 0.45, powerOs相当の powerUs 0.50)。
//   さらに発進時は前輪が空転しやすい(spin 0.45)。一方エンジンが前にあり制動は安定(brake 1.08)。
//   → 弱点(パワーアンダー)を避け、強み(安定ブレーキ)を使う設計にした。
//      TCAP=166 と最も低く: コーナーでしっかりアクセルを抜き、前輪に曲げる仕事をさせる。
//      D_MID=444 / D_TURN=404 と最も早く減速判定し、CRUISE/SLOW も最低(196/114)。
//   D_SIDE=192 と壁マージンも最大: アンダーで膨らみがちな分、早めに壁から離れて余裕を持つ。
// この設定の狙い: アンダーで壁に刺さらないことを最優先。速さは控えめだが堅実に完走する安全運転型。

int TOP = 246;      // 直線の最高 PWM
int CRUISE = 196;   // 中速域の PWM
int SLOW = 114;     // コーナーの PWM
int TCAP = 166;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 600;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 444;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 404;   // 前方[mm] これ未満=広い方へ曲げる
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
