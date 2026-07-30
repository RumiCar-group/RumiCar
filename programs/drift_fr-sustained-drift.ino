// Sustained Drift — フルスケール・サーキットで持続ドリフト  [競技 / フルスケール]  by Opus 4.8
// ★領域=フルスケール・レース / コース=競技サーキット (フルスケール) / 車=ドリフトFR★
//
// J1 で判明: ドリフト車はフルスケール・サーキットで即スピン(<11秒)し「持続ドリフト」が存在しなかった。
// Phase J2 でドリフト車専用の縦μ(muXDrift)を下げて空転パワーオーバーを復活させ、保持できるスライドが
// 生まれた。このプログラムはそれを ToF×3 だけで保持する(J-1: 姿勢=ヨー/βは直接読まない。壁の距離
// L/C/R の差と前方の詰まり方だけで操る。逆ハン=カウンターは姿勢推定を要するが J1 でそれは不成立なので
// 当てない=スロットルだけでスライドを維持する ZeroCounter の作法)。
//
// 円角四角(直線×4+コーナー×4・全コーナーが同じ向き)を:
//   直線  : グリップで踏んで前進(平行移動)。前が速く詰まれば(高速でコーナー接近)先読みブレーキ。
//   コーナー: 広い側へ全舵を当てたまま、スロットルを「フレア210 / グリップ円150」で脈打たせて後輪を
//            流し続ける(空転代=車速<車輪速上限 を保ちスライドを維持)。
// 注: 横滑りは前方センサーに映らないので壁ガードは早め(GUARD)に拾い、滑走中に前進で逃げない(悪化する)。
//
// 条件を変えて確かめよう(J4): この走りが成り立つのは「壁で囲まれたフルスケール・サーキット」。
//   プログラムはそのままで、領域やコースを変えて走らせると結果はどう変わる? 先回りの答えは書かない。
//   どんな条件なら成り立ち、どんな条件だと成り立たないかは、走った結果から自分で気づこう。
// ★Stage AO 注記(走行エンジン v2)★ v2(4輪 two-track・muXDrift ハック廃止・忠実な荷重移動)では、この実寸
//   フルスケール・コーナーは速度が高すぎて、最初のコーナーで即スピン→スタックする(前進は最初の直線ぶんのみ)。
//   ToF×3+3値ステアでは姿勢(ヨー/β)を読めず逆ハンを当てられない=持続ドリフト不成立、という J1 の限界は
//   v2 でも不変で、むしろ荷重移動が忠実なぶん一層はっきり出る(=最速レースにはドリフトは不利)。この
//   「ドリフトの意味/利点」を安全に学ぶ環境は 卓上スリップタイヤ(Stage AO6・低速で滑りを保てる)を参照。
int TOP, CORNER, BIAS, GUARD, GUARDS, DUTY, DUTYHI, FLARE, GRIP, CLOSE;
int CONF=150000;     // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。本フルスケールは実車レーダ相当の
// 150m レンジで設計しているので信頼区間もレーダ地平(150m)とする。実短距離 ToF の地面拘束は ~5m
// (=250mm×領域スケール20)ゆえ、実機/自機ではここを下げて要調整(下げるほど遠くの壁を見ず反応が遅れる)。
int prevC; int started; int t;

void setup() {
  RC_setup();
  TOP = 170; CORNER = 50000; BIAS = 6000; GUARD = 9000; GUARDS = 4500;
  DUTY = 24; DUTYHI = 9; FLARE = 210; GRIP = 150; CLOSE = 2200;
  prevC = 0; started = 0; t = 0;
}

void loop() {
  int L = RC_read(LEFT);
  int C = RC_read(CENTER);
  int R = RC_read(RIGHT);
  if (L < 0 || L > CONF) L = 99000;   // 範囲外(-3)=遠く開けている → 大きな値へ正規化
  if (C < 0 || C > CONF) C = 99000;
  if (R < 0 || R > CONF) R = 99000;
  int dC = 0;
  if (started == 1) dC = prevC - C;   // dC>0 = 前方が縮む = コーナー/壁へ接近
  prevC = C; started = 1;
  t = t + 1;

  // 壁ガード: 近すぎたら広い側へ舵を残して BRAKE (滑り状態の万能リセット)。前進で逃げない。
  if (C < GUARD || L < GUARDS || R < GUARDS) {
    if (L > R) RC_steer(LEFT); else RC_steer(RIGHT);
    RC_drive(BRAKE, 240);
    return;
  }

  // コーナー: 前方が詰まっている = 進入/旋回中。広い側(=コーナー内側・全周同じ向き)へ全舵を保持し
  //   スロットルを脈打たせて後輪を流し続ける。
  if (C < CORNER) {
    if (L >= R) RC_steer(LEFT); else RC_steer(RIGHT);
    if ((t % DUTY) < DUTYHI) RC_drive(FORWARD, FLARE);
    else RC_drive(FORWARD, GRIP);
    return;
  }

  // 直線: 近い壁から離れてセンタリングしつつグリップで前進。前が速く詰まれば先読みブレーキ。
  if (L - R > BIAS) RC_steer(LEFT);
  else if (R - L > BIAS) RC_steer(RIGHT);
  else RC_steer(CENTER);
  if (dC > CLOSE) RC_drive(BRAKE, 210);
  else RC_drive(FORWARD, TOP);
}
