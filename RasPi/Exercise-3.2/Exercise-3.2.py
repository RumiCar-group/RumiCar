from RumiCar import *

# 道なりに走る: 3 つのセンサーで速さと向きを決める
# 実行: python3 ../RumiCar.py Exercise-3.2.py


def setup():
    RC_setup()


def loop():
    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)
    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2)

    if s0 == -3:            # -3: 範囲外 = 遠い (-3 は数としては最小なので、比較の前にどの距離 (最大 2000) よりも大きい値に置き換える)
        s0 = 9999
    if s1 == -3:
        s1 = 9999
    if s2 == -3:
        s2 = 9999

    if s1 < 100:
        RC_drive(REVERSE, 150)
    elif s1 < 150:
        RC_drive(FORWARD, 150)
    elif s1 < 250:
        RC_drive(FORWARD, 200)
    else:
        RC_drive(FORWARD, 255)

    if s0 > s2:
        RC_steer(LEFT)
    else:
        RC_steer(RIGHT)
