// Apex Hunter — ノーマル FR 用  [Fable Racing Line / Lv1 基準]  by Fable 5
// 実測 (v6.0.0・完走が定義される全41コース中39コース・既定3台・laps=3・rejoin=OFF): 37/39 完走。
// 素直なFRはこの単純ロジックで6種中トップ級。
// ★学習の出発点★ FRは駆動(後輪)と操舵(前輪)が分かれるので最も素直に曲がる(us=0.10)。
// だから小細工なしの「素直なギャップフォロワー」がそのまま良く走る。まずこれで基本を掴もう。
//
// 全車に共通する土台ロジック(4つ):
//   (1) 前方センサー C の空き具合で速度を3段に変える(直線=速い / 中速 / コーナー=遅い)
//   (2) 前が詰まったら左右で「広い方」へ全力で曲げる
//   (3) 側方の壁が近ければ離れる方向へ補正する
//   (4) 前が塞がったまま開いてこなければ、いったん下がって切り返す(行き止まり脱出)
// 他の車種は「この土台に何を足すか/引くか」で特性を引き出す。FRは足す必要が少ない=基準。
// ※FRはアクセルONで少しリアが出る(powerOs=0.40)ので、旋回中だけ上限PWM(TCAP)で軽く抑える。

int TOP=250, CRUISE=215, SLOW=130, TCAP=195;       // 速度: 直線/中速/コーナー/旋回中の上限
int D_OPEN=620, D_MID=405, D_TURN=375, D_SIDE=180; // 判定距離[mm]
int CONF=640, OPEN=9999;   // 信頼区間[mm]: これを超える/範囲外(-3)の測距は「遠い/開放」とみなす。
// 実機 VL53L0X は地面拘束・低信号で ~250mm 超を信頼しにくい。本シム卓上コースは実機模型の約2.5倍
// 広い(廊下~550mm)ため実スケール換算 ~640mm(=250×2.56) を既定にする。★実機へ移すときは自機の
// 車体/搭載高に合わせて下げ(目安250mm)、併せて速度も落とすこと。fullscale は 250×領域スケール。

int ESC=120, STUCK=1, BACK=7, SETTLE=4;   // (4) [脱出] 前方が ESC[mm] 未満で「詰まり続け」が STUCK 回
int prevC; int seen; int stuckT; int escBack; int escRec; int escDir;  // 続いたら行き止まり → BACK 回後退 → SETTLE 回落ち着き
// 3値操舵は最小旋回半径が決まっているので、それより小さいコーナーは「下がって角度を作り直す」しか
// 通れない(実機でも同じ)。要点は2つ:
//   ★掠めただけでは発火させない★ 速いコースでは壁を数十mmでかすめるのが普通(実測: 全3台完走できる
//     コースでも前方最小 25mm まで詰まる)。距離だけで判定すると正常な走行を壊す。だから「近い」に
//     加えて「前方が開いてこない(dC<=0)」を必須にする — かすめて抜ける場面は前方が開くので発火しない。
//   ★後退中は舵を固定する★ 毎回 L/R を見比べて舵を選び直すと左右にぶれて同じ場所を往復し(極限周回)、
//     ヘアピンから出られなくなる。抜ける向きは入った瞬間に決めて、後退の間ずっと保つ。

void setup() { RC_setup(); prevC = 0; seen = 0; stuckT = 0; escBack = 0; escRec = 0; escDir = CENTER; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters(); if (L < 0 || L > CONF) L = OPEN;  // 信頼区間外/範囲外=開放
  int C = sensor1.readRangeSingleMillimeters(); if (C < 0 || C > CONF) C = OPEN;
  int R = sensor2.readRangeSingleMillimeters(); if (R < 0 || R > CONF) R = OPEN;

  // (4) ★行き止まりからの脱出★ 詰まり続けたら、開いている側へ鼻先が向くよう“逆ハンで後退”する。
  int dC = 0; if (seen == 1) dC = C - prevC; prevC = C; seen = 1;   // dC<=0 = 前方が開いてこない
  if (C < ESC && dC <= 0) stuckT = stuckT + 1;   // 「近い」かつ「開かない」が続いた回数
  if (C >= ESC) stuckT = 0;                      // 前が開いたら解除
  if (escBack > 0) { escBack = escBack - 1; RC_steer(escDir); RC_drive(REVERSE, 170); return; }   // 舵を固定して後退
  if (escRec > 0) { escRec = escRec - 1; RC_steer(CENTER); RC_drive(FORWARD, 90); return; }       // 直後は落ち着かせる
  if (stuckT >= STUCK) {
    if (L > R) escDir = RIGHT; else escDir = LEFT;   // 開いている側へ鼻先が向く舵を“入口で決めて”固定
    escBack = BACK; escRec = SETTLE; stuckT = 0;
    RC_steer(escDir); RC_drive(REVERSE, 170); return;
  }

  // (2)(3) 操舵: 前が詰まれば広い方へ、側方が近ければ「近い方の壁から」離れる
  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (L < D_SIDE || R < D_SIDE) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else { RC_steer(CENTER); turning = 0; }

  // (1) 速度: 前方が開けているほど速く
  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;  // 旋回中は軽く抑え巻き込みを防ぐ
  RC_drive(FORWARD, pwm);
}
