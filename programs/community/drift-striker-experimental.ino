// Drift Striker (experimental) — 状況分岐ドリフト + 逆ハンキャッチ  [技法デモ by Fable 5]
//
// 【これは何か】Fable Racing Line の物理拡張 (連続ドリフト/逆ハン/センサー微分) を全部使い、
//   「ヘアピン級の急コーナーだけテールを流して一気に回し、前方が開いたら逆ハンでキャッチ」
//   という状況分岐を実装した“技法のデモ”です。
//
// 【正直な実測結果(シミュレータ)】
//   3値操舵・20Hz というRumiCarの制御の粗さでは、誘発ドリフトは横滑り(slipSlide)で
//   外へ膨らみ、純グリップ版(Grip Reaper / Rally Tamer)に「勝てません」。
//   全30コース完走: 純グリップ 25 vs 本プログラム 24(FR)/ 22(4WD)。平均速度もほぼ同じ。
//   → 結論: シミュレータ上では“滑らかなグリップ”が最速・最安定。ドリフトは見せ場向き。
//
// 【では何のために残すか】
//   ・連続グリップ余裕モデルで「滑り出し」を、逆ハンで「姿勢の安定化」を体験できる教材。
//   ・実車は操舵がより細かく(プロポーショナル/速いサーボ)、滑り出しも路面依存なので、
//     実機では本プログラムの方が速くなる余地があります。実車で試して育ててください。
//   ・改造のヒント: HAIRPIN(誘発する急さ)、誘発PWM、キャッチの閾値(dC)とPWMを調整。
//
// 状態機械: 通常=グリップ → (旋回中 & 前方<HAIRPIN)で誘発 → (前方が開く dC<-25)で逆ハン。

int TOP=248, CRUISE=205, SLOW=120, TCAP=150;
int D_OPEN=620, D_MID=420, D_TURN=385, D_SIDE=190;
int HAIRPIN=205;   // ヘアピン級と見なす前方距離[mm]
int pC; int inited; int prov; int caught; int dir;

void setup() {
  RC_setup();
  pC = 0; inited = 0; prov = 0; caught = 0; dir = 0;
}

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

  // センサー微分: 前方の変化 (正=詰まる, 負=開く)。コーナーの開きを検知する。
  int dC = 0;
  if (inited == 1) dC = pC - C;
  pC = C; inited = 1;

  // 通常(グリップ)の操舵・速度
  int steer = 1;     // 1=LEFT 2=CENTER 3=RIGHT
  int turning = 1;
  if (C < D_TURN) { if (L > R) steer = 1; else steer = 3; }
  else if (R < D_SIDE) steer = 1;
  else if (L < D_SIDE) steer = 3;
  else { steer = 2; turning = 0; }
  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP; else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;

  // ヘアピン状態機械: 誘発 → 逆ハンキャッチ
  if (prov == 0 && caught == 0) {
    if (turning == 1 && C < HAIRPIN) {        // 誘発開始
      if (L > R) dir = 1; else dir = 3;       // 滑り方向 = 曲がる方向
      prov = 5;
    }
  }
  if (prov > 0) {                              // 誘発中: 切り込み + 高PWMでテールを流す
    steer = dir; pwm = 240; prov = prov - 1;
    if (dC < -25) { prov = 0; caught = 4; }    // 前方が開き始めた=回り込んだ → 逆ハンへ
  } else if (caught > 0) {                      // 逆ハン中: 反対へ当てて姿勢を止める(低PWM)
    if (dir == 1) steer = 3; else steer = 1;
    pwm = 120; caught = caught - 1;
  }

  if (steer == 1) RC_steer(LEFT);
  else if (steer == 3) RC_steer(RIGHT);
  else RC_steer(CENTER);
  RC_drive(FORWARD, pwm);
}
