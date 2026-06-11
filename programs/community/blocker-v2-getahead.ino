// Blocker v2 — 前に出てから守る(2段階)  [戦術プログラム]  by Fable 5
//
// ★狙い★ 「塞ぐには、まず何としても相手の前に出る必要がある」というレースの本質を形にした。
//   後方センサーが無いので、前方の様子だけで2つのフェーズを切り替える:
//     ① 追越し: 前に車(前方が中途半端に詰まる×横が開く)→ 開いた側へ全開で抜く(リスクを取る)
//     ② ブロック: 前方が一定時間 空き続けた=前に出た → 低速占有 + コーナーはイン側(アペックス)を守る
//
// ★なぜ/学び(2台レースの実測)★
//   ・ペース優位 × 狭いコース … 前に出られ、相手を基準比 30% まで封殺(4.1m vs 自由13.7m)。最強。
//   ・同等ペース × 狭いコース … 前に出られず、無理な追越しでクラッシュ(リスクが顕在化)。
//   ・広いコース … 遅い相手でも前に出るのが難しく、ブロックが成立しにくい。
//   → 結論: 「前に出る」のが前提で、それ自体がペース or 位置の優位を要する難所。狭い道ほど
//     一度出れば守りやすいが、出る過程は最も危険。track position(位置取り)が王様。
//
// 使い方: 複数台モードで「他車を障害物にする」ON。相手の後ろからでも仕掛けられる。
int TOP=252, SLOW=120, BLOCK=148;
int D_OPEN=620, D_MID=405, D_TURN=378, D_SIDE=180;
int CARNEAR=170, CARFAR=470, GAP=560, GETAHEAD=24;  // 前方が GETAHEAD ティック空けば「前に出た」
int clearT; int ahead;

void setup() { RC_setup(); clearT = 0; ahead = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();
  int carAhead = (C > CARNEAR && C < CARFAR && (L > GAP || R > GAP)) ? 1 : 0;  // 前車の気配
  if (carAhead == 1) clearT = 0; else clearT = clearT + 1;
  if (clearT >= GETAHEAD) ahead = 1;   // 一定時間 前が空いた = 前に出た
  if (carAhead == 1) ahead = 0;        // また前に車 = まだ後ろ

  if (ahead == 0) {
    // ① 追越しフェーズ: リスクを取って前に出る
    if (carAhead == 1) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); RC_drive(FORWARD, TOP); return; }
    int t = 1;
    if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
    else if (R < D_SIDE) RC_steer(LEFT); else if (L < D_SIDE) RC_steer(RIGHT); else { RC_steer(CENTER); t = 0; }
    int p = SLOW; if (C > D_OPEN) p = TOP; else if (C > D_MID) p = 200; RC_drive(FORWARD, p);
  } else {
    // ② ブロックフェーズ: 低速占有 + コーナーはイン側(アペックス)を守って相手を外へ追いやる
    if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
    else if (R < D_SIDE) RC_steer(LEFT); else if (L < D_SIDE) RC_steer(RIGHT); else RC_steer(CENTER);
    RC_drive(FORWARD, BLOCK);
  }
}
