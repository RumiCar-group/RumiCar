// Steady Nose — ノーマル FF 用  [Fable Racing Line / Lv3 弱点を補う]  by Fable 5
// 試走(全30ドライ周回): 23/30 完走・平均 1.64 km/h。乾いた路面ではFFは構造上やや遅いが安定。
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

void setup() { RC_setup(); }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;

  int steering = 1;   // 1=今ハンドルを切っている, 0=まっすぐ
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); steering = 0; }

  // ★FFの肝★ 操舵中はアクセルを抜き(前輪を操舵に専念)、まっすぐなら前方の空きで加速する。
  int pwm;
  if (steering == 1) pwm = TURN_PWM;
  else if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  else pwm = SLOW;
  RC_drive(FORWARD, pwm);
}
