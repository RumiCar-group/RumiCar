// Drift Wall — ドリフトの“車幅”で塞ぐブロッカー  [戦術 / racecraft]  by Fable 5
//
// ★目的★ 速さではなく「相手に抜かせない」。ドリフトでテールを左右に流し、車体の横幅を使って
//   走行ラインを占有する。単純な低速ブロックでは“隙間”ができて抜かれるコースで本領を発揮する。
//
// ★なぜドリフトか / 実測(賢い追い越し車を45秒で抑えた割合。低いほどブロック成功)★
//   スピードウェイ : 単純ブロック 99%(抜かれる) → Drift Wall 4%(完封)
//   ボトルネック   : 単純ブロック 99% → Drift Wall 2%
//   ※ オーバルのように大きく開けたコースでは振り過ぎてラインを空けることがある(95%=失敗)。
//   → 単純ブロックと Drift Wall は“相補的”。コースで使い分けるのが上級者の racecraft。
//   ※ 純粋なラップ速度ではグリップ走行が最速(ドリフトは速さでは勝てない)。Drift Wall は
//     「1位を獲るための妨害」に特化した使い方。
//
// 使い方: 複数台モードで「他車を障害物にする」ON。自分が前に出た状況で使う。ドリフトFR/4WD推奨。
int D_TURN = 380, D_SIDE = 175, BASE = 120, FLICK = 210;
int t;

void setup() { RC_setup(); t = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();
  t = t + 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); RC_drive(FORWARD, 150); return; } // コーナーは占有
  if (R < D_SIDE) { RC_steer(LEFT);  RC_drive(FORWARD, BASE); return; }
  if (L < D_SIDE) { RC_steer(RIGHT); RC_drive(FORWARD, BASE); return; }
  // 直線: 左右に振ってテールを流し車幅で塞ぐ。進みすぎてラインを空けないよう低PWMを挟む。
  int ph = t % 24;
  if (ph < 6)       { RC_steer(LEFT);  RC_drive(FORWARD, FLICK); }  // 左へ振り(誘発)
  else if (ph < 12) { RC_steer(RIGHT); RC_drive(FORWARD, BASE);  }  // 逆ハンで戻し(低速)
  else if (ph < 18) { RC_steer(RIGHT); RC_drive(FORWARD, FLICK); }  // 右へ振り
  else              { RC_steer(LEFT);  RC_drive(FORWARD, BASE);  }  // 逆ハンで戻し
}
