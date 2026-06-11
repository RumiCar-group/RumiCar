// Drift Showtime — ドリフトショー(8の字ドーナツ)  [エンタメ / おまけ]  by Fable 5
//
// ★目的★ 速さではなく“魅せる”ためのデモ走行。開けた「ドリフト広場 (ショー会場)」コースで、
//   全ロック+高PWMの連続ドーナツを左右に切り替えて 8 の字を描く。
//   実測: 平均スリップ 0.58 で 70 秒以上継続(広場 4.4×3.4m 内に収まる)。
//
// 見どころ: 一人称ビュー下のタイヤスモークと、車体が大きく横を向く姿勢(前輪の逆ハン描画)。
//   軌跡に 8 の字が残ります。再生速度を上げると迫力が増します。
//
// 使い方: コースで「ドリフト広場 (ショー会場)」を選び、ドリフトFR / ドリフト4WD で実行。
int t;

void setup() { RC_setup(); t = 0; }

void loop() {
  t = t + 1;
  int ph = t % 180;   // 180tick=9秒で1サイクル(左ドーナツ→つなぎ→右ドーナツ→つなぎ)
  if (ph < 80)        { RC_steer(LEFT);   RC_drive(FORWARD, 230); }  // 左ドーナツ(連続ドリフト)
  else if (ph < 90)   { RC_steer(CENTER); RC_drive(FORWARD, 110); }  // つなぎ(向き替え)
  else if (ph < 170)  { RC_steer(RIGHT);  RC_drive(FORWARD, 230); }  // 右ドーナツ
  else                { RC_steer(CENTER); RC_drive(FORWARD, 110); }  // つなぎ
}
