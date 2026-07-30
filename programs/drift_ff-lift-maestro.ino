// Lift Maestro — ドリフト FF 用  [Fable Racing Line / Lv6 特殊を管理+技を切替]  by Fable 5
// 試走(ドライ31コース×60秒): 25/31 完走・タイト4/8・総距離 698m (旧Lift Whisper: 23/31・655m)。
// ドリフトFFは特殊。パワーでは滑らず「高速旋回中にアクセルを“急に”抜くと」リアが流れる
// (リフトオフ・オーバーステア)。だから旧版の肝 = スロットルのレート制限 (RAMP) は維持し、
// そこへ Drift Maestro の [脱出]/[出口]/[防御] を足した。
// ※ [防御] のドリフトウォールは FF でも有効: 振り出し→戻しの減速がリフトオフを誘発し
//   テールが流れて車幅を塞ぐ (FF はこの“抜いて滑らせる”が唯一のドリフト)。

int TOP=234, CRUISE=184, SLOW=114, TCAP=158;        // リフトオフでの過剰な滑り出しを誘発しない上限
int D_OPEN=605, D_MID=458, D_TURN=410, D_SIDE=196;
int CONF=640, OPEN=9999;   // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。実機 VL53L0X は地面拘束で
// ~250mm 超を信頼しにくい。本シム卓上は実機模型の約2.5倍広いので実スケール換算 ~640mm を既定にする。
// ★実機へ移すときは自機の車体/搭載高に合わせて下げ(目安250mm)速度も落とす。fullscale は 250×領域スケール。
int RAMP=10;         // 1ループで下げてよいPWM上限 (急リフト禁止 = リフトオフドリフト防止)
int ESC=90;          // [脱出] 前方がこれ未満=行き止まり
int GUARD=250;       // [出口] 内側壁がこれ未満なら加速を我慢
int DEF_STYLE=2;     // [防御] 0=なし 1=ライン占有 2=ドリフトウォール
int prevPwm; int defT; int defOn; int prevTurn;

void setup() { RC_setup(); prevPwm=0; defT=0; defOn=0; prevTurn=0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;

  // [脱出] 行き止まり: 開いている側へ鼻先が向くよう逆ハンで後退
  if (C < ESC) { if (L > R) RC_steer(RIGHT); else RC_steer(LEFT); RC_drive(REVERSE, 170); prevPwm = 0; return; }

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

  // [通常] グリップ走行 + リフトオフ回避 (旧版の肝)
  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); turning = 0; }

  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  if (prevTurn == 1 && turning == 0 && pwm > CRUISE) pwm = CRUISE;  // 出口で急に踏まない
  prevTurn = turning;

  // [出口] レイトアペックス: 前が開いても内側壁が近い間は CRUISE 止め + 外へ当て舵
  if (pwm > CRUISE) {
    if (R < GUARD && R < L) { RC_steer(LEFT); pwm = CRUISE; }
    else if (L < GUARD && L < R) { RC_steer(RIGHT); pwm = CRUISE; }
  }

  // ★リフトオフ回避★ アクセルを“急に”抜かない。減らす時は1ループ RAMP までに制限。
  if (pwm < prevPwm - RAMP) pwm = prevPwm - RAMP;
  prevPwm = pwm;
  RC_drive(FORWARD, pwm);
}
