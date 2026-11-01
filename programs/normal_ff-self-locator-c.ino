// Self-Locator (C) — ToF×3 + 車輪エンコーダで「今コースのどこにいるか」を1次元ヒストグラムフィルタで推定する  [競技 / フルスケール・Arduino C++・要エンコーダ]  by Opus 5
// ★領域=フルスケール / コース=競技サーキット / 車=ノーマルFF / 車輪エンコーダ(任意)ON★
// Python 版 Self-Locator の忠実な C 移植。考え方は py 版と同一:
//   1周目(試走): 反応走行しながら「スタートからの距離」を目盛りに、各地点の前方/左右の壁距離を
//                「指紋(fingerprint)」の配列 fpC/fpL/fpR に記録する。スタート地点の壁も覚える。
//   周回検出=loop closure: 方位が一周ぶん回り、スタートの壁パターンとの差 match が「局所最小」に
//                なった点で1周確定(単なる閾値割れは対称ゴーストで早発火するので最小追跡)。
//   2周目以降(本番): 予測(エンコーダで確率分布 w[] を前へ移流)＋観測(指紋残差の有理カーネル)で
//                ヒストグラムフィルタを回し、オドメトリ中心の ±WIN 窓のピークへ GAIN だけ寄せる。
//   他車分離(残差ゲーティング): 覚えた壁より有意に近い channel は位置更新から外す(他車が居ても発散しない)。
// ★C 移植のポイント(配列の学びどころ)★
//   ・指紋と信念(確率分布)を配列で持つ: int fpC[NF]; / float w[NF]; / float wt[NF];(予測用の作業配列)
//   ・py の a//b は floor(a/b)。剰余 % と floor(), atan2(), round(), abs() はそのまま使える。
//   ・配列の要素数は sizeof(fpC)/sizeof(fpC[0]) で取れる(Arduino の定石)。
// ★毎周おなじ反応ラインで走る(先読みで線を変えない)=指紋が周回間で再現し照合が効く。速さより
//   「自分がどこに居るか正確に知る」ことを見せるサンプル(先読み最速化は Apex Strategist へ)。★
// ★D-1: 学習側は ToF×3 + 任意エンコーダのみ。位置は自前計測の学習物(コースデータは渡されない)。★
// 走行(反応)定数
const int CONF=150000;
const int D_TURN=16000;
const int D_MID=22000;
const int D_OPEN=33000;
const int BIAS=2000;
const int TOP=91;
const int MID=72;
const int SLOW=53;
const int TCAP=58;
const int CLOSE=1600;
const int BRK=240;
// 自己位置(loop closure)定数
const int BLF=380;
const int NF=160;
const int LAP_MIN_S=30000;
const int LAP_HDG=6300;
const int CLOSURE=4000;
const int MINWAIT=6;
// 観測カーネルσ² (残差の効き。σ大=甘い・小=多峰へ発散)
const float SC2=64000000;
const float SS2=100000000;
const int CARGAP2=9000;
const float DT=0.05;
const int WIN=7;
const float GAIN=0.05;
// 指紋(fine バケツ)・ヒストグラム
int fpC[NF];
int fpL[NF];
int fpR[NF];
int fpN[NF];
float w[NF];
float wt[NF];
float LAPLEN=0;
int LAPBF=0;
int lp=0;
float s=0;
float sl=0;
float hdg=0;
float prevC=0;
int started=0;
long sc=0;
float fs0C=0;
float fs0L=0;
float fs0R=0;
int lstart=0;
int armed=0;
float mmin=999999;
float mminSl=0;
int sinceMin=0;
float mdbg=0;
float estb=0;
float ests=0;
float conf=0;
int carF=0;
int carL=0;
int carR=0;
float pe=0;
float betaEst=0;
long betaN=0;

void setup(){
  RC_setup();
  LAPLEN=0;
  LAPBF=0;
  lp=0;
  s=0;
  sl=0;
  hdg=0;
  prevC=0;
  started=0;
  sc=0;
  fs0C=0;
  fs0L=0;
  fs0R=0;
  lstart=0;
  armed=0;
  mmin=999999;
  mminSl=0;
  sinceMin=0;
  mdbg=0;
  estb=0;
  ests=0;
  conf=0;
  carF=0;
  carL=0;
  carR=0;
  pe=0;
  betaEst=0;
  betaN=0;
  for(int i=0;i<NF;i++){
    fpC[i]=99000;
    fpL[i]=99000;
    fpR[i]=99000;
    fpN[i]=0;
    w[i]=0;
    wt[i]=0;
  }
}

void loop(){
  float v=RC_wheel_speed(REAR);
  if(v<0){ v=0; }
  s=s+v;
  sl=sl+v;
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
  // ── 反応走行 (毎周同一ライン=指紋再現性の源) ──
  int turning=1;
  int st=0;
  int dir=CENTER;
  if(C<D_TURN){
    if(L>R){ dir=LEFT; st=1; }
    else { dir=RIGHT; st=-1; }
  }
  else if(L-R>BIAS){ dir=LEFT; st=1; }
  else if(R-L>BIAS){ dir=RIGHT; st=-1; }
  else { dir=CENTER; turning=0; }
  sc=sc+1;
  if(turning==1 && (sc%2)<1){ RC_steer(dir); }
  else { RC_steer(CENTER); }
  hdg=hdg+st*v;
  int pwm=SLOW;
  if(C>D_OPEN){ pwm=TOP; }
  else if(C>D_MID){ pwm=MID; }
  if(turning==1 && pwm>TCAP){ pwm=TCAP; }
  if(dC>CLOSE && C<D_OPEN){ RC_drive(BRAKE,BRK); }
  else { RC_drive(FORWARD,pwm); }
  // ── 指紋づくり (fine バケツ・lap-local 弧長 sl 目盛り・初回入場時に1回) ──
  int bf=floor(sl/BLF);
  if(bf<0){ bf=0; }
  if(bf>=NF){ bf=NF-1; }
  if(lstart==0){
    fs0C=C;
    fs0L=L;
    fs0R=R;
    lstart=1;
  }
  if(lp==0){
    if(fpN[bf]==0){
      fpC[bf]=C;
      fpL[bf]=L;
      fpR[bf]=R;
      fpN[bf]=1;
    }
  }
  // ── loop closure (局所最小): 方位が一周ぶん回ったら武装し、スタート指紋との差 match が「最小」に
  //    なった点=真のスタートで1周確定。単なる閾値割れは対称ゴーストで早発火するので最小追跡。──
  float match=abs(C-fs0C)+abs(L-fs0L)+abs(R-fs0R);
  mdbg=match;
  if(sl>LAP_MIN_S && (hdg>LAP_HDG || hdg<-LAP_HDG)){ armed=1; }
  if(armed==1){
    if(match<mmin){
      mmin=match;
      mminSl=sl;
      sinceMin=0;
    }
    else { sinceMin=sinceMin+1; }
    // 最小を過ぎ (match が再上昇) かつ最小が十分小さいなら発火。LAPLEN=mminSl (真の1周長)。
    if(sinceMin>MINWAIT && mmin<CLOSURE){
      if(lp==0){
        LAPLEN=mminSl;
        LAPBF=floor(mminSl/BLF);
        if(LAPBF<1){ LAPBF=1; }
        if(LAPBF>=NF){ LAPBF=NF-1; }
        int i=0;
        while(i<LAPBF){
          if(fpN[i]==0){
            int pj=i-1;
            if(pj<0){ pj=LAPBF-1; }
            fpC[i]=fpC[pj];
            fpL[i]=fpL[pj];
            fpR[i]=fpR[pj];
          }
          w[i]=0.0;
          i=i+1;
        }
        lp=1;
        Serial.println("localizer ready: " + LAPBF + " bins (loop closed)");
      }
      // 再アンカー: sl を「最小からの走行ぶん」へ。信念もそこへ。
      sl=sl-mminSl;
      int bb=floor(sl/BLF);
      if(bb<0){ bb=0; }
      if(bb>=LAPBF){ bb=LAPBF-1; }
      estb=bb;
      int i2=0;
      while(i2<LAPBF){
        if(i2==bb){ w[i2]=1.0; }
        else { w[i2]=0.0; }
        i2=i2+1;
      }
      armed=0;
      mmin=999999;
      sinceMin=0;
    }
  }
  // ============ 自己位置推定 (lp==1・ヒストグラムフィルタ) ============
  if(lp==1){
    // 予測: エンコーダで前進 dsb ビン (小数)。前方移流 + 小拡散。
    float dsb=v/BLF;
    if(dsb<0){ dsb=0; }
    if(dsb>0.9){ dsb=0.9; }
    int i=0;
    while(i<LAPBF){
      wt[i]=w[i];
      i=i+1;
    }
    i=0;
    while(i<LAPBF){
      int jm=i-1;
      if(jm<0){ jm=LAPBF-1; }
      int jp=i+1;
      if(jp>=LAPBF){ jp=0; }
      w[i]=(1-dsb)*wt[i]+dsb*wt[jm];
      w[i]=0.96*w[i]+0.02*wt[jm]+0.02*wt[jp];
      i=i+1;
    }
    // 他車ゲーティング (現ベスト bin の指紋より有意に近ければ その channel は位置更新から除外)
    int cb=floor(estb);
    if(cb<0){ cb=0; }
    if(cb>=LAPBF){ cb=LAPBF-1; }
    int useC=1;
    int useL=1;
    int useR=1;
    if(C<fpC[cb]-CARGAP2){ useC=0; }
    if(L<fpL[cb]-CARGAP2){ useL=0; }
    if(R<fpR[cb]-CARGAP2){ useR=0; }
    // 観測: 指紋残差 → 有理カーネル k=1/(1+err)。ソフト適用で1tick の誤マッチでは信念が飛ばない。
    i=0;
    while(i<LAPBF){
      float err=0;
      if(useC==1){
        float d0=C-fpC[i];
        err=err+d0*d0/SC2;
      }
      if(useL==1){
        float d1=L-fpL[i];
        err=err+d1*d1/SS2;
      }
      if(useR==1){
        float d2=R-fpR[i];
        err=err+d2*d2/SS2;
      }
      float kk=1/(1+err);
      w[i]=w[i]*(0.6+0.4*kk);
      i=i+1;
    }
    // 正規化 (総和ガード)
    float sm=0;
    i=0;
    while(i<LAPBF){
      sm=sm+w[i];
      i=i+1;
    }
    if(sm<0.000000001){
      i=0;
      while(i<LAPBF){
        w[i]=1.0/LAPBF;
        i=i+1;
      }
      sm=1.0;
    }
    i=0;
    while(i<LAPBF){
      w[i]=w[i]/sm;
      i=i+1;
    }
    // 推定 ŝ = オドメトリ位置を中心に ±WIN 窓内の地図照合ピークへ GAIN だけ寄せる (有界な地図補正)。
    float center=sl/BLF;
    if(center<0){ center=0; }
    if(center>=LAPBF){ center=center%LAPBF; }
    int c0=floor(center);
    if(c0<0){ c0=0; }
    if(c0>=LAPBF){ c0=LAPBF-1; }
    float pk=0;
    int pi=c0;
    int jw=-WIN;
    while(jw<=WIN){
      int kw=(c0+jw)%LAPBF;
      if(kw<0){ kw=kw+LAPBF; }
      if(w[kw]>pk){
        pk=w[kw];
        pi=kw;
      }
      jw=jw+1;
    }
    float csum=0;
    float wsum=0;
    int jj=-3;
    while(jj<=3){
      int kk2=(pi+jj)%LAPBF;
      if(kk2<0){ kk2=kk2+LAPBF; }
      csum=csum+w[kk2]*jj;
      wsum=wsum+w[kk2];
      jj=jj+1;
    }
    float off=0;
    if(wsum>0){ off=csum/wsum; }
    float estRaw=pi+off;
    // 相補フィルタ: 出力=オドメトリ予測 + GAIN×(観測ピーク−予測) の円環ブレンド。
    float diff=estRaw-center;
    if(diff>LAPBF/2){ diff=diff-LAPBF; }
    if(diff<-LAPBF/2){ diff=diff+LAPBF; }
    estb=center+GAIN*diff;
    if(estb<0){ estb=estb+LAPBF; }
    if(estb>=LAPBF){ estb=estb-LAPBF; }
    ests=estb*BLF;
    conf=pk;
    // 他車フラグ (recall 用・driver は他車回避しないがフラグは出す)
    carF=0;
    carL=0;
    carR=0;
    if(useC==0){ carF=1; }
    if(useL==0){ carL=1; }
    if(useR==0){ carR=1; }
    // ── β 再挑戦 (地図事前分布つき・実験): レコンライン からの横ずれ e とそのレート ──
    int cbf=floor(estb);
    if(cbf<0){ cbf=0; }
    if(cbf>=LAPBF){ cbf=LAPBF-1; }
    if(useL==1 && useR==1 && v>3){
      float e=((fpL[cbf]-L)+(R-fpR[cbf]))/2;
      float de=e-pe;
      pe=e;
      float braw=atan2(de/1000/DT,v)*57.29578;
      betaEst=0.7*betaEst+0.3*braw;
      betaN=betaN+1;
    }
    if((sc%10)==0){
      Serial.println("pos=bin " + round(estb) + "/" + LAPBF + " conf=" + round(conf*100) + "%");
    }
  }
}
