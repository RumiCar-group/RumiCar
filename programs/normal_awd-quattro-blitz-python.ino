# Quattro Blitz (Python) — ノーマル 4WD 用  [卓上 Lv2 強みを使う・Python 版]  by Opus 4.8
# C 版「Quattro Blitz」の Python 移植。ロジックと定数は完全に同一 (=同じ走り)。
# ★引き出すロジック★ 4WD だけができる「コーナー出口で“誰より早く”フル加速」。
#   前方距離 C の前回との差 dC を取り、前が開き出した瞬間(dC>0=立ち上がり)に全開へ。
# 基準(FR)との違い: 速度3段は同じ。そこに「立ち上がり検知 → 即全開」を足しただけ。
# ※Python で前回値を覚えるには module 変数 (prevC/started) を global で書き換える。

TOP=235
CRUISE=215
SLOW=140
TCAP=208
D_OPEN=645
D_MID=410
D_TURN=386
D_SIDE=178
CONF=640       # 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」
OPEN=9999
EXIT=18        # 前方が1ループでこの[mm]以上開いたら=コーナー脱出 → 立ち上がり全開
prevC=0        # 前回の前方距離 (差分=接近/開きの検知に使う)
started=0

def setup():
    global prevC, started
    RC_setup()
    prevC=0
    started=0

def loop():
    global prevC, started
    L=RC_read(LEFT)
    C=RC_read(CENTER)
    R=RC_read(RIGHT)
    if L<0 or L>CONF:
        L=OPEN
    if C<0 or C>CONF:
        C=OPEN
    if R<0 or R>CONF:
        R=OPEN
    dC=0
    if started==1:
        dC=C-prevC     # dC>0 = 前方が開いてくる = コーナーの立ち上がり
    prevC=C
    started=1
    turning=1
    if C<D_TURN:
        if L>R:
            RC_steer(LEFT)
        else:
            RC_steer(RIGHT)
    elif R<D_SIDE:
        RC_steer(LEFT)
    elif L<D_SIDE:
        RC_steer(RIGHT)
    else:
        RC_steer(CENTER)
        turning=0
    pwm=SLOW
    if C>D_OPEN:
        pwm=TOP
    elif C>D_MID:
        pwm=CRUISE
    if turning==1 and pwm>TCAP:
        pwm=TCAP
    # ★立ち上がり全開★ コーナーを抜け前方が開き出したら、トラクションを信じて即フルパワー
    if dC>EXIT and C>D_MID:
        pwm=TOP
    RC_drive(FORWARD,pwm)
