import sys
sys.path.append('..')

from RumiCar import *
import time

# 右・左に交互に操舵する
RC_setup()
while True:
    RC_steer(RIGHT)
    time.sleep(0.5)
    RC_steer(LEFT)
    time.sleep(0.5)
