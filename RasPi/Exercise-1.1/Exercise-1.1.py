from RumiCar import *

# 前方のセンサーの距離を表示する
# (0〜200 = mm, -1 = タイムアウト, -3 = 20 cm より先には何もない)
# 実行: python3 ../RumiCar.py Exercise-1.1.py


def setup():
    RC_setup()


def loop():
    print(RC_read(CENTER))
