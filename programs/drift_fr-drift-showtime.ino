// Drift Showtime — ドリフト FR + 動力学モデル 用ショー演目  by Fable 5
// 「ドリフト広場 (ショー会場)」で踊る非レース・プログラム。実測: 600秒 無事故
// (dyn/standard 両物理)、dyn では全時間の53%が slip>0.4、β最大159° (バックエントリー成立)。
// 演目はループする:
//   [Act1 ドーナツ] 全舵+全開255 → 後輪空転で横力が抜け β≈52°・半径0.25m の安定回転
//   [Act2 8の字]   ドーナツを切り、直線バーストで移動して逆回りのドーナツへ (左右交互)
//   [Act3 バックエントリー] 助走全開 → フェイント → BRAKE+舵で振り出し (u<0.45 へ減速が
//        物理的に必須) → 255+舵で深化 β>90° (テールから進入) → BRAKE+CENTER で停止回収
// 操作列は test_dyn.mjs (donut / backentry シナリオ) の実証値を 20Hz tick に換算した
// オープンループ + 距離センサーの壁ガード。standard 物理でも走るが、β が 45° で
// 頭打ちになるため本領は動力学モデル (車両設定でトグル ON)。
// 信頼区間: この演目は開ループ(タイミング制御)で踊り、ToF は「壁に寄り過ぎたら止める」近距離ガード
// (~数百mm=信頼区間内)だけに使う=測距航法しない。ゆえに遠方読みへの CONF カットオフは設けない。
int at;        // 現在のフェーズ内の経過 tick (20Hz)
int act;       // 0=ドーナツ 1=8の字 2=バックエントリー
int ph;        // 演目内フェーズ
int laps;      // 8の字の回転数 (偶数=右回り、奇数=左回り)
int guardT;    // 壁ガードで退避中の残り tick

void setup() { RC_setup(); at = 0; act = 0; ph = 0; laps = 0; guardT = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

  // [壁ガード] 壁へ寄り過ぎたら「BRAKE で停止 (滑り状態の万能リセット) → 開いた側へ
  // グリップ離脱」。横滑りは前方センサーに映らないので早め (450mm) に拾い、
  // 滑走中に FORWARD で逃げようとしない (エネルギーを足して悪化する — 実測)。
  // バックエントリーの技の最中 (act2 ph>=2) は介入しない。
  if (act != 2 || ph < 2) {
    if (C < 600 || L < 260 || R < 260) { if (guardT == 0) guardT = 36; }
  }
  if (guardT > 0) {
    guardT = guardT - 1;
    if (guardT > 24) { RC_steer(CENTER); RC_drive(BRAKE, 255); return; }   // まず停める
    // 解除は全レイがクリアしてから (C だけで解除すると、横レイが壁を見たまま即再アーム
    // → BRAKE 連打のデッドロックで壁際に居座る — 実測)。
    // ドーナツ相の中断は「その周回は終了」扱いで進める (頭からやり直すと、壁際の周回が
    // 5 秒間ノーガードで完走することは稀で、ショーがその場で進まなくなる — 実測)
    if ((C > 900 && L > 300 && R > 300) || guardT == 0) {
      guardT = 0;
      if (act < 2 && ph == 2) at = 99; else at = 0;
    }
    else {
      if (L > R) RC_steer(LEFT); else RC_steer(RIGHT);
      if (C < 220) { RC_drive(REVERSE, 170); } else { RC_drive(FORWARD, 160); }
      return;
    }
  }

  at = at + 1;

  // ドーナツの作法 (動力学モデル): 255 を入れ続けると u が車輪速上限に達して空転代が消え、
  // 再グリップ→過回転スピン→フィッシュテールの暴れに入る (実測で漂流1.7m/40s)。
  // フレア (255×0.5s) を 170 のグリップ円で挟んでも漂流は溜まるため、約5秒ごとに
  // 「照準→中央へ復帰」を必ず挟む封じ込め構造にする (照準=回転しながら C が開くのを待つ)。
  if (act == 0 || act == 1) {           // [Act1] ドーナツ / [Act2] 8の字 (構造は同じ)
    int lt = 1;                         // この周回の回転方向 (Act1=左固定, Act2=交互)
    if (act == 1 && laps % 2 == 1) lt = 0;
    if (ph == 0) {                      // 照準: グリップ回転で前が大きく開くのを待つ
      if (lt == 1) RC_steer(LEFT); else RC_steer(RIGHT);
      RC_drive(FORWARD, 170);
      if (C > 1700 || at > 70) { ph = 1; at = 0; }
      return;
    }
    if (ph == 1) {                      // 復帰: 開けた方 (≈中央方向) へグリップで戻る
      RC_steer(CENTER); RC_drive(FORWARD, 190);
      if (at >= 14) { ph = 2; at = 0; }
      return;
    }
    if (lt == 1) RC_steer(LEFT); else RC_steer(RIGHT);          // ドーナツ:
    if (at % 34 < 24) RC_drive(FORWARD, 170);                   //   グリップ円で捕まえ
    else RC_drive(FORWARD, 255);                                //   フレアで流す
    if (at >= 100) {
      laps = laps + 1; ph = 0; at = 0;
      if (act == 0 && laps >= 2) { act = 1; at = 0; laps = 0; }
      else if (act == 1 && laps >= 4) { act = 2; at = 0; }
    }
    return;
  }

  // [Act3] バックエントリー
  if (ph == 0) {                        // 仕切り直し (停止=滑り状態のリセット) → 照準
    if (at < 14) { RC_steer(CENTER); RC_drive(BRAKE, 255); return; }   // 決めの間
    RC_steer(LEFT); RC_drive(FORWARD, 170);   // グリップ回転で前と左前方が開くのを待つ
    // L/R レイは前方±22° — 真横の空きは測れないため、左前方回廊 (L) の開きを代理にする
    if ((C > 1600 && L > 1400) || at > 90) { ph = 1; at = 0; }
    return;
  }
  if (ph == 1) {                        // 助走: 全開直進。技のフットプリントは実測で
    RC_steer(CENTER); RC_drive(FORWARD, 255);   // 前+0.82m/左+1.06m/後0.65m
    if (C > 1100 && C < 1500 && at >= 16 && L > 1200) { ph = 2; at = 0; }
    if (at > 120) { act = 0; at = 0; ph = 0; }   // 条件が揃わない回は見送って開幕へ
    return;
  }
  if (ph == 2) {                        // フェイント (逆へ 0.1s)
    RC_steer(RIGHT); RC_drive(FORWARD, 255);
    if (at >= 2) { ph = 3; at = 0; }
    return;
  }
  if (ph == 3) {                        // 振り出し: BRAKE+舵で u≈0.15・β≈25° へ
    RC_steer(LEFT); RC_drive(BRAKE, 255); //  (SC.backentry 実測 20step=7tick)
    if (at >= 7) { ph = 4; at = 0; }
    return;
  }
  if (ph == 4) {                        // 深化: 255+舵 1.9s で β>90° (テールから滑走。
    RC_steer(LEFT); RC_drive(FORWARD, 255);     //  実測 114step=38tick)
    if (at >= 38 || C < 650 || L < 550 || R < 550) { ph = 5; at = 0; }   // 壁が迫ったら即回収
    return;
  }
  if (ph == 5) {                        // 回収: 駆動を抜いてリア再グリップ → 停止
    RC_steer(CENTER); RC_drive(BRAKE, 255);     //  (実測 87step=29tick)
    if (at >= 30) { ph = 6; at = 0; }
    return;
  }
  RC_steer(CENTER); RC_drive(FREE, 0);  // 決めポーズ 1 秒 → 開幕へ
  if (at >= 20) { act = 0; at = 0; ph = 0; }
}
