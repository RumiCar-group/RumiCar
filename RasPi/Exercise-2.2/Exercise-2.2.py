import sys
sys.path.append('..')

from RumiCar import *
import time

# 3 段階の速さで前進する (255 = 全開, 150 = 全開の約 6 割の信号)
RC_setup()
while True:
    RC_drive(FORWARD, 255)
    time.sleep(0.5)
    RC_drive(FORWARD, 200)
    time.sleep(0.5)
    RC_drive(FORWARD, 150)
    time.sleep(0.5)
