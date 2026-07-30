# Apex Strategist — 試走で覚えた地図から速度プロファイルを計画して先読み最速で周回する  [競技 / フルスケール・Python・要エンコーダ]  by Opus 4.8
# ★領域=フルスケール / コース=競技サーキット / 車=ノーマルFF / 車輪エンコーダ(任意)ON★
# 標準の Circuit Racer は v2 で全舵=切りすぎスピンを避けるため保守的(TCAP 低め)に組み、実測 ~210s/3周(~31m/s
# 巡航)で流す。Apex Strategist は本番前に試走(recon)でコースを地図化し、その地図で自信を持って攻める:
#   A) 走りの土台=recon で「踏める」と分かったコースを反応型で"攻めチューン"する(直線=踏み切り TOP2=255・
#      操舵中だけ TCAP=120 で前輪空転→巻き込みを抑える=v2 の FF レッスン)。実測 ~163–187s/3周=Circuit Racer 比
#      11–22% 速い(crash0)。★この差の大半はこの"攻めチューン"が生む★=地図が「攻めても大丈夫」を保証する。
#   B) 速度プロファイル計画: recon で曲率代理 cr[b]=各バケツで実際に舵を当てていた割合(速度重み)を作り、
#      vmax[b]=VMAX−KMAP×cr[b](直線ほど速い)→前進/後退パス(O(N))で実現可能な計画へ。本番は自己位置
#      (エンコーダ距離+loop closure)で今のバケツを知り、地図のきつい区間の手前でスロットルを絞る(加速ガバナ)。
# ★正直な実測(CI-14・AO8/AO10 と同型の限界)★ このコースは4コーナー全部 R≈116m と均一で"どのコーナーを速く"の
#   差が無く、かつ ToF 自己位置は幅28m・2回対称の超楕円では loop closure が粗い(~数バケツ=AO10 の絶対位置限界と
#   同根)。ゆえに「地図で更に踏み込む精密な後追い制動」は安全に成立せず、速度プロファイルの純利得は概ね中立
#   (加速ガバナは安全側の保険)。=速いのは"recon が保証する攻めチューン"で、精密プロファイルはこの清潔コースでは
#   出番が小さい、という正直な結果(勝てない所は勝てない=AO8 のドリフト NO-GO・AO10 の ToF<オドメトリ と同じ教訓)。
# ★戦略ドリフト(教材)★ コーナー毎に grip/drift を選ぶ状態機械を持つ: cr[b] が幾何限界級(=フル舵を当て
#   続けないと曲がれない≈R<1.1×R_min。car の最小回転半径 R_min=5.84m)のバケツだけ drift 発動。だが AO8 の
#   go/no-go 実測どおり乾燥の中高速コーナーでは3値ステアの持続ドリフトは grip に勝てず出口再グリップも作れず
#   NO-GO。この競技サーキットは最小 R≈116m ≫ 6.4m ゆえ drift-GO のバケツは存在せず状態機械は常に grip・非発動
#   =「速いのは正確な grip、drift は限界を超えた時だけ・ここでは出番なし」を正直に見せる(勝てない所は勝てない)。
# ★D-1: 学習側は ToF×3 + 任意エンコーダのみ。地図・自己位置・速度計画は全て自前計測の学習物。★
#   エンコーダ未装備なら距離を測れず計画不能 → 反応型(Circuit Racer 相当)へ正直にフォールバックする。
N=72
BL=700
CONF=150000
# 反応ドライブ (試走ラップ + エンコーダ無しフォールバック) = Recon/Circuit 系の清潔チューン
D_TURN=16000
D_MID=22000
D_OPEN=33000
BIAS=2000
TOP=91
MID=72
SLOW=53
TCAP0=58
CLOSE=1600
BRK=240
RSTEER=2
# 本番(phase1)の反応速度=前方開放度ベースで直線は踏み切り(TOP2)・操舵中は TCAP 頭打ち(清潔高速の土台)
TOP2=255
MID2=200
SLOW2=120
# loop closure (方位一周 + スタート指紋の局所最小)。odo/m≈20・1周≈41960odo(BL=700→~60バケツ)・hdg≈7100/周。
# 定数は comp_localize(AO10)の実証値=0.54bin(スタートを 13% 早取りしない)。
LAP_MIN_S=30000
LAP_HDG=6300
CLOSURE=4000
MINWAIT=6
SFS0=2500
# 速度プロファイル計画 (BL=700odo≈35m/バケツ・FWD/BWD=2a·Δs の v² 予算)
VMAX=58
VMIN=34
KMAP=54
FWD=350
BWD=1400
LOOK=3
PWMAX=255
PWB=100
KP=12
OVER=2
BRKG=220
TCAP=120
CORNTH=46
# 他車 / TTC (追突誘発ゼロ)
CARGAP=3000
CAR_SEE=19000
CARMIN=20000
SIDE_DIFF=6000
TTCB=255
TTC_MIN=0.8
DT=0.05
FOLLOW=44
FOLLOW_NEAR=9000
# 戦略ドリフト状態機械 (教材=このコースでは非発動)
DRIFT_ON=1
KGO=0.85
DBRK=255
# 地図・計画・状態
sev=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
mapL=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
mapR=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
cS=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
cV=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
vprof=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dgo=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
phase=0
s=0
sl=0
hdg=0
prevC=0
started=0
wmin=99000
sc=0
LAPLEN=0
LAPB=0
fs0C=0
fs0L=0
fs0R=0
lstart=0
armed=0
mmin=999999
mminSl=0
sinceMin=0
estb=0
dstate=0
driftFired=0
maxcr=0

def setup():
    global phase, s, sl, hdg, prevC, started, wmin, sc, LAPLEN, LAPB, fs0C, fs0L, fs0R, lstart, armed, mmin, mminSl, sinceMin, estb, dstate, driftFired, maxcr
    RC_setup()
    phase=0
    s=0
    sl=0
    hdg=0
    prevC=0
    started=0
    wmin=99000
    sc=0
    LAPLEN=0
    LAPB=0
    fs0C=0
    fs0L=0
    fs0R=0
    lstart=0
    armed=0
    mmin=999999
    mminSl=0
    sinceMin=0
    estb=0
    dstate=0
    driftFired=0
    maxcr=0
    for i in range(N):
        sev[i]=99000
        mapL[i]=99000
        mapR[i]=99000
        cS[i]=0
        cV[i]=0
        vprof[i]=0
        dgo[i]=0

def loop():
    global phase, s, sl, hdg, prevC, started, wmin, sc, LAPLEN, LAPB, fs0C, fs0L, fs0R, lstart, armed, mmin, mminSl, sinceMin, estb, dstate, driftFired, maxcr
    v=RC_wheel_speed(REAR)
    enc=1
    if v<0:
        enc=0
        v=0
    s=s+v
    sl=sl+v
    L=RC_read(LEFT)
    C=RC_read(CENTER)
    R=RC_read(RIGHT)
    if L<0 or L>CONF:
        L=99000
    if C<0 or C>CONF:
        C=99000
    if R<0 or R>CONF:
        R=99000
    dC=0
    if started==1:
        dC=prevC-C
    prevC=C
    started=1
    b=sl//BL
    if phase==1:
        b=b%LAPB
    if b<0:
        b=0
    if b>=N:
        b=N-1
    # ── 操舵の向き (広い方へ) + 方位デッドレコニング intent st ──
    turning=1
    st=0
    dir=CENTER
    if C<D_TURN:
        if L>R:
            dir=LEFT
            st=1
        else:
            dir=RIGHT
            st=0-1
    elif L-R>BIAS:
        dir=LEFT
        st=1
    elif R-L>BIAS:
        dir=RIGHT
        st=0-1
    else:
        dir=CENTER
        turning=0
    # ── 戦略ドリフト状態機械: このバケツが drift-GO なら drift 制御、そうでなければ grip ──
    dstate=0
    drifting=0
    if phase==1 and DRIFT_ON==1 and dgo[b]==1:
        drifting=1
        dstate=3
        driftFired=driftFired+1
    # ── 操舵の適用 (デューティ変調で実効舵角を薄める=v2 の大R で全舵切りすぎ→後輪破り を回避) ──
    sc=sc+1
    applSign=0
    if drifting==1:
        RC_steer(dir)
        applSign=st
    elif turning==1 and (sc%RSTEER)<1:
        RC_steer(dir)
        applSign=st
    else:
        RC_steer(CENTER)
    hdg=hdg+st*v
    # ── 試走(phase0): 地図づくり (最近壁・曲率代理=当て舵の割合・スタート指紋) ──
    if phase==0:
        if C<sev[b]:
            sev[b]=C
        if L<mapL[b]:
            mapL[b]=L
        if R<mapR[b]:
            mapR[b]=R
        cS[b]=cS[b]+applSign*v
        cV[b]=cV[b]+v
        if C>CARMIN and C<wmin:
            wmin=C
        if lstart==0 and sl>SFS0:
            fs0C=C
            fs0L=L
            fs0R=R
            lstart=1
    # ── loop closure(1回だけ): 方位一周後、スタート指紋との差 match の局所最小=1周確定→LAPLEN 確定+
    #    原点をスタートへ整合。以後は純オドメトリで毎周 sl-=LAPLEN 再アンカー(AO10=オドメトリ骨格が
    #    最良・周回内0.30bin。指紋照合は対称コースで多峰=毎周は不安定ゆえ最初の1回のみ使う)。──
    match=abs(C-fs0C)+abs(L-fs0L)+abs(R-fs0R)
    if phase==0 and sl>LAP_MIN_S and (hdg>LAP_HDG or hdg<0-LAP_HDG):
        armed=1
    if phase==0 and armed==1:
        if match<mmin:
            mmin=match
            mminSl=sl
            sinceMin=0
        else:
            sinceMin=sinceMin+1
        if sinceMin>MINWAIT and mmin<CLOSURE:
            if phase==0:
                LAPLEN=mminSl
                LAPB=mminSl//BL
                if LAPB<1:
                    LAPB=1
                if LAPB>=N:
                    LAPB=N-1
                # 曲率代理 cr=|Σ当て舵×v|/Σv → 3バケツ平滑 → vmax 線形写像 → 前進/後退パス
                i=0
                while i<LAPB:
                    im=i-1
                    if im<0:
                        im=LAPB-1
                    ip=i+1
                    if ip>=LAPB:
                        ip=0
                    d0=cV[i]
                    if d0<1:
                        d0=1
                    dm=cV[im]
                    if dm<1:
                        dm=1
                    dp=cV[ip]
                    if dp<1:
                        dp=1
                    cr=(abs(cS[i])/d0*2+abs(cS[im])/dm+abs(cS[ip])/dp)/4
                    if cr>maxcr:
                        maxcr=cr
                    vm=VMAX-KMAP*cr
                    if vm>VMAX:
                        vm=VMAX
                    if vm<VMIN:
                        vm=VMIN
                    vprof[i]=vm
                    dgo[i]=0
                    if cr>=KGO:
                        dgo[i]=1
                    i=i+1
                p=0
                while p<2:
                    i=0
                    while i<LAPB:
                        j=i+1
                        if j>=LAPB:
                            j=0
                        lim=sqrt(vprof[i]*vprof[i]+FWD)
                        if vprof[j]>lim:
                            vprof[j]=lim
                        i=i+1
                    p=p+1
                p=0
                while p<2:
                    i=LAPB-1
                    while i>=0:
                        j=i-1
                        if j<0:
                            j=LAPB-1
                        lim=sqrt(vprof[i]*vprof[i]+BWD)
                        if vprof[j]>lim:
                            vprof[j]=lim
                        i=i-1
                    p=p+1
                phase=1
                armed=0
                Serial.println("plan ready: " + LAPB + " zones (max curvature " + round(maxcr*100) + "%)")
    # ── 本番: 純オドメトリで毎周 sl-=LAPLEN 再アンカー。原点はスタート(tick0)のまま=地図と同一原点で
    #    整合(地図は phase0 の sl-from-start で構築・照会も同じ sl→整合。指紋シフトはしない)。骨格=
    #    エンコーダ距離で周回内高精度(AO10・0.30bin)、周回境界だけ LAPLEN で畳む(ドリフト有界)。──
    if phase==1 and sl>=LAPLEN:
        sl=sl-LAPLEN
    # ── b を再アンカー後で取り直す ──
    b=sl//BL
    if phase==1:
        b=b%LAPB
    if b<0:
        b=0
    if b>=N:
        b=N-1
    if phase==1:
        estb=b
    # ── 速度: 試走/フォールバックは反応、本番は速度プロファイル ──
    if phase==0 or enc==0:
        # 試走(phase0)もエンコーダ無しフォールバックも「反応型 高速」(TOP2/操舵中 TCAP)で走る=無駄な遅い
        #   周回を作らない(試走ラップも本番同等に速い・地図はその間に作る)。反応制動(dC)で安全。
        pwm=SLOW2
        if C>D_OPEN:
            pwm=TOP2
        elif C>D_MID:
            pwm=MID2
        if turning==1 and pwm>TCAP:
            pwm=TCAP
        if dC>CLOSE and C<D_OPEN:
            RC_drive(BRAKE,BRK)
        else:
            RC_drive(FORWARD,pwm)
    elif drifting==1:
        # drift 制御 (TURN_IN/DRIFT: フル舵 + ブレーキ脈動で後軸を流す。EXIT は grip 復帰=非 drift 側)
        if (sc%2)<1:
            RC_drive(BRAKE,DBRK)
        else:
            RC_drive(FORWARD,PWMAX)
    else:
        # 他車検知 (地図差分)
        carF=0
        cthr=wmin-CARGAP
        if cthr>CAR_SEE:
            cthr=CAR_SEE
        if C<cthr:
            carF=1
        # 計画目標速度: この先 LOOK バケツの最小 vprof(=手前から減速)。地図が視程(コーン~25m)の地平を
        #   延ばし、反応型が「見えてから」制動する頭打ちを破る。min-over-窓 なので自己位置が数バケツ
        #   ズレても頑健(コーナーを取り逃さない)。
        tgt=vprof[b]
        j=1
        while j<=LOOK:
            bb=(b+j)%LAPB
            if vprof[bb]<tgt:
                tgt=vprof[bb]
            j=j+1
        # 反応の速さ=前方開放度ベース+操舵中 TCAP 頭打ち(Circuit Racer で実証した清潔高速の土台=直線は
        #   踏み切って高速へ・操舵中は前輪空転→巻き込みを TCAP で防ぐ=v2 の FF レッスン)。
        pwm=SLOW2
        if C>D_OPEN:
            pwm=TOP2
        elif C>D_MID:
            pwm=MID2
        if turning==1 and pwm>TCAP:
            pwm=TCAP
        # ★地図の速度プロファイルを「加速ガバナ」として使う★ この先(LOOK バケツ内)に計画速度の低い=きつい
        #   区間があれば、そこへ突っ込む手前でスロットルを計画速度ぶんに絞る(ハードブレーキでなく throttle-lift)。
        #   地図は加速を「抑える」だけ=位置が数バケツずれても踏み込み過ぎで刺さらない(常に安全側)。これが
        #   コーン視程(~25m)の地平を地図で延ばす本質=反応型が「見えてから」しか緩められないのを先に緩める。
        if tgt<CORNTH and (turning==1 or dC>0):
            pcap=PWB+KP*(tgt-v)
            if pcap<0:
                pcap=0
            if pcap<pwm:
                pwm=pcap
        # TTC ガード: 前車へ接近しすぎたら制動オーバーライド(追突誘発ゼロ)
        ttcB=0
        if carF==1 and dC>0 and v>1:
            clos=dC/1000/DT
            if clos>0:
                ttc=C/1000/clos
                if ttc<TTC_MIN:
                    ttcB=1
        if carF==1 and C<FOLLOW_NEAR and v>FOLLOW:
            ttcB=1
        # 制動: 反応(前方が速く縮む=安全網) OR TTC。ハードブレーキは反応/TTC のみ(地図は加速ガバナで
        #   throttle-lift のみ=位置ズレでも刺さらない)。Circuit Racer と同じく制動は操舵中でも可(実証済清潔)。
        brake=0
        if dC>CLOSE and C<D_OPEN:
            brake=1
        if ttcB==1:
            brake=1
        if brake==1:
            RC_drive(BRAKE,BRK)
        else:
            RC_drive(FORWARD,pwm)
