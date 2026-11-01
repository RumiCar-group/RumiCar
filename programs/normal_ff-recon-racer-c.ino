// Recon Racer (C) — コースを試走で覚えてレーシングライン＋本番は他車を見て最善手  [競技 / フルスケール・Arduino C++・要エンコーダ]  by Opus 5
// ★領域=フルスケール / コース=競技サーキット / 車=ノーマルFF / 車輪エンコーダ(任意)ON★
// Python 版 Recon Racer の忠実な C 移植。走りの考え方は py 版と同一:
//   1周目(試走): 前方3センサーで安全に周回しつつ、エンコーダで測った「スタートからの距離」を
//                インデックスに各地点の 前方の詰まり(コーナーのきつさ sev) と 左右の壁の余地(mapL/mapR)
//                を配列の地図に記録する。前方の最近壁距離 wmin も覚える(近すぎる読み=他車は除く)。
//   2周目以降(本番): 覚えた地図で「この先のきついコーナー」を先読みして手前で減速し、「覚えた壁の
//                距離」と「今のセンサー値」の差から前/横の他車を見つけ、前が詰まったら余地の広い側
//                (=多くはアウト側)から抜き、抜けないなら追従して自滅しない。
// ★C 移植のポイント(ここが配列対応の学びどころ)★
//   ・地図は配列 int sev[N]; / mapL[N]; / mapR[N]; で持つ。長さは const int N=72; の定数で決める。
//   ・要素数は sizeof(sev)/sizeof(sev[0]) でも取れる(Arduino の定石。本シムは要素数を返す)。
//   ・Python の切り捨て除算 a//b は C に無い。本シムの C の / は真の除算なので floor(a/b) と書く
//     (実機 Arduino C は int どうしなら / が切り捨て=同じ結果になる)。
//   ・配列は関数へ渡すと参照が渡る(C のポインタ減衰と同じ)。int f(int a[], int n) と書ける。
// ★D-1: 学習側は ToF×3 + 任意エンコーダのみ。絶対位置・方位(ヨー)は与えられない★
//   エンコーダ未装備だと距離が測れず地図は無効化され、反応のみ(Circuit Racer 相当)で走る。
const int N=72;
const int BL=700;
const int AHEAD=3;
const int TIGHT=20000;
const int VTIGHT=15000;
const int VFAST=26;
const int RTCAP=53;
const int LAP_MIN_S=18000;
const int LAP_HDG=6000;
const int TOP=91;
const int MID=72;
const int SLOW=53;
const int TCAP=58;
const int D_OPEN=33000;
const int D_MID=22000;
const int D_TURN=16000;
const int CONF=150000;   // 信頼区間[mm]: >CONF/範囲外(-3)=遠い/開放
const int CLOSE=1600;
const int BRK=240;
const int BIAS=2000;
const int CARMIN=20000;
const int CARGAP=3000;
const int CAR_SEE=19000;
const int OVT_NEAR=12000;
const int SIDE_OPEN=15000;
const int SIDE_DIFF=6000;
const int FOLLOW=44;
const int FOLLOW_NEAR=9000;
// 地図(配列)。バケツ番号 = スタートからの距離 / BL。
int sev[N];
int mapL[N];
int mapR[N];
float LAPLEN=0;
int LAPB=0;
int phase=0;
float s=0;
float hdg=0;
float prevC=0;
int started=0;
float wmin=99000;
int told=0;
long sc=0;

void setup(){
  RC_setup();
  LAPLEN=0;
  LAPB=0;
  phase=0;
  s=0;
  hdg=0;
  prevC=0;
  started=0;
  wmin=99000;
  told=0;
  sc=0;
  // 配列の初期化。要素数は sizeof で取れる(= N と同じ)。
  for(int i=0;i<sizeof(sev)/sizeof(sev[0]);i++){
    sev[i]=99000;
    mapL[i]=99000;
    mapR[i]=99000;
  }
}

void loop(){
  // --- 自己位置(距離)をエンコーダで積分。未装備(-1)なら距離を測れず地図は無効=反応のみで走る ---
  float v=RC_wheel_speed(REAR);
  if(v<0){ v=0; }
  s=s+v;
  // --- 前方3センサー(範囲外 -3 は「遠く開けている」に正規化) ---
  float L=RC_read(LEFT);
  float C=RC_read(CENTER);
  float R=RC_read(RIGHT);
  if(L<0 || L>CONF){ L=99000; }
  if(C<0 || C>CONF){ C=99000; }
  if(R<0 || R>CONF){ R=99000; }
  float dC=0;
  if(started==1){ dC=prevC-C; }
  prevC=C;
  started=1;
  // --- 距離→バケツ(本番は1周ぶんで折り返す)。py の s//BL は C では floor(s/BL) ---
  int b=floor(s/BL);
  if(phase==1){ b=b%LAPB; }
  if(b<0){ b=0; }
  if(b>=N){ b=N-1; }
  // --- 地図づくり: 前方の詰まり・左右の余地を配列へ記録(最小=最も近い壁) ---
  if(C<sev[b]){ sev[b]=C; }
  if(L<mapL[b]){ mapL[b]=L; }
  if(R<mapR[b]){ mapR[b]=R; }
  if(phase==0 && C>CARMIN && C<wmin){ wmin=C; }
  // --- 前方の他車検知(地図差分) ---
  float cthr=wmin-CARGAP;
  if(cthr>CAR_SEE){ cthr=CAR_SEE; }
  int carF=0;
  if(C<cthr){ carF=1; }
  // --- 側方の他車検知(本番のみ・地図差分) ---
  int carL=0;
  int carR=0;
  if(phase==1){
    if(mapL[b]-L>SIDE_DIFF){ carL=1; }
    if(mapR[b]-R>SIDE_DIFF){ carR=1; }
  }
  // --- 先読み: この先 AHEAD バケツのうち最もきついコーナーを見る(本番のみ) ---
  float coming=99000;
  if(phase==1){
    int j=1;
    while(j<=AHEAD){
      int bb=(b+j)%LAPB;
      if(sev[bb]<coming){ coming=sev[bb]; }
      j=j+1;
    }
  }
  // --- 抜きどころ判断: 前が他車で近く・直線なら、今ほんとうに広く開いている側へ寄せて抜く ---
  int ovt=0;
  if(carF==1 && C<OVT_NEAR && coming>TIGHT){
    int okL=0;
    int okR=0;
    if(L>SIDE_OPEN && carL==0){ okL=1; }
    if(R>SIDE_OPEN && carR==0){ okR=1; }
    if(okL==1 && okR==1){
      if(mapL[b]>mapR[b]){ ovt=1; }
      else if(mapR[b]>mapL[b]){ ovt=-1; }
      else if(L>R){ ovt=1; }
      else { ovt=-1; }
    }
    else if(okL==1){ ovt=1; }
    else if(okR==1){ ovt=-1; }
    if(told==0 && ovt!=0){
      Serial.println("traffic ahead: passing on the open side");
      told=1;
    }
  }
  // --- 操舵の向き(壁回避 → 抜き → センタリング)。st=方位デッドレコニング用の意図方向 ---
  int turning=1;
  int st=0;
  int dir=CENTER;
  if(C<D_TURN){
    if(L>R){ dir=LEFT; st=1; }
    else { dir=RIGHT; st=-1; }
  }
  else if(ovt==1){ dir=LEFT; st=1; }
  else if(ovt==-1){ dir=RIGHT; st=-1; }
  else if(L-R>BIAS){ dir=LEFT; st=1; }
  else if(R-L>BIAS){ dir=RIGHT; st=-1; }
  else { dir=CENTER; turning=0; }
  // ★v2(精密動力学)対応=全舵デューティ変調★ 2ループに1回だけ実際に舵を当て残りは CENTER に戻す。
  sc=sc+1;
  if(turning==1 && (sc%2)<1){ RC_steer(dir); }
  else { RC_steer(CENTER); }
  hdg=hdg+st*v;
  // --- 速度(反応): 前が開けているほど速く・旋回中は抑える ---
  int pwm=SLOW;
  if(C>D_OPEN){ pwm=TOP; }
  else if(C>D_MID){ pwm=MID; }
  if(turning==1 && pwm>TCAP){ pwm=TCAP; }
  // --- 先読み減速(本番のみ) ---
  if(phase==1 && coming<TIGHT && pwm>RTCAP){ pwm=RTCAP; }
  // --- 抜けない時の追従(自滅回避) ---
  if(carF==1 && ovt==0 && C<FOLLOW_NEAR && pwm>FOLLOW){ pwm=FOLLOW; }
  // --- ブレーキ判断: 反応(前方が速く縮む) + 先読み(きついコーナーへ高速接近) ---
  int brake=0;
  if(dC>CLOSE && C<D_OPEN){ brake=1; }
  if(phase==1 && coming<VTIGHT && v>VFAST){ brake=1; }
  if(brake==1){ RC_drive(BRAKE,BRK); }
  else { RC_drive(FORWARD,pwm); }
  // --- 1周ぶん回ったら「試走→本番」へ ---
  if(phase==0 && s>LAP_MIN_S && (hdg>LAP_HDG || hdg<-LAP_HDG)){
    LAPLEN=s;
    LAPB=floor(s/BL);
    if(LAPB<1){ LAPB=1; }
    phase=1;
    Serial.println("course learned: " + LAPB + " zones");
  }
}
