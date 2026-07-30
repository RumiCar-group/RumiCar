// Brake Lab — ABS ブレーキング  [競技 / フルスケール]  by Fable 5
// ★領域=フルスケール・レース / コース=競技グラウンド / 「車輪エンコーダ(任意)」を ON★
//
// 全力でブレーキを踏むと駆動輪がロックする (スリップ率 -1 = 路面に対し車輪が止まる)。
// ロックしたタイヤは「縦も横もグリップを失う」ので、①制動力がむしろ落ちて止まれない、
// ②横力が消えて舵が効かない (=曲がれない/スピン)。
// ABS = 車輪速でロックを検知し、ロックしたら一瞬ブレーキを緩めて車輪を転がし直す。
//   FR は前輪(非駆動)が接地速度の基準。後輪速が前輪速より大きく落ちたら「ロック」。
//
// A/B 比較デモ: ①ベタ踏みロック → ②ABS、各々の停止距離を Serial に出す。
//   実測: ロック ≈50m に対し ABS ≈32m (約 1/3 短い)。ロックは止まれないのが分かる。
// 信頼区間: ABS は車輪エンコーダでロックを検知して走り、前方 ToF 測距は読まない=信頼区間パラメータは無い。
int phase = 0;
float odo = 0, lockDist = 0;

void setup() { RC_setup(); }

void loop() {
  float gs = RC_wheel_speed(FRONT);
  float rear = RC_wheel_speed(REAR);
  RC_steer(CENTER);

  if (phase == 0) {                   // 助走 (TC で 18m/s まで)
    int pwm = (gs + 5) / 110 * 255;
    RC_drive(FORWARD, pwm);
    if (gs > 18) { phase = 1; odo = 0; }
    return;
  }
  if (phase == 1) {                   // ① ベタ踏みロック (止まりきれない)
    RC_drive(BRAKE, 255);
    odo = odo + gs / 20;
    if (gs < 0.5) { lockDist = odo; Serial.println(lockDist); RC_setup(); phase = 2; }
    return;
  }
  if (phase == 2) {                   // 助走 (再び)
    int pwm = (gs + 5) / 110 * 255;
    RC_drive(FORWARD, pwm);
    if (gs > 18) { phase = 3; odo = 0; }
    return;
  }
  if (rear < gs * 0.8) RC_drive(FREE, 0);   // ② ABS: ロック検知で緩める
  else                 RC_drive(BRAKE, 255);
  odo = odo + gs / 20;
  if (gs < 0.5) { Serial.println(odo); RC_setup(); phase = 0; }
}
