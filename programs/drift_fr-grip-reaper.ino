// Grip Reaper — ドリフト FR 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
// 戦略: ドリフトFRをあえてグリップで走らせる。旋回中は大きくアクセルを抜き巻き込みを封印。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。
//
// 【なぜこの設定にしたか — ドリフト FR の物理特性 (config.js の DRIVE) を踏まえて】
//   ドリフトFRは旋回中に PWM>220 で後輪が滑り出す(パワースライド)。滑りは派手だが3値操舵では
//   制御が難しく、壁に向かって突っ込みやすい。最速かつ無衝突を狙うなら「あえて滑らせない」が正解。
//   → コーナー側の PWM を常に 220 未満に抑え、滑り出しトリガーを踏まないグリップ走行にした。
//      加えて素のオーバーステア(powerOs 0.40)も強いので、TCAP=150 と全車で最も低く設定し、
//      旋回中は大きくアクセルを抜いて巻き込み(過剰な回頭)を封印する。これが効いて完走数が激増した。
//   D_SIDE=190 と壁マージンも広め: オーバーステアで姿勢が乱れても早めの補正で立て直す。
// この設定の狙い: 「滑らせれば速い」の誘惑を捨て、滑り出す限界の直前のグリップを使い切る玄人志向の堅実ドリフト。

int TOP = 248;      // 直線の最高 PWM
int CRUISE = 205;   // 中速域の PWM
int SLOW = 120;     // コーナーの PWM
int TCAP = 150;     // 旋回中の上限 PWM (低いほどアクセルを抜く)
int D_OPEN = 620;   // 前方[mm] これ以上=直線とみなし全開
int D_MID = 420;    // 前方[mm] これ以上=中速、未満=減速
int D_TURN = 385;   // 前方[mm] これ未満=広い方へ曲げる
int D_SIDE = 190;   // 側方[mm] これ未満=壁から離れる

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
