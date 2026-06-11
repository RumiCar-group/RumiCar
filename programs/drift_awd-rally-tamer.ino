// Rally Tamer — ドリフト 4WD 用  [Fable Racing Line / Lv5 限界を管理+]  by Fable 5
// 試走(全30ドライ周回): 24/30 完走・平均 2.12 km/h。穏やかな滑りで速さと安定のバランス良好。
// ドリフト4WDも PWM を上げすぎると四輪ドリフトに入るが、四駆ゆえグリップ回復が速く(release=6.0)
// 挙動が穏やか。鋭い巻き込みは少ない=一番“御しやすい滑り”。
// ★引き出すロジック★ Grip Reaper と同じ「限界の手前を使う」自制。ただし安定して回復が速いので、
//   旋回中の上限PWM(TCAP)を FRドリフトより高めに取れる=ドリフト車の中では速い。
//   ここは「ロジックは同じで定数だけ攻める」例(車種ごとに“どこまで攻めるか”が違う、の学び)。

int TOP=255, CRUISE=212, SLOW=144, TCAP=197;
int D_OPEN=640, D_MID=410, D_TURN=384, D_SIDE=178;
int prevTurn;

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
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  if (prevTurn == 1 && turning == 0 && pwm > CRUISE) pwm = CRUISE;
  prevTurn = turning;
  RC_drive(FORWARD, pwm);
}
