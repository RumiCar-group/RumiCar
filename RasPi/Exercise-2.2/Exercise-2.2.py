from RumiCar import *
import time

# 3 段階の速さで前進する (255 = 全開, 150 = 全開の約 6 割の信号)
# 実行: python3 ../RumiCar.py Exercise-2.2.py


def setup():
    RC_setup()


def loop():
    RC_drive(FORWARD, 255)
    time.sleep(0.5)
    RC_drive(FORWARD, 200)
    time.sleep(0.5)
    RC_drive(FORWARD, 150)
    time.sleep(0.5)
