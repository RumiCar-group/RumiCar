// Range-Flow Estimator — 前方ToF×3(+任意の後方/エンコーダ)の時間変化から姿勢(横滑り角β)を推定する研究サンプル  [競技 / フルスケール]  by Opus 4.8
// ★領域=フルスケール・レース / コース=競技サーキット (フルスケール) / 車=ノーマルFF★
//
// ねらい(研究): 「姿勢(ヨー/横滑り角β)を直接読むセンサーは足さない」(J-1)。代わりに、各ToFが
//   1ループで何mm縮んだか/伸びたか(レンジレート)から、車体の速度(前後vx・横vy)と回頭ω を逆算し、
//   β=atan2(vy,vx) を推定してみる。原理=レンジフロー(測距オドメトリ):
//     各ビーム i は機体に固定で 向きn_i・取付p_i が既知。壁を「ビームに正対する平面」と素朴に仮定すると、
//     距離変化 Δd_i ≈ -(n_i·vx + n_i·vy + ω·k_i) の線形式になる(k_i=(n_i×p_i)_z)。ビーム3本(+後方/
//     エンコーダ)→ 連立を最小二乗(正規方程式+Cramer)で解く。±65°の広い左右ビームが横滑りvyを左右差に
//     出すので観測しやすい…はず、というのが研究テーマ。
//
// ★正直な限界(Phase J1 で本番実測=NO-GO。CI-7: 緩めない)★
//   ToF からの姿勢推定は本シムでは本番基準(平均誤差MAE≤15° かつ 符号一致≥80%)を満たさない=不成立。
//   J1(精密版)でも MAE 10〜12° / 符号一致 70〜72%(<80%)で、「βは常に0」と決め打ちする自明推定に勝てなかった。
//   この素朴版を走らせると更にはっきり分かる:
//     ・エンコーダON(推奨)→ 推定βは真βと同じくらい小さく出る(MAE 数°)が、それは安定周回の真βが
//       元々数°と小さいからで「β=0と決め打ち」と同程度。肝心の【どちらに滑っているか=符号】が当たらない。
//     ・エンコーダOFF → 前後速度の符号すら定まらず、推定が大きく外れる(だから下の事前情報/エンコーダが要る)。
//   理由(構造的・直せない): ①壁の法線は1スキャンからは分からない(法線↔自己運動の循環)。素朴な
//     「壁はビームに正対」仮定が、斜め壁では横滑りの符号を取り違える。②フルスケールは壁が遠く、回頭
//     ω×(数十m)が vx/vy(~1)を圧倒する悪条件。③安定周回の真βは数°と小さく、符号≥80%は"ほぼ完璧"を要求。
//   → だから"成り立たない研究課題"として提供する。素朴な壁法線仮定をどう直すか——それが君の研究テーマ。
//
// 使い方: 走らせて、ここが出力する「推定β」と、画面 DEPTH パネルの「β …°」(真値)を並べて見比べよう。
//   後方センサー(任意)・車輪エンコーダ(任意)を ON にすると観測式が1〜2本増える(が、それでも基準には届かない)。
//   推定をどう直せば真βに近づくか——それがこの研究環境の問いです。
//   (走りは Circuit Racer と同じ素直なFF周回。壁が視野に入るので推定の素材になる。)
double PL, PC, PR, PB; int prevC; int started; int t; int sc;
int CONF=150000;     // 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」。本フルスケールは実車レーダ相当の
// 150m レンジ設計ゆえ信頼区間もレーダ地平(150m)。実短距離 ToF の地面拘束は ~5m(=250mm×領域スケール20)。
// 実機/自機では下げて要調整。※姿勢推定部は別途 L<140000 で有効域を絞る(遠方壁ほどレンジフローが弱い)。

double k_of(double nx, double ny, double px, double py) { return ny*px - nx*py; }   // ω係数 (n×p)_z

double det3(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
  return a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);   // 3x3 行列式 (Cramer 法用)
}

void setup() { RC_setup(); started = 0; t = 0; prevC = 0; PL = 0; PC = 0; PR = 0; PB = 0; sc = 0; }

void loop() {
  int L = RC_read(LEFT);
  int C = RC_read(CENTER);
  int R = RC_read(RIGHT);
  // ---- 周回ドライバ (Circuit Racer と同じ。FFは安定周回・壁が視野に入る) ----
  int Lg = L; int Cg = C; int Rg = R;
  if (Lg < 0 || Lg > CONF) Lg = 99000;   // 範囲外(-3)/信頼区間外=遠く開けている
  if (Cg < 0 || Cg > CONF) Cg = 99000;
  if (Rg < 0 || Rg > CONF) Rg = 99000;
  int dCb = 0;
  if (started == 1) dCb = prevC - Cg;
  prevC = Cg;
  // 判定距離は視野コーン(25°)対応で直線レイ時代の約半分(Circuit Racer と同じ再チューン・#27/Stage AM)。
  int turning = 1; int dir = CENTER;
  if (Cg < 16000) { if (Lg > Rg) dir = LEFT; else dir = RIGHT; }
  else if (Lg - Rg > 2000) dir = LEFT;
  else if (Rg - Lg > 2000) dir = RIGHT;
  else { dir = CENTER; turning = 0; }
  // ★v2(精密動力学)対応=全舵デューティ変調★ 2ループに1回だけ実際に舵を当て残りは CENTER に戻す
  //   =実効舵角を半分に薄める(大R コーナーで全舵は切りすぎ→後輪破りスピン。Circuit Racer と同じ v2 対策)。
  int phase = sc % 2; sc = sc + 1;
  if (turning == 1 && phase < 1) RC_steer(dir); else RC_steer(CENTER);
  int pwm = 82;
  if (Cg > 33000) pwm = 140;
  else if (Cg > 22000) pwm = 110;
  if (turning == 1 && pwm > 78) pwm = 78;   // 旋回中の上限は v2 で 110→78
  if (dCb > 1600 && Cg < 33000) RC_drive(BRAKE, 240);
  else RC_drive(FORWARD, pwm);

  // ---- 姿勢推定(研究): レンジフロー ----  距離は m に直す。範囲外/未装備は無効。
  int B = RC_read(BACK);                 // 後方(任意装備)。未装備は -2
  double enc = RC_wheel_speed(REAR);     // 車輪面速度[m/s](任意装備)。未装備は -1
  double dL = L / 1000.0; double dC = C / 1000.0; double dR = R / 1000.0; double dB = B / 1000.0;
  t = t + 1;

  // 各ビーム: 行[nx, ny, k]·(vx,vy,ω) = -Δd_i (素朴な「壁はビームに正対」仮定)。正規方程式 ATA·x = ATb を組む。
  double a00=0.0; double a01=0.0; double a02=0.0; double a11=0.0; double a12=0.0; double a22=0.0;
  double b0=0.0; double b1=0.0; double b2=0.0; int rows=0;
  // L (取付 p=[0.130,0.018], n=[cos65,sin65])
  if (L > 0 && L < 140000 && started == 1 && PL > 0) {
    double nx=0.4226; double ny=0.9063; double k=k_of(nx,ny,0.130,0.018); double rhs=-(dL - PL);
    a00=a00+nx*nx; a01=a01+nx*ny; a02=a02+nx*k; a11=a11+ny*ny; a12=a12+ny*k; a22=a22+k*k;
    b0=b0+nx*rhs; b1=b1+ny*rhs; b2=b2+k*rhs; rows=rows+1;
  }
  // C (p=[0.135,0], n=[1,0])
  if (C > 0 && C < 140000 && started == 1 && PC > 0) {
    double nx=1.0; double ny=0.0; double k=k_of(nx,ny,0.135,0.0); double rhs=-(dC - PC);
    a00=a00+nx*nx; a01=a01+nx*ny; a02=a02+nx*k; a11=a11+ny*ny; a12=a12+ny*k; a22=a22+k*k;
    b0=b0+nx*rhs; b1=b1+ny*rhs; b2=b2+k*rhs; rows=rows+1;
  }
  // R (p=[0.130,-0.018], n=[cos65,-sin65])
  if (R > 0 && R < 140000 && started == 1 && PR > 0) {
    double nx=0.4226; double ny=-0.9063; double k=k_of(nx,ny,0.130,-0.018); double rhs=-(dR - PR);
    a00=a00+nx*nx; a01=a01+nx*ny; a02=a02+nx*k; a11=a11+ny*ny; a12=a12+ny*k; a22=a22+k*k;
    b0=b0+nx*rhs; b1=b1+ny*rhs; b2=b2+k*rhs; rows=rows+1;
  }
  // B (後方・任意: p=[-0.045,0], n=[-1,0])
  if (B > 0 && B < 140000 && started == 1 && PB > 0) {
    double nx=-1.0; double ny=0.0; double k=k_of(nx,ny,-0.045,0.0); double rhs=-(dB - PB);
    a00=a00+nx*nx; a01=a01+nx*ny; a02=a02+nx*k; a11=a11+ny*ny; a12=a12+ny*k; a22=a22+k*k;
    b0=b0+nx*rhs; b1=b1+ny*rhs; b2=b2+k*rhs; rows=rows+1;
  }
  // エンコーダ(任意): vx を直接拘束 (1·vx = enc·Δt, Δt≈0.05s)。重み50。
  if (enc >= 0) { double w=50.0; double rhs=enc*0.05; a00=a00+w; b0=b0+w*rhs; rows=rows+1; }
  // 前進は既知(FORWARD指令)なので vx>0 の弱い事前情報を入れる。これが無いと前後速度の符号が
  //   定まらず推定が±180°へ飛ぶ。エンコーダがあればそちらが正確に vx を与える(この事前は弱い)。
  double pw=3.0; a00=a00+pw; b0=b0+pw*0.6; rows=rows+1;

  // 前回値を保存
  if (L > 0 && L < 140000) PL = dL; else PL = 0;
  if (C > 0 && C < 140000) PC = dC; else PC = 0;
  if (R > 0 && R < 140000) PR = dR; else PR = 0;
  if (B > 0 && B < 140000) PB = dB; else PB = 0;
  started = 1;

  if (rows < 3) return;
  // 正則化して 3x3 を Cramer で解く
  a00=a00+0.0001; a11=a11+0.0001; a22=a22+0.000001;
  double det = det3(a00,a01,a02, a01,a11,a12, a02,a12,a22);
  if (det < 0.000000001 && det > -0.000000001) return;
  double vx = det3(b0,a01,a02, b1,a11,a12, b2,a12,a22) / det;
  double vy = det3(a00,b0,a02, a01,b1,a12, a02,b2,a22) / det;
  double beta = atan2(vy, vx) * 57.29578;

  // 約2回/秒で出力。画面 DEPTH の「β …°」(真値)と見比べよう。
  if ((t % 10) == 0) Serial.println("推定β=" + round(beta) + "° / 画面DEPTHの真βと比べよう (J1=NO-GO: 届かない)");
}
