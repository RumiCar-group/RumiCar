# Recon Racer — コースを試走で覚えてレーシングライン＋本番は他車を見て最善手  [競技 / フルスケール・Python・要エンコーダ]  by Opus 4.8
# ★領域=フルスケール / コース=競技サーキット / 車=ノーマルFF / 車輪エンコーダ(任意)ON★
# 実車のレースは本番前に試走(recon)してコースを研究し、本番は他車を見ながら抜きどころを計算する。
#   1周目(試走): 前方3センサーで安全に周回しつつ、エンコーダで測った「スタートからの距離」を
#                インデックスに各地点の 前方の詰まり(=コーナーのきつさ sev) と 左右の壁の余地(mapL/mapR) を
#                地図に記録する。前方の最も近い壁の距離 wmin も覚える(壁にしては近すぎる読み=他車は除く)。
#   2周目以降(本番): 覚えた地図で「この先のきついコーナー」を先読みして手前で減速する。さらに
#                「覚えた壁の距離」と「今のセンサー値」の差から前/横の他車を見つけ、前が詰まったら
#                余地の広い側(=多くはアウト側)から抜き、抜けないなら追従して自滅しない。
# 反応のみの Circuit Racer は速度が乗るコーナーで止まりきれずスピンするが、Recon Racer は地図の
# 先読みでそこを事前に殺し、他車は地図差分で見分けて避ける/抜く=試走で覚える価値(本番フロー実証)。
# ★視野コーン(25°)対応(#27・Stage AM)★ 距離センサーは扇内最近を返すため中央 C の読みが直線レイ時代の
#   約半分になる(実測 cone/thin≈48%)。前方/側方の距離しきい値(TIGHT/D_OPEN/CARMIN/SIDE_OPEN 等)は
#   すべて約半分へ下げた。地図値(sev/mapL/mapR)も現センサーで測るので他車検知の差分も自動的に整合する。
#   一方 距離(エンコーダ)・方位(操舵履歴)・速度(pwm)はコーン非依存なので不変。旧しきい値のままだと
#   「常にコーナー」と誤認して踏めずスピンした(実測 111→89秒・β180°→8°)。
# ★精密動力学 v2 対応(Stage AO)★ 走行エンジンが v2(4輪 two-track・忠実な荷重移動)に変わると、3値ステアの
#   全舵は大R コーナーには切りすぎ=後輪を破ってオーバーステア→スピンする。対策は Circuit Racer と同じ「舵の
#   デューティ変調」(2ループに1回だけ実際に舵を当て残りは CENTER=実効舵角を半分に薄める)。あわせて旋回中の
#   上限 TCAP を 72→58 に下げた(v2 のコーナー限界速度は低い。Recon は先読みブレーキが多く後輪を抜きやすい)。
# ★D-1: 学習側は ToF×3 + 任意エンコーダのみ。絶対位置・方位(ヨー)は与えられない★ → 自己位置は
#   「エンコーダ速度の積み上げ=距離」と「操舵履歴=方位の目安」のデッドレコニング。1周の区切りは
#   方位が一周ぶん回ったことで検出する。他車検知に新しいセンサーは要らない: ToF は壁と他車を区別せず
#   最も近い距離を返すので、「試走で覚えた壁の距離」より今が有意に近ければ そこに他車が居る。
#   エンコーダ未装備だと距離が測れず地図は無効化され、反応のみ(Circuit Racer 相当)で走る=正直なフォールバック。
N=72
BL=700
AHEAD=3
TIGHT=20000
VTIGHT=15000
VFAST=26
RTCAP=53
LAP_MIN_S=18000
LAP_HDG=6000
TOP=91
MID=72
SLOW=53
TCAP=58
D_OPEN=33000
D_MID=22000
D_TURN=16000
CONF=150000  # 信頼区間[mm]: >CONF/範囲外(-3)=遠い/開放。実車レーダ相当150mレンジ設計ゆえ信頼区間もレーダ地平。
#            実短距離ToFの地面拘束は~5m(=250mm×領域スケール20)。実機/自機では下げて要調整(下げるほど遠くの壁を見ず反応遅れ)。
CLOSE=1600
BRK=240
BIAS=2000
CARMIN=20000
CARGAP=3000
CAR_SEE=19000
OVT_NEAR=12000
SIDE_OPEN=15000
SIDE_DIFF=6000
FOLLOW=44
FOLLOW_NEAR=9000
sev=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
mapL=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
mapR=[99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000, 99000]
LAPLEN=0
LAPB=0
phase=0
s=0
hdg=0
prevC=0
started=0
wmin=99000
told=0
sc=0

def setup():
    global LAPLEN, LAPB, phase, s, hdg, prevC, started, wmin, told, sc
    RC_setup()
    LAPLEN=0
    LAPB=0
    phase=0
    s=0
    hdg=0
    prevC=0
    started=0
    wmin=99000
    told=0
    sc=0
    for i in range(N):
        sev[i]=99000
        mapL[i]=99000
        mapR[i]=99000

def loop():
    global LAPLEN, LAPB, phase, s, hdg, prevC, started, wmin, told, sc
    # --- 自己位置(距離)をエンコーダで積分。未装備(-1)なら距離を測れず地図は無効=反応のみで走る ---
    v=RC_wheel_speed(REAR)
    if v<0:
        v=0
    s=s+v
    # --- 前方3センサー(範囲外 -3 は「遠く開けている」に正規化) ---
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
    # --- 距離→バケツ(本番は1周ぶんで折り返す) ---
    b=s//BL
    if phase==1:
        b=b%LAPB
    if b<0:
        b=0
    if b>=N:
        b=N-1
    # --- 地図づくり: 前方の詰まり(コーナーのきつさ sev)・左右の余地(mapL/mapR)を記録(最小=最も近い壁)。
    #     前方の最近壁距離 wmin は「壁にしては近すぎる(=他車)」読み(CARMIN 未満)を除いて学習する ---
    if C<sev[b]:
        sev[b]=C
    if L<mapL[b]:
        mapL[b]=L
    if R<mapR[b]:
        mapR[b]=R
    if phase==0 and C>CARMIN and C<wmin:
        wmin=C
    # --- 前方の他車検知(地図差分): 今の前方が「覚えた最も近い壁 wmin」よりさらに近ければ他車。壁は決して
    #     CAR_SEE より近くないと学んでいるので、試走中(地図形成前)でも上限 CAR_SEE で前方の他車を見分けられる ---
    cthr=wmin-CARGAP
    if cthr>CAR_SEE:
        cthr=CAR_SEE
    carF=0
    if C<cthr:
        carF=1
    # --- 側方の他車検知(本番のみ・地図差分): 覚えた左右の壁 mapL/mapR より live が有意に近ければ そこに他車 ---
    carL=0
    carR=0
    if phase==1:
        if mapL[b]-L>SIDE_DIFF:
            carL=1
        if mapR[b]-R>SIDE_DIFF:
            carR=1
    # --- 先読み: この先 AHEAD バケツのうち最もきついコーナーを見る(本番のみ) ---
    coming=99000
    if phase==1:
        j=1
        while j<=AHEAD:
            bb=(b+j)%LAPB
            if sev[bb]<coming:
                coming=sev[bb]
            j=j+1
    # --- 抜きどころ判断: 前が他車で近く(<OVT_NEAR)・直線(coming>TIGHT)なら、今ほんとうに広く開いている側
    #     (live>SIDE_OPEN かつ その側に他車なし)へ寄せて抜く。両側が広いなら 地図が覚えた余地の大きい側=アウト
    #     を選ぶ。広い側が無ければ抜かず控える(=FOLLOW で追従して自滅しない)。live を使うので壁へは寄らない ---
    ovt=0
    if carF==1 and C<OVT_NEAR and coming>TIGHT:
        okL=0
        okR=0
        if L>SIDE_OPEN and carL==0:
            okL=1
        if R>SIDE_OPEN and carR==0:
            okR=1
        if okL==1 and okR==1:
            if mapL[b]>mapR[b]:
                ovt=1
            elif mapR[b]>mapL[b]:
                ovt=0-1
            elif L>R:
                ovt=1
            else:
                ovt=0-1
        elif okL==1:
            ovt=1
        elif okR==1:
            ovt=0-1
        if told==0 and ovt!=0:
            Serial.println("traffic ahead: passing on the open side")
            told=1
    # --- 操舵の向き(壁回避 → 抜き → センタリング)を決める。st=方位デッドレコニング用の意図方向 ---
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
    elif ovt==1:
        dir=LEFT
        st=1
    elif ovt==0-1:
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
    # ★v2(精密動力学)対応=全舵デューティ変調★ 2ループに1回だけ実際に舵を当て残りは CENTER に戻す=実効舵角を
    #   半分に薄める(大R で全舵は切りすぎ→後輪破りスピン。Circuit Racer と同じ v2 対策)。方位デッドレコニング
    #   (hdg)は意図方向 st で積む=デューティに関係なく周回検出は安定。
    sc=sc+1
    if turning==1 and (sc%2)<1:
        RC_steer(dir)
    else:
        RC_steer(CENTER)
    hdg=hdg+st*v
    # --- 速度(反応): 前が開けているほど速く・旋回中は抑える ---
    pwm=SLOW
    if C>D_OPEN:
        pwm=TOP
    elif C>D_MID:
        pwm=MID
    if turning==1 and pwm>TCAP:
        pwm=TCAP
    # --- 先読み減速: 覚えたコーナーが近いなら手前で速度を落とす(本番のみ) ---
    if phase==1 and coming<TIGHT and pwm>RTCAP:
        pwm=RTCAP
    # --- 抜けない時(前が他車・両側塞がり)で 接近しすぎ(<FOLLOW_NEAR)なら追従速度まで落として自滅を避ける
    #     (遠い検知では速度を保つ=団子で全体が鈍らない) ---
    if carF==1 and ovt==0 and C<FOLLOW_NEAR and pwm>FOLLOW:
        pwm=FOLLOW
    # --- ブレーキ判断: 反応(前方が速く縮む) + 先読み(きついコーナーへ高速接近) ---
    brake=0
    if dC>CLOSE and C<D_OPEN:
        brake=1
    if phase==1 and coming<VTIGHT and v>VFAST:
        brake=1
    if brake==1:
        RC_drive(BRAKE,BRK)
    else:
        RC_drive(FORWARD,pwm)
    # --- 1周ぶん回ったら「試走→本番」へ。以後この地図で先読み＋他車を見て最善手 ---
    if phase==0 and s>LAP_MIN_S and (hdg>LAP_HDG or hdg<0-LAP_HDG):
        LAPLEN=s
        LAPB=s//BL
        if LAPB<1:
            LAPB=1
        phase=1
        Serial.println("course learned: " + LAPB + " zones")
