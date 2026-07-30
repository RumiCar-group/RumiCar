# Steady Nose (Python) — ノーマル FF 用  [卓上 Lv3 弱点を補う・Python 版]  by Opus 4.8
# C 版「Steady Nose」の Python 移植。ロジックと定数は完全に同一 (=同じ走り)。
# FF は前輪が「駆動」と「操舵」を兼ねる。曲げながら踏むと前輪が駆動に取られて曲がる力を失い
# 外へ膨らむ(パワーアンダー)。
# ★引き出すロジック(基準と“決定的に違う”点)★
#   「操舵している間はアクセルを抜く」。前輪に“曲げる仕事”を専念させ、まっすぐで踏む。
#   速度を前方距離だけでなく『今ハンドルを切っているか』で決めるのが核。これが FF の肝。

TOP=250
CRUISE=205
SLOW=120
TURN_PWM=120   # 操舵中に抜く先の低PWM
D_OPEN=600
D_MID=440
D_TURN=400
D_SIDE=190
CONF=640       # 信頼区間[mm]: >CONF/範囲外(-3)=「遠い/開放」
OPEN=9999

def setup():
    RC_setup()

def loop():
    L=RC_read(LEFT)
    C=RC_read(CENTER)
    R=RC_read(RIGHT)
    if L<0 or L>CONF:
        L=OPEN
    if C<0 or C>CONF:
        C=OPEN
    if R<0 or R>CONF:
        R=OPEN
    steering=1     # 1=今ハンドルを切っている, 0=まっすぐ
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
        steering=0
    # ★FF の肝★ 操舵中はアクセルを抜き(前輪を操舵に専念)、まっすぐなら前方の空きで加速する。
    if steering==1:
        pwm=TURN_PWM
    elif C>D_OPEN:
        pwm=TOP
    elif C>D_MID:
        pwm=CRUISE
    else:
        pwm=SLOW
    RC_drive(FORWARD,pwm)
