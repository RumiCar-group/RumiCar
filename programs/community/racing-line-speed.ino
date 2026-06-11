// Racing Line (speed profile) — 速いライン(速度プロファイル版)  [研究/教材]  by Fable 5
//
// ★テーマ★ アウト-イン-アウトの「速いライン」を RumiCar で再現できるか。
//
// ★正直な実測結果(FR, 全30ドライ周回での比較)★
//   ・幾何的なアウト-イン-アウト(進入で外へ付け、アペックスで内、脱出で外へ開く)は
//     3値操舵では「外側=壁へ commit」してしまい、全10コースで即クラッシュ(0/10完走)。
//     → 反応制御の3値操舵では、地図が無く“ラインを置く”ことができない。これが robot 走行の壁。
//   ・そこでラインの本質=「速度プロファイル(早めに減速→アペックス最低速→早めに立ち上がる)」だけを
//     安全な壁追従の操舵に載せたのが本プログラム。結果は基準(壁追従)の 98.8%(ほぼ互角)。
//     スピードウェイ/複合コーナー/うねりと円のような“速い流れるコース”では基準を上回ったが、
//     タイトなコースではむしろ僅かに遅い。
//   → 結論: 反応制御では壁追従が既にほぼ最適。ラインの旨味(早い立ち上がり)は速い流れの所だけ。
//     これは Quattro Blitz(4WDの立ち上がり全開)と同じ発想で、トラクションのある車ほど効く。
//
// ★学び★ 理屈で速いライン ≠ ロボットで速いライン。装備(地図/連続操舵)が変われば話が変わる。
int TOP=255, CRUISE=216, SLOW=132, TCAP=176;
int D_OPEN=620, D_MID=430, D_TURN=380, D_SIDE=180;
int prevC; int started;

void setup() { RC_setup(); prevC = 0; started = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();
  int dC = 0; if (started == 1) dC = C - prevC; prevC = C; started = 1;
  int t = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }   // 操舵は安全な壁追従
  else if (R < D_SIDE) RC_steer(LEFT); else if (L < D_SIDE) RC_steer(RIGHT); else { RC_steer(CENTER); t = 0; }
  int p = SLOW; if (C > D_OPEN) p = TOP; else if (C > D_MID) p = CRUISE;
  if (t == 1 && p > TCAP) p = TCAP;          // アペックスは速度を抑える
  if (dC > 22 && C > D_MID) p = TOP;          // ★脱出で早めに全開(立ち上がり優先=ラインの旨味)
  RC_drive(FORWARD, p);
}
