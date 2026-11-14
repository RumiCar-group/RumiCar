from RumiCar import *
import time

# 走行と操舵を組み合わせてジグザグに走る
# 実行: python3 ../RumiCar.py Exercise-2.4.py


def setup():
    RC_setup()


def loop():
    RC_drive(FORWARD, 255)
    RC_steer(RIGHT)
    time.sleep(0.5)
    RC_steer(LEFT)
    time.sleep(0.5)
