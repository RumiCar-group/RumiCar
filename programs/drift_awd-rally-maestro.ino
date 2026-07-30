// Rally Maestro — ドリフト 4WD 用  [Fable Racing Line / Lv5 技を切替+定数攻め]  by Fable 5
// 試走(ドライ31コース×60秒): standard 30/31 完走 / 動力学モデル 31/31 (旧Rally Tamer: 26/31)。
// 4WDはドリフトFRと違い slipYaw が小さく (0.25)、滑らせても回頭がほぼ増えない=
// “滑らせるドリフト”は4WDでは速さに直結しない (実測でブレーキドリフトは逆効果)。
// そこで Drift Maestro から [回頭] を外し、[脱出]/[出口]/[防御] と
// 「重い4WD向けの定数」(早めに曲げ始める D_TURN=470・低めの TCAP=135・広い GUARD=340) で攻める。
// 学び: 同じ技セットでも“どの技を有効にするか”と“定数”は車の物理で決まる。

int TOP=248, CRUISE=205, SLOW=100, TCAP=135;        // 重い4WD: 旋回はやや抑えめが速い
                     // SLOW は 100: 動力学モデルは旋回中 β≈10° 外へ流れて対地ラインが
                     // 膨らむため、旋回速度を落として締める (kin は 120 と完走数同じ)
int D_OPEN=620, D_MID=420, D_TURN=470, D_SIDE=190;  // 慣性が大きい分、早めに曲げ始める
int CONF=640, OPEN=9999;   // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。実機 VL53L0X は地面拘束で
// ~250mm 超を信頼しにくい。本シム卓上は実機模型の約2.5倍広いので実スケール換算 ~640mm を既定にする。
// ★実機へ移すときは自機の車体/搭載高に合わせて下げ(目安250mm)速度も落とす。fullscale は 250×領域スケール。
int ESC=90;          // [脱出] 前方がこれ未満=行き止まり
int GUARD=340;       // [出口] 重さで膨らむ分、ガードを広めに
int DEF_STYLE=2;     // [防御] 0=なし 1=ライン占有 2=ドリフトウォール
int prevTurn; int defT; int defOn;

void setup() { RC_setup(); prevTurn=0; defT=0; defOn=0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;

  // [脱出] 行き止まり: 開いている側へ鼻先が向くよう逆ハンで後退
  if (C < ESC) { if (L > R) RC_steer(RIGHT); else RC_steer(LEFT); RC_drive(REVERSE, 170); return; }

  // [防御] 後方に追走車 → 約3秒防御を維持
  int B = RC_read(BACK);
  if (B >= 0 && B < 650) defOn = 60; else if (defOn > 0) defOn = defOn - 1;
  defT = defT + 1;
  if (DEF_STYLE > 0 && defOn > 0) {
    if (C < 380) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); RC_drive(FORWARD, 150); return; }
    if (R < 175) { RC_steer(LEFT); RC_drive(FORWARD, 120); return; }
    if (L < 175) { RC_steer(RIGHT); RC_drive(FORWARD, 120); return; }
    if (DEF_STYLE == 2) {
      int ph = defT % 24;
      if (ph < 6) { RC_steer(LEFT); RC_drive(FORWARD, 210); }
      else if (ph < 12) { RC_steer(RIGHT); RC_drive(FORWARD, 120); }
      else if (ph < 18) { RC_steer(RIGHT); RC_drive(FORWARD, 210); }
      else { RC_steer(LEFT); RC_drive(FORWARD, 120); }
      return;
    }
    RC_steer(CENTER); RC_drive(FORWARD, 150); return;
  }

  // [通常] グリップ走行
  int t = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); t = 0; }
  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (t == 1 && pwm > TCAP) pwm = TCAP;
  if (prevTurn == 1 && t == 0 && pwm > CRUISE) pwm = CRUISE;
  prevTurn = t;

  // [出口] レイトアペックス: 前が開いても内側壁が近い間は CRUISE 止め + 外へ当て舵
  if (pwm > CRUISE) {
    if (R < GUARD && R < L) { RC_steer(LEFT); pwm = CRUISE; }
    else if (L < GUARD && L < R) { RC_steer(RIGHT); pwm = CRUISE; }
  }
  RC_drive(FORWARD, pwm);
}
