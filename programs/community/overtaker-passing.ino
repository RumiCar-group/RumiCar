// Overtaker — 追い越し  [戦術プログラム: 前車を抜く]  by Fable 5
//
// ★狙い★ 前を塞ぐ車(Blocker)を抜く。壁のコーナーと違い、前車の“先”には横の逃げ場がある。
//   その幾何の違いを突いて回り込む。Blocker への対抗手であり、攻守を比べる教材。
//
// ★ロジック★ 前方が「車1台分くらい(170〜470mm)」に詰まり、かつ左右どちらかが大きく開いて
//   いれば(=壁のコーナーではなく前車だと判断)、その開いた側へ全開で回り込む。
//   それ以外は普通のグリップ追走(速いライン)。
//
// ★なぜ/学び(実測)★
//   ・狭いコースでは横が開かず抜けない → Blocker の勝ち(基準比3%)。
//   ・広いコースでは回り込めて抜ける → Overtaker の勝ち(基準比99%)。
//   つまり「追い越せるか」はコース幅と“前車の先の空き”で決まる。速さだけでなく相手の位置を
//   読む判断が要る。前車かどうかは、前方が中途半端に詰まる×横が大きく開く、で見分けている。
int TOP = 250, CRUISE = 215, SLOW = 130, TCAP = 195;
int D_OPEN = 620, D_MID = 405, D_TURN = 375, D_SIDE = 180;
int CARNEAR = 170, CARFAR = 470, GAP = 600;   // 前方CARNEAR..CARFAR=前車らしい / 横GAP以上開=抜ける

void setup() { RC_setup(); }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();
  // 追い越し: 前車らしく(中途半端に詰まる)横が大きく開くなら、その側へ全開で回り込む
  if (C > CARNEAR && C < CARFAR && (L > GAP || R > GAP)) {
    if (L > R) RC_steer(LEFT); else RC_steer(RIGHT);
    RC_drive(FORWARD, TOP);
    return;
  }
  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); turning = 0; }
  int pwm = SLOW; if (C > D_OPEN) pwm = TOP; else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  RC_drive(FORWARD, pwm);
}
