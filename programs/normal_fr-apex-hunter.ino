// Apex Hunter — ノーマル FR 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: バランス型FR。素直な回頭を活かし、やや早めの減速で向きを変えて立ち上がる。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ノーマル FR の物理特性 (config.js の DRIVE) を踏まえて】
//   FRは駆動(後輪)と操舵(前輪)が分かれるため最も素直に曲がる。回頭力が高い(yawGain 1.05)。
//   ただしアクセルONで後輪が滑りやすくパワーオーバーステア気味(powerOs 0.40)。コーナーで全開だと
//   リアが巻き込んで内側へ切れ込み、結果アウト側の壁にぶつかる。
//   → TCAP=195 に設定: 旋回中はアクセルを少しだけ抜き、巻き込みを抑える。FRは元々曲がるので
//      抜きすぎず中庸にし、回頭の良さで速さを稼ぐ。CRUISE/TOP は高め(220/252)で直線は速い。
//   D_TURN=372 とやや早めに減速判定: ブレーキで前荷重を作りつつ向きを変える「FRらしい」走り。
// この設定の狙い: 回頭の良さを活かして高い平均速度。巻き込みだけ TCAP で軽く抑える、攻めと安定のバランス型。

int TOP = 252;      // 直線の最高 PWM
int CRUISE = 220;   // 中速域の PWM
int SLOW = 130;     // コーナーの PWM
int TCAP = 195;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 620;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 405;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 372;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 180;   // 側方[mm] これ未満=壁から離れる

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
