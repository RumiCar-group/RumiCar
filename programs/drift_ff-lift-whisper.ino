// Lift Whisper — ドリフト FF 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: リフトオフの流れを抑え、荷重移動を穏やかに。速度を抑えめにしてリアの破綻を防ぐ。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ドリフト FF の物理特性 (config.js の DRIVE) を踏まえて】
//   ドリフトFFは特殊で、パワーでは滑らず「高速旋回中にアクセルを抜くと」リアだけ流れる(リフトオフ
//   オーバーステア)。つまり急なアクセルオフが牙になる。さらにFF由来のパワーアンダーも併せ持つ。
//   → 二律背反(踏めばアンダー/抜けばリアが流れる)を避けるには「そもそも速度を上げすぎない」のが要。
//      滑り出しは正規化速度 sp≥0.45 が条件なので、CRUISE/SLOW を低め(197/116)に保ち、急な減速で
//      リアを飛ばさないようにする。TCAP=170 は穏やかなアクセル変化で荷重移動を急がせない狙い。
//   D_MID=442 / D_SIDE=192 と早め減速・広い壁マージン: FF系らしく安全第一で破綻を未然に防ぐ。
// この設定の狙い: リフトオフの落とし穴を避け、速度と荷重移動を穏やかに保つ。最も繊細さを要する車を優しく御す型。

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
