// Steady Nose — ノーマル FF 用  [Fable Racing Line / Lv3 弱点を補う]  by Fable 5
// 実測 (v6.0.0・同条件): 31/39 完走。乾いた路面ではFFは構造上やや遅いが安定。
//   ※FFの真価は雨(低グリップ)。攻めるとテールの出る車は自滅するが、アンダーのFFは生き残る。
// FFは前輪が「駆動」と「操舵」を兼ねる。曲げながらアクセルを踏むと前輪が駆動に取られて
// 曲がる力を失い、外へ膨らむ(パワーアンダー, powerUs=0.50)。発進空転も大きい(spin=0.45)。
// 一方エンジンが前にあり制動は安定(brake=1.08)。
// ★引き出すロジック(基準と“決定的に違う”点)★
//   「操舵している間はアクセルを抜く」。前輪に“曲げる仕事”を専念させ、まっすぐで踏む。
//   速度を前方距離だけでなく『今ハンドルを切っているか』で決めるのが核。これがFFの肝。

int TOP=250, CRUISE=205, SLOW=120, TURN_PWM=120;   // TURN_PWM=操舵中に抜く先の低PWM
int D_OPEN=600, D_MID=440, D_TURN=400, D_SIDE=190;
int CONF=640, OPEN=9999;   // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。実機 VL53L0X は地面拘束で
// ~250mm 超を信頼しにくい。本シム卓上は実機模型の約2.5倍広いので実スケール換算 ~640mm を既定にする。
// ★実機へ移すときは自機の車体/搭載高に合わせて下げ(目安250mm)速度も落とす。fullscale は 250×領域スケール。

int ESC=110, STUCK=1, BACK=5, SETTLE=4;   // [脱出] 基準FRと同じ土台。詰まり続けたら BACK 回後退→SETTLE 回落ち着き。
int prevC; int seen; int stuckT; int escBack; int escRec; int escDir;
// FF は前輪が駆動と操舵を兼ねるぶん曲がりきれず詰まりやすいので切り返しの効きが大きい。
// ★掠めただけでは発火させない★ =「近い」かつ「開かない」が続いたときだけ行き止まりとみなす。
// ★後退中は舵を固定する★ =左右にぶれて同じ場所を往復するのを防ぐ。

void setup() { RC_setup(); prevC = 0; seen = 0; stuckT = 0; escBack = 0; escRec = 0; escDir = CENTER; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;

  // ★行き止まりからの脱出★ 詰まり続けたら、開いている側へ鼻先が向くよう“逆ハンで後退”する。
  int dC = 0; if (seen == 1) dC = C - prevC; prevC = C; seen = 1;   // dC<=0 = 前方が開いてこない
  if (C < ESC && dC <= 0) stuckT = stuckT + 1;   // 「近い」かつ「開かない」が続いた回数
  if (C >= ESC) stuckT = 0;                      // 前が開いたら解除
  if (escBack > 0) { escBack = escBack - 1; RC_steer(escDir); RC_drive(REVERSE, 170); return; }
  if (escRec > 0) { escRec = escRec - 1; RC_steer(CENTER); RC_drive(FORWARD, 90); return; }  // 脱出直後は落ち着かせる
  if (stuckT >= STUCK) {
    if (L > R) escDir = RIGHT; else escDir = LEFT;
    escBack = BACK; escRec = SETTLE; stuckT = 0;
    RC_steer(escDir); RC_drive(REVERSE, 170); return;
  }

  int steering = 1;   // 1=今ハンドルを切っている, 0=まっすぐ
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (L < D_SIDE || R < D_SIDE) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }   // 近い方の壁から離れる
  else { RC_steer(CENTER); steering = 0; }

  // ★FFの肝★ 操舵中はアクセルを抜き(前輪を操舵に専念)、まっすぐなら前方の空きで加速する。
  int pwm;
  if (steering == 1) pwm = TURN_PWM;
  else if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  else pwm = SLOW;
  RC_drive(FORWARD, pwm);
}
