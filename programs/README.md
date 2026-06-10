# RumiCar Simulator — 車種別 最速プログラム集 "Fable Racing Line"

[RumiCar Simulator](https://cpo.tokyo/rumicar/) 用に、各車種の物理特性を最大限に
引き出すよう調整した走行プログラムです。Arduino C++ 形式なので実機 RumiCar でも動作します。
tuned by **Fable 5**。

## 一覧
| プログラム | 車種 | 戦略 |
|---|---|---|
| **Apex Hunter** | ノーマル FR | // Apex Hunter — ノーマル FR 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
 |
| **Steady Nose** | ノーマル FF | // Steady Nose — ノーマル FF 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
 |
| **Quattro Blitz** | ノーマル 4WD | // Quattro Blitz — ノーマル 4WD 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
 |
| **Grip Reaper** | ドリフト FR | // Grip Reaper — ドリフト FR 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
 |
| **Rally Tamer** | ドリフト 4WD | // Rally Tamer — ドリフト 4WD 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
 |
| **Lift Whisper** | ドリフト FF | // Lift Whisper — ドリフト FF 用 最速プログラム  [Fable Racing Line]  tuned by Fable 5
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
