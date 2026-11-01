import sys
sys.path.append('..')

from RumiCar import *

# 3 つのセンサーの距離を 1 行に表示する
RC_setup()
while True:
    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)
    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2)
