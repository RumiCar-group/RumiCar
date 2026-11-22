from RumiCar import *

# 安全に止まる: 前方のセンサーの距離で走行を決める
# 実行: python3 ../RumiCar.py Exercise-3.1.py

FAR_MM = 9999           # 範囲外 (-3) を「遠い」として比べるための値。RC_read が返すどの距離 (最大 2000 mm) よりも大きい


def setup():
    RC_setup()


def loop():
    d1 = RC_read(CENTER)
    print(d1)
    if d1 < 0:
        # ステータス (負の数は距離ではない): ステータスの処理はこのブロックにまとめる
        if d1 == -3:
            d1 = FAR_MM         # -3: 範囲外 (上限より手前に測れた物が無い) を「遠い」として比べる
        else:
            RC_drive(BRAKE, 0)  # -1: センサーが応答しない → 止まる
            return
    # ここから先の d1 は距離 [mm] (範囲外は FAR_MM)
    if d1 < 300:            # 30 cm より近くに何かある
        if d1 > 120:
            RC_drive(FORWARD, 255)
        elif d1 < 80:
            RC_drive(REVERSE, 255)
        else:
            RC_drive(BRAKE, 255)
    else:
        RC_drive(FREE, 0)
