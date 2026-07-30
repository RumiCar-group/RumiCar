# Apex Hunter (Python) — ノーマル FR 用  [卓上 Lv1 基準・Python 版]  by Opus 4.8
# C 版「Apex Hunter」の Python 移植。ロジックと定数は完全に同一 (=まったく同じ走り) で、
# 書き方だけ Python (def setup/loop・global・RC_read) に置き換えた入門サンプル。
# ★学習の出発点★ FR は駆動(後輪)と操舵(前輪)が分かれるので最も素直に曲がる。
# 全車に共通する土台ロジック(3つ):
#   (1) 前方センサー C の空き具合で速度を3段に変える(直線=速い / 中速 / コーナー=遅い)
#   (2) 前が詰まったら左右で「広い方」へ全力で曲げる
#   (3) 側方の壁が近ければ離れる方向へ補正する
# ※Python では RC_read(LEFT/CENTER/RIGHT) で前方3センサーを読む(C の sensor0/1/2 と同じ値)。

TOP=250        # 速度: 直線
CRUISE=215     # 速度: 中速
SLOW=130       # 速度: コーナー
TCAP=195       # 旋回中の上限
D_OPEN=620     # 判定距離[mm]: これより前が開けば TOP
D_MID=405      # 判定距離[mm]: これより前が開けば CRUISE
D_TURN=375     # 判定距離[mm]: これより前が詰まれば曲げる
D_SIDE=180     # 判定距離[mm]: 側方がこれより近ければ離れる
CONF=640       # 信頼区間[mm]: これを超える/範囲外(-3)の測距は「遠い/開放」とみなす
OPEN=9999      # 開放を表す大きな値

def setup():
    RC_setup()

def loop():
    L=RC_read(LEFT)    # 前方3センサー (C の sensor0/1/2 に対応)
    C=RC_read(CENTER)
    R=RC_read(RIGHT)
    if L<0 or L>CONF:  # 信頼区間外/範囲外(-3) = 開放
        L=OPEN
    if C<0 or C>CONF:
        C=OPEN
    if R<0 or R>CONF:
        R=OPEN
    # (2)(3) 操舵: 前が詰まれば広い方へ、側方が近ければ離れる
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
    # (1) 速度: 前方が開けているほど速く
    pwm=SLOW
    if C>D_OPEN:
        pwm=TOP
    elif C>D_MID:
        pwm=CRUISE
    if turning==1 and pwm>TCAP:   # 旋回中は軽く抑え巻き込みを防ぐ
        pwm=TCAP
    RC_drive(FORWARD,pwm)
