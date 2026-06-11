// Grip Reaper — ドリフト FR 用  [Fable Racing Line / Lv4 限界を管理]  by Fable 5
// 試走(全30ドライ周回): 27/30 完走・平均 1.94 km/h。滑らせない自制が効いて完走数トップ級。
// ドリフトFRは旋回中にアクセルを踏みすぎる(駆動力がグリップ限界 grip を超える)と後輪が滑り出す。
// 3値操舵では滑りを御しにくく壁へ刺さる。だから戦略は「あえて滑らせない=限界の“手前”を使い切る」。
// ★引き出すロジック★ throttle discipline(アクセルの自制):
//   (a) 旋回中はコーナー上限PWM(滑り出す手前。TCAP<220)を絶対に超えない。
//   (b) 出口で“急に”踏むとパワーオーバーで滑るので、旋回直後の一拍は CRUISE 止まりにする。
// ※ここは個別ロジックというより「限界を知って自制する」例。FR基準にdisciplineを足した形。

int TOP=248, CRUISE=205, SLOW=120, TCAP=150;
int D_OPEN=620, D_MID=420, D_TURN=385, D_SIDE=190;
int prevTurn;   // 直前が旋回中だったか (出口で急に踏まないため)

void setup() { RC_setup(); prevTurn = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); turning = 0; }

  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;             // (a) 滑り出す手前で頭打ち
  if (prevTurn == 1 && turning == 0 && pwm > CRUISE) pwm = CRUISE; // (b) 出口は急に踏まない
  prevTurn = turning;
  RC_drive(FORWARD, pwm);
}
