import sys
sys.path.append('..')

from RumiCar import *

# 前方のセンサーの距離を表示する
# (0〜200 = mm, -1 = タイムアウト, -3 = 20 cm より先には何もない)
RC_setup()
while True:
    print(RC_read(CENTER))
