from RumiCar import *

# 前方のセンサーの距離を表示する
# (0〜2000 = mm, -1 = タイムアウト, -3 = 範囲外: 2000 mm より先・何も無い・反射が弱い)
# 実行: python3 ../RumiCar.py Exercise-1.1.py


def setup():
    RC_setup()


def loop():
    print(RC_read(CENTER))
