from RumiCar import *
import time

# 前進と後退を繰り返す
# 実行: python3 ../RumiCar.py Exercise-2.3.py


def setup():
    RC_setup()


def loop():
    RC_drive(FORWARD, 255)
    time.sleep(0.5)
    RC_drive(REVERSE, 255)
    time.sleep(0.5)
