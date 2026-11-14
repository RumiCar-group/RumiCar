from RumiCar import *
import time

# ハンドルの強さを変えてみよう: 強さ 255 (いちばん強い) と 128 (半分) で右・左に切る
# 実行: python3 ../RumiCar.py Exercise-2.5.py


def setup():
    RC_setup()


def loop():
    RC_steer(RIGHT, 255)
    time.sleep(0.5)
    RC_steer(LEFT, 255)
    time.sleep(0.5)
    RC_steer(RIGHT, 128)
    time.sleep(0.5)
    RC_steer(LEFT, 128)
    time.sleep(0.5)
