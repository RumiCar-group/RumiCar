// Apex Hunter — ノーマル FR 用  [Fable Racing Line / Lv1 基準]  by Fable 5
// 試走(全30ドライ周回): 27/30 完走・平均 2.08 km/h。素直なFRはこの単純ロジックで6種中トップ級。
// ★学習の出発点★ FRは駆動(後輪)と操舵(前輪)が分かれるので最も素直に曲がる(us=0.10)。
// だから小細工なしの「素直なギャップフォロワー」がそのまま良く走る。まずこれで基本を掴もう。
//
// 全車に共通する土台ロジック(3つ):
//   (1) 前方センサー C の空き具合で速度を3段に変える(直線=速い / 中速 / コーナー=遅い)
//   (2) 前が詰まったら左右で「広い方」へ全力で曲げる
//   (3) 側方の壁が近ければ離れる方向へ補正する
// 他の車種は「この土台に何を足すか/引くか」で特性を引き出す。FRは足す必要が少ない=基準。
// ※FRはアクセルONで少しリアが出る(powerOs=0.40)ので、旋回中だけ上限PWM(TCAP)で軽く抑える。

int TOP=250, CRUISE=215, SLOW=130, TCAP=195;       // 速度: 直線/中速/コーナー/旋回中の上限
int D_OPEN=620, D_MID=405, D_TURN=375, D_SIDE=180; // 判定距離[mm]

void setup() { RC_setup(); }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

  // (2)(3) 操舵: 前が詰まれば広い方へ、側方が近ければ離れる
  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); turning = 0; }

  // (1) 速度: 前方が開けているほど速く
  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;  // 旋回中は軽く抑え巻き込みを防ぐ
  RC_drive(FORWARD, pwm);
}
