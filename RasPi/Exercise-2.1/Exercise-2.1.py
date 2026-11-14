from RumiCar import *
import time

# 右・左に交互に操舵する
# 実行: python3 ../RumiCar.py Exercise-2.1.py


def setup():
    RC_setup()


def loop():
    RC_steer(RIGHT)
    time.sleep(0.5)
    RC_steer(LEFT)
    time.sleep(0.5)
