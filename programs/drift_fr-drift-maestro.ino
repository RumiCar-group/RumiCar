// Drift Maestro — ドリフト FR 用  [Fable Racing Line / Lv4 状況で技を切替]  by Fable 5
// 試走(ドライ31コース×60秒): standard 31/31 完走 / 動力学モデル 30/31 (旧Grip Reaper: 28/31)。
// ウェット2コース (grip0.5/0.55) も両モード完走 ([TC] トラクション管理による)。
// ドリフトは「滑らせるほど速い」のではない。平坦ドライでは滑り=外への横流れ=ロスで、
// 基本はグリップが速い (滑る手前 TCAP の自制は旧版と同じ)。速さの源は“技の使い分け”:
//   [脱出] 前方が壁ぴったり → 開いた側へ鼻先を振る逆ハン後退 (ドリフトの切り返し/180°ターンの応用)
//   [出口] 出口で前が開いても内側壁が近い間は加速を我慢+外へ当て舵 (レイトアペックス)。
//          出口の壁刺さり(ストリート等のクラッシュ原因)はほぼこれで消える。
//   [回頭] 深いコーナーで進入速度過多 (前方が閉じ続ける=下り等) の時だけブレーキドリフト:
//          BRAKE で前荷重→リアが流れ、“減速しながら回頭”する。滑りが合理的なのはここ。
//   [TC]   アクセルは踏み足し上限つき (トラクション管理)。一気に踏むと駆動が摩擦を食い尽くし
//          “直進中なのに横が消える”——ウェット路面 (grip0.5) では発進加速だけで独楽スピンになる
//          (実測)。踏み足しを絞ればウェットでも横グリップが残り、ドライの最高速は変わらない。
//          雨をセンサーで当てにいく案 (舵と逆側レイの低下検知) はドライのコーナーでも同じ
//          見え方が頻発して判別不能だった——“雨でも死なない踏み方を常にする”が正解。
//   [防御] 後方センサー(任意装備・OFFなら無効)が追走車を検知 → ライン占有 or ドリフトウォール
//          (テールを左右に流して車幅で塞ぐ。広い直線では追走車を 100%→4% に完封する唯一の技)。

int TOP=225, CRUISE=205, SLOW=120, TCAP=150;        // 速度3段 + 旋回中の上限 (滑る手前)
                     // TOP は 225 に抑える: 動力学モデルはモーターブレーキが後輪のみ
                     // (減速 ≈1.4-1.9 m/s²) で、248 だとコーナー出現に減速が間に合わない
int D_OPEN=620, D_MID=420, D_TURN=385, D_SIDE=190;  // 判定距離[mm]
int CONF=640, OPEN=9999;   // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。実機 VL53L0X は地面拘束で
// ~250mm 超を信頼しにくい。本シム卓上は実機模型の約2.5倍広いので実スケール換算 ~640mm を既定にする。
// ★実機へ移すときは自機の車体/搭載高に合わせて下げ(目安250mm)速度も落とす。fullscale は 250×領域スケール。
int ESC=90;          // [脱出] 前方がこれ未満=行き止まり
int GUARD=250;       // [出口] 内側壁がこれ未満なら加速を我慢
int TRIG_T=6;        // [回頭] 旋回がこの tick 数続く=深いコーナー
int DCF=24;          // [回頭] 1tick にこれ[mm]以上閉じ続ける=進入速度過多
int BURST=2, GAP=8;  // [回頭] ブレーキドリフトのデューティ (BRAKE 2 / 前進 8)
int RAMP=5;          // [TC] 1tick の踏み足し上限 (PWM)。+5/tick ≈ 0.29m/s² の加速要求で、
                     //      ウェット(grip0.5)の駆動上限 ≈0.86m/s² の1/3=横グリップを残して加速。
                     //      掃引実測 (5/6/8/10/12/15/20/∞): 5 だけが ドライ/ウェット/峠 全勝
                     //      (緩すぎても単コースは軌道カオスで悪化しうる。総合で判断)
int DEF_STYLE=2;     // [防御] 0=なし 1=ライン占有(狭路向き) 2=ドリフトウォール(広い直線向き)
int turnT; int prevTurn; int pC; int started; int fastT; int defT; int defOn;
int spdT; int escRec; int cur;

void setup() { RC_setup(); turnT=0; prevTurn=0; pC=0; started=0; fastT=0; defT=0; defOn=0; spdT=0; escRec=0; cur=0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;
  int dC = 0; if (started == 1) dC = C - pC; pC = C; started = 1;
  // 速度の目安: 前が開いていた直後だけ「速い」(低速の定常旋回ではレイ掃引で dC が
  // 急減して [回頭] が誤発火する — kin では無害だが dyn ではパワーオーバーで刺さる)
  if (C > D_MID) spdT = 10; else if (spdT > 0) spdT = spdT - 1;

  // [脱出] 行き止まり: 開いている側へ鼻先が向くよう逆ハンで後退 (壁前38mmでも生還)
  if (C < ESC) { if (L > R) RC_steer(RIGHT); else RC_steer(LEFT); RC_drive(REVERSE, 170); turnT = 0; escRec = 4; cur = 0; return; }
  // [脱出] 直後はタイヤを落ち着かせてから通常制御へ (dyn での前後往復スピンを断つ)
  if (escRec > 0) { escRec = escRec - 1; RC_steer(CENTER); RC_drive(FORWARD, 90); cur = 90; return; }

  // [防御] 後方に追走車 → 約3秒防御を維持 (横に並ばれ後方レイが見失っても解かない)
  int B = RC_read(BACK);
  if (B >= 0 && B < 650) defOn = 60; else if (defOn > 0) defOn = defOn - 1;
  defT = defT + 1;
  if (DEF_STYLE > 0 && defOn > 0) {
    if (C < 380) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); RC_drive(FORWARD, 150); cur = 150; return; }
    if (R < 175) { RC_steer(LEFT); RC_drive(FORWARD, 120); cur = 120; return; }
    if (L < 175) { RC_steer(RIGHT); RC_drive(FORWARD, 120); cur = 120; return; }
    if (DEF_STYLE == 2) {  // ドリフトウォール: 振り出し→逆ハンの繰り返しでテールを流し車幅を塞ぐ
      int ph = defT % 24;
      if (ph < 6) { RC_steer(LEFT); RC_drive(FORWARD, 210); cur = 210; }
      else if (ph < 12) { RC_steer(RIGHT); RC_drive(FORWARD, 120); cur = 120; }
      else if (ph < 18) { RC_steer(RIGHT); RC_drive(FORWARD, 210); cur = 210; }
      else { RC_steer(LEFT); RC_drive(FORWARD, 120); cur = 120; }
      return;
    }
    RC_steer(CENTER); RC_drive(FORWARD, 150); cur = 150; return;   // ライン占有 (低速で前を塞ぐ)
  }

  // [通常] グリップ走行: 3段速度 + 旋回中は滑る手前で頭打ち + 出口で急に踏まない (旧版と同じ)
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

  // [回頭] 深いコーナー & 速度過多が続く → ブレーキドリフト (減速しつつリアを流して回頭)
  if (t == 1 && C < D_TURN) turnT = turnT + 1; else turnT = 0;
  if (t == 1 && dC < 0 - DCF) fastT = fastT + 1; else fastT = 0;
  if (turnT >= TRIG_T && fastT >= 2 && spdT > 0) {
    int ph2 = (turnT - TRIG_T) % (BURST + GAP);
    if (ph2 < BURST) { RC_drive(BRAKE, 255); cur = cur - 40; if (cur < 60) cur = 60; return; }
    int rp = 205;
    if (rp > cur + RAMP) rp = cur + RAMP;
    cur = rp; RC_drive(FORWARD, rp); return;
  }

  // [出口] レイトアペックス: 前が開いても内側壁が近い間は CRUISE 止め + 外へ当て舵
  if (pwm > CRUISE) {
    if (R < GUARD && R < L) { RC_steer(LEFT); pwm = CRUISE; }
    else if (L < GUARD && L < R) { RC_steer(RIGHT); pwm = CRUISE; }
  }
  // [TC] 踏み足しはスルーレート制限 (アクセルを戻す側は即時)
  if (pwm > cur + RAMP) pwm = cur + RAMP;
  cur = pwm;
  RC_drive(FORWARD, pwm);
}
