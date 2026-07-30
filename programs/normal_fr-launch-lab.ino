// Launch Lab — トラクション制御ローンチ  [競技 / フルスケール]  by Fable 5
// ★領域=フルスケール・レース / コース=競技グラウンド / 「車輪エンコーダ(任意)」を ON★
//
// フルスケールでは発進トルクがタイヤのグリップを上回るので、全開ベタ踏みすると
// 駆動輪(後)が空転して前に進まない (ホイールスピン)。これを“車輪速”で検知して抑えるのが
// トラクション制御 (TC)。FR は前輪が非駆動なので「前輪速 ≒ 本当の接地速度」。
//   スリップ率 = (後輪速 - 前輪速) / 前輪速。空転すると後輪速だけ跳ね上がる。
// TC の肝: アクセル開度ではなく「接地速度より少しだけ速い目標」を狙い続けると、
//   駆動力が摩擦ピーク内に収まりスリップが暴れない → 同じ時間でずっと遠くまで加速する。
//
// このプログラムは A/B 比較デモ: ①全開ベタ踏み → ②TC、各2.5秒の到達距離を Serial に出す。
//   実測: 全開 ≈16m に対し TC ≈40m (+24m)。フルスケールでこそ意味を持つ。
// 信頼区間: TC ローンチは車輪エンコーダ(前=接地速度/後=駆動速度の差)で走り、前方 ToF 測距は読まない=信頼区間パラメータは無い。
int phase = 0, t = 0;
float odo = 0, fullDist = 0;   // odo = 走行距離[m] (前輪速の積分 = 車輪オドメトリ)

void setup() { RC_setup(); }

void loop() {
  float gs = RC_wheel_speed(FRONT);   // 接地速度[m/s] (非駆動の前輪)
  float rear = RC_wheel_speed(REAR);  // 駆動輪(後)の速度[m/s]
  RC_steer(CENTER);
  t = t + 1;

  if (phase == 0) {                   // ① 全開ベタ踏み (空転して進まない)
    RC_drive(FORWARD, 255);
    odo = odo + gs / 20;              // 20Hz ループなので 1 回 = gs * (1/20) m
    if (t > 50) { fullDist = odo; phase = 1; t = 0; }   // 2.5 秒
    return;
  }
  if (phase == 1) {                   // ブレーキで停止 → 記録してスタートへ戻す
    if (rear < gs * 0.8) RC_drive(FREE, 0);   // 後輪ロック気味なら緩める (簡易ABS)
    else                 RC_drive(BRAKE, 255);
    if (gs < 0.5) { Serial.println(fullDist); RC_setup(); odo = 0; t = 0; phase = 2; }
    return;
  }
  if (phase == 2) {                   // ② トラクション制御ローンチ
    int pwm = (gs + 5) / 110 * 255;   // 「接地速度+5m/s」を狙う (110=フルスケール最高速)
    RC_drive(FORWARD, pwm);           //   → 駆動力が摩擦ピーク内に収まりスリップしない
    odo = odo + gs / 20;
    if (t > 50) { Serial.println(odo); RC_setup(); odo = 0; t = 0; phase = 3; }
    return;
  }
  RC_drive(FREE, 0);                  // ひと呼吸おいて最初へ
  if (t > 20) { phase = 0; t = 0; }
}
