from RumiCar import *

# なめらかに走る車: Exercise-3.2 の「左右どちらかに必ず切る」の代わりに、
# 左右の差が小さければまっすぐ (CENTER)、大きければ広い側へ切り、
# ハンドルの強さを走りながら調整する (効かなければ 1.3 倍、行き過ぎたら半分)
# 実行: python3 ../RumiCar.py Exercise-3.3.py

ipower = 128            # ハンドルの強さ 0〜255 (半分から始める)
last_turn = CENTER      # 前回切った側
last_gap = 0            # 前回の左右の差 [mm]


def setup():
    RC_setup()
    RC_setReadLimit(200)    # RC_read が信じる距離の上限 [mm]。これより先は -3 (遠い)


def loop():
    global ipower, last_turn, last_gap

    iband = 30          # 左右の差がこれ以下ならまっすぐ [mm]
    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)
    if s0 == -3:        # -3: 上限より先 = 開けている (-3 は数としては最小なので、先に置き換える)
        s0 = 250
    if s1 == -3:
        s1 = 250
    if s2 == -3:
        s2 = 250

    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2, " Power:", ipower)

    if s0 == -1 or s1 == -1 or s2 == -1:
        return          # -1: センサーが応答しない。この回は何もせず、前の動きを続ける

    if s1 < 100:        # しきい値は setup() の上限以下にする
        RC_drive(REVERSE, 150)
    elif s1 < 150:
        RC_drive(FORWARD, 150)
    elif s1 < 200:
        RC_drive(FORWARD, 200)
    else:
        RC_drive(FORWARD, 255)

    gap = abs(s0 - s2)
    if gap <= iband:
        RC_steer(CENTER)
    else:
        if s0 > s2:
            side = LEFT
        else:
            side = RIGHT
        if last_turn != CENTER and side != last_turn:
            ipower = ipower // 2            # 真ん中を越えた: 強すぎるので半分にする
        elif side == last_turn and gap >= last_gap:
            ipower = ipower * 13 // 10      # 真ん中に近づいていない: 弱いので 1.3 倍にする
        ipower = max(40, min(ipower, 255))  # 1.3 倍でまた増やせるよう 40 以上に保つ
        RC_steer(side, ipower)
        last_turn = side
    last_gap = gap
