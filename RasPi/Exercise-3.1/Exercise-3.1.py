from RumiCar import *

# 安全に止まる: 前方のセンサーの距離で走行を決める
# 実行: python3 ../RumiCar.py Exercise-3.1.py


def setup():
    RC_setup()


def loop():
    d1 = RC_read(CENTER)
    print(d1)
    if d1 == -3:            # -3: 範囲外 = 遠い (-3 は数としては最小なので、比較の前にどの距離 (最大 2000) よりも大きい値に置き換える)
        d1 = 9999
    if d1 < 300:            # 30 cm より近くに何かある
        if d1 > 120:
            RC_drive(FORWARD, 255)
        elif d1 < 80:
            RC_drive(REVERSE, 255)
        else:
            RC_drive(BRAKE, 255)
    else:
        RC_drive(FREE, 0)
