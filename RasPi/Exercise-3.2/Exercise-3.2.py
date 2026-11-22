from RumiCar import *

# 道なりに走る: 3 つのセンサーで速さと向きを決める
# 実行: python3 ../RumiCar.py Exercise-3.2.py

FAR_MM = 9999           # 範囲外 (-3) を「遠い」として比べるための値。RC_read が返すどの距離 (最大 2000 mm) よりも大きい


def setup():
    RC_setup()


def loop():
    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)
    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2)

    if s0 < 0 or s1 < 0 or s2 < 0:
        # ステータス (負の数は距離ではない): ステータスの処理はこのブロックにまとめる
        if s0 == -1 or s1 == -1 or s2 == -1:
            RC_drive(BRAKE, 0)  # -1: センサーが応答しない → 止まる
            return
        if s0 == -3:            # -3: 範囲外 (上限より手前に測れた物が無い) を「遠い」として比べる
            s0 = FAR_MM
        if s1 == -3:
            s1 = FAR_MM
        if s2 == -3:
            s2 = FAR_MM
    # ここから先の s0〜s2 は距離 [mm] (範囲外は FAR_MM)

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
