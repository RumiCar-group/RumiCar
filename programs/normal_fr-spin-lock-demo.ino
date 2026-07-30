// Spin & Lock Demo — 空転とロックを見る  [競技 / フルスケール]  by Fable 5
// ★領域=フルスケール・レース / コース=競技グラウンド (車輪エンコーダは不要)★
//
// 特別な装備なしで、フルスケールの2大現象をただ“見る”デモ:
//   ① 全開発進 → 駆動輪が空転 (タイヤスモークが出て、なかなか前に進まない)。
//   ② 舵を当てたままベタ踏みブレーキ → 駆動輪ロックで横力が消え、スピンする。
// 「なぜこうなるか/どう抑えるか」は Launch Lab / Brake Lab (車輪エンコーダ ON) で学ぶ。
// 信頼区間: このデモは固定シーケンス(時間制御)で空転/ロックを見せ、ToF 測距を読まない=信頼区間パラメータは無い。
int t = 0, phase = 0;

void setup() { RC_setup(); }

void loop() {
  t = t + 1;
  if (phase == 0) {                   // ① 全開ローンチ (空転)
    RC_steer(CENTER); RC_drive(FORWARD, 255);
    if (t > 60) { phase = 1; t = 0; }
    return;
  }
  if (phase == 1) {                   // ② 舵+ベタ踏みブレーキ (ロック→スピン)
    RC_steer(LEFT); RC_drive(BRAKE, 255);
    if (t > 40) { phase = 2; t = 0; }
    return;
  }
  RC_steer(CENTER); RC_drive(FREE, 0); // 停止して仕切り直し
  if (t > 40) { RC_setup(); phase = 0; t = 0; }
}
