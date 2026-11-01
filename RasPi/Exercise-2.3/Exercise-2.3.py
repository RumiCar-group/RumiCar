import sys
sys.path.append('..')

from RumiCar import *
import time

# 前進と後退を繰り返す
RC_setup()
while True:
    RC_drive(FORWARD, 255)
    time.sleep(0.5)
    RC_drive(REVERSE, 255)
    time.sleep(0.5)
