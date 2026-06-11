// Lift Whisper — ドリフト FF 用  [Fable Racing Line / Lv6 特殊を管理]  by Fable 5
// 試走(全30ドライ周回): 22/30 完走・平均 1.70 km/h。最も御しにくい車をレート制限で完走へ。
// ドリフトFFは特殊。パワーでは滑らず、「高速旋回中にアクセルを“急に”抜くと」前へ荷重が移り
// リアが軽くなって流れる(リフトオフ・オーバーステア)。つまり他車と逆で“急に抜く”のが危ない。
// しかも滑り出しは速度がある程度高い時だけ(minSp=0.45)。
// ★引き出すロジック(他車と真逆)★
//   (1) 最高速を少し抑え、そもそも滑り出す速度域に入りにくくする。
//   (2) ★肝★ アクセルを“急に”は減らさない=スロットルをレート制限してゆっくり抜く。
//       こうすると荷重移動が穏やかになり、リフトオフでリアが流れるのを防げる。
//   FF normal が「曲げる時にスパッと抜く」のと正反対。同じFFでも逆になるのが学びどころ。

int TOP=234, CRUISE=184, SLOW=114, TCAP=158;
int D_OPEN=605, D_MID=458, D_TURN=410, D_SIDE=196;
int RAMP=10;       // 1ループで下げてよいPWMの最大量(これ以上の急減を禁じる=急なリフトを防ぐ)
int prevPwm;       // 前回出した PWM

void setup() { RC_setup(); prevPwm = 0; }

void loop() {
  int L = sensor0.readRangeSingleMillimeters();
  int C = sensor1.readRangeSingleMillimeters();
  int R = sensor2.readRangeSingleMillimeters();

  int turning = 1;
  if (C < D_TURN) { if (L > R) RC_steer(LEFT); else RC_steer(RIGHT); }
  else if (R < D_SIDE) RC_steer(LEFT);
  else if (L < D_SIDE) RC_steer(RIGHT);
  else { RC_steer(CENTER); turning = 0; }

  int pwm = SLOW;
  if (C > D_OPEN) pwm = TOP;          // (1) TOP をやや低めにして滑り出す速度域を避ける
  else if (C > D_MID) pwm = CRUISE;
  if (turning == 1 && pwm > TCAP) pwm = TCAP;
  // (2) ★リフトオフ回避★ アクセルを“急に”抜かない。減らす時は1ループ RAMP までに制限。
  if (pwm < prevPwm - RAMP) pwm = prevPwm - RAMP;
  prevPwm = pwm;
  RC_drive(FORWARD, pwm);
}
