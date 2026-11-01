// Traction Blitz — ノーマル 4WD 用  [Fable Racing Line / Lv2 強みを使う]  by Fable 5
// 実測 (v6.0.0・同条件): 35/39 完走。最高速級。
// 4WDは四輪で路面を掴み発進空転がほぼ無く(spin=0)、加速が最強(accel=1.25)・最高速も上。
// 弱点はやや重くアンダー寄り(massが大)。
// ★引き出すロジック★ 他車にはできない「コーナー出口で“誰より早く”フル加速」。
//   前方距離 C の前回との差 dC を取り、前が開き出した瞬間(dC>0=立ち上がり)に全開へ。
//   FR/FFが同じことをすると空転やオーバー/アンダーで膨らむが、4WDはトラクションで決まる。
// 基準(FR)との違い: 速度3段は同じ。そこに「立ち上がり検知 → 即全開」を足しただけ。

int TOP=235, CRUISE=215, SLOW=140, TCAP=208;  // 動力学モデル較正: 255/226/150 は制動が
                                              // 間に合わず刺さる (kin でも 235/215/140 の方が完走+1)
int D_OPEN=645, D_MID=410, D_TURN=386, D_SIDE=178;
int CONF=640, OPEN=9999;   // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。実機 VL53L0X は地面拘束で
// ~250mm 超を信頼しにくい。本シム卓上は実機模型の約2.5倍広いので実スケール換算 ~640mm を既定にする。
// ★実機へ移すときは自機の車体/搭載高に合わせて下げ(目安250mm)速度も落とす。fullscale は 250×領域スケール。
int EXIT=18;            // 前方が1ループでこの[mm]以上開いたら=コーナー脱出 → 立ち上がり全開
int ESC=100, STUCK=1, BACK=7, SETTLE=6;   // [脱出] 基準FRと同じ土台。詰まり続けたら BACK 回後退→SETTLE 回落ち着き。
int stuckT; int escBack; int escRec; int escDir;   //   掠めただけでは発火させず、後退中は舵を固定する。
int prevC; int started; // 前回の前方距離 (差分=接近/開きの検知に使う)

void setup() { RC_setup(); prevC = 0; started = 0; stuckT = 0; escBack = 0; escRec = 0; escDir = CENTER; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;
  int dC = 0;
  if (started == 1) dC = C - prevC;   // dC>0 = 前方が開いてくる = コーナーの立ち上がり
  prevC = C; started = 1;
  // ★行き止まりからの脱出★ 詰まり続けたら開いている側へ鼻先を向けて後退 (立ち上がり検知は持ち越さない)
  if (C < ESC && dC <= 0) stuckT = stuckT + 1;   // 「近い」かつ「開かない」が続いた回数
  if (C >= ESC) stuckT = 0;                      // 前が開いたら解除
  if (escBack > 0) { escBack = escBack - 1; RC_steer(escDir); RC_drive(REVERSE, 170); started = 0; return; }
  if (escRec > 0) { escRec = escRec - 1; RC_steer(CENTER); RC_drive(FORWARD, 90); started = 0; return; }
  if (stuckT >= STUCK) {
    if (L > R) escDir = RIGHT; else escDir = LEFT;
    escBack = BACK; escRec = SETTLE; stuckT = 0; started = 0;
    RC_steer(escDir); RC_drive(REVERSE, 170); return;
  }

  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (L < D_SIDE || R < D_SIDE) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }   // 近い方の壁から離れる
  else { RC_steer(CENTER); turning = 0; }

  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  // ★立ち上がり全開★ コーナーを抜け前方が開き出したら、トラクションを信じて即フルパワー
  if (dC > EXIT && C > D_MID) pwm = TOP;
  RC_drive(FORWARD, pwm);
}
