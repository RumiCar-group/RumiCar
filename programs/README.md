# RumiCar Simulator — 車種別 最速プログラム集 "Fable Racing Line"

[RumiCar Simulator](https://cpo.tokyo/rumicar/) 用に、各車種の物理特性を最大限に
引き出すよう調整した走行プログラムです。Arduino C++ 形式なので実機 RumiCar でも動作します。
tuned by **Fable 5**。

## 一覧
| プログラム | 車種 | 戦略 |
|---|---|---|
| **Apex Hunter** | ノーマル FR | // Apex Hunter — ノーマル FR 用 最速プログラム  [Fable Racing Line]
// 戦略: バランス型FR。素直な回頭を活かし、やや早めの減速で向きを変えて立ち上がる。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

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
 |
| **Steady Nose** | ノーマル FF | // Steady Nose — ノーマル FF 用 最速プログラム  [Fable Racing Line]
// 戦略: 前輪駆動の安定したブレーキを活かし、最も早く減速。アンダーを出さず確実に通す。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

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
 |
| **Quattro Blitz** | ノーマル 4WD | // Quattro Blitz — ノーマル 4WD 用 最速プログラム  [Fable Racing Line]
// 戦略: 四駆のトラクションと安定性で最速。全開区間を最大化し、コーナーも高めの速度で。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

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
 |
| **Grip Reaper** | ドリフト FR | // Grip Reaper — ドリフト FR 用 最速プログラム  [Fable Racing Line]
// 戦略: ドリフトFRをあえてグリップで走らせる。旋回中は大きくアクセルを抜き巻き込みを封印。
// 前方の空き具合で速度を3段に変え、詰まれば広い方へ全力で切る適応走行。
// 旋回中は TCAP まで自動でアクセルを抜き、巻き込み(オーバーステア)を抑える。
// 単純コースは直線で全開、カーブ主体コースは早めの減速で破綻を防ぐ。

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
 |
| **Rally Tamer** | ドリフト 4WD | // Rally Tamer — ドリフト 4WD 用 最速プログラム  [Fable Racing Line]
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
 |
| **Lift Whisper** | ドリフト FF | // Lift Whisper — ドリフト FF 用 最速プログラム  [Fable Racing Line]
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
 |

## 共通アルゴリズム
前方センサーの空き具合で速度を3段 (直線=全開 / 中速 / コーナー=減速) に変え、前方が
詰まれば左右で広い方へ全力操舵、側方が近ければ離れます。さらに**旋回中は TCAP まで
自動でアクセルを抜き**、巻き込み(オーバーステア)を抑えます。車種ごとの強み・弱みは
冒頭の定数 (TOP/CRUISE/SLOW/TCAP/D_*) の違いだけで表現しています。

- 単純コース → 直線で全開区間が伸びる
- カーブ主体コース → 前方が頻繁に詰まり自動的に減速側へ寄る

## 車種ごとの考え方
- **Quattro Blitz (4WD)**: トラクションと安定性で最速。コーナーも高い PWM で。
- **Apex Hunter (FR)**: 素直な回頭を活かしつつ、やや早めの減速で向きを変える。
- **Steady Nose (FF)**: 安定したブレーキを活かし最も早く減速。アンダーを出さない。
- **Grip Reaper (ドリフトFR)**: あえてグリップ走行。旋回中に大きくアクセルを抜き巻き込みを封印。
- **Rally Tamer (ドリフト4WD)**: 滑り出す直前 (PWM<220) のグリップ上限を攻める。
- **Lift Whisper (ドリフトFF)**: リフトオフの流れを抑え、荷重移動を穏やかに。

## 使い方
シミュレータでは車種を選ぶと自動でその車のプログラムが読み込まれます。別の車種用の
プログラムを選んで他の車に入れ、どれだけ挙動が変わるかを比較することもできます
(攻めの 4WD 用プログラムを FF 車に入れるとアンダーで膨らむ、等)。
