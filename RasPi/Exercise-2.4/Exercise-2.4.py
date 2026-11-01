import sys
sys.path.append('..')

from RumiCar import *
import time

# 走行と操舵を組み合わせてジグザグに走る
RC_setup()
while True:
    RC_drive(FORWARD, 255)
    RC_steer(RIGHT)
    time.sleep(0.5)
    RC_steer(LEFT)
    time.sleep(0.5)
