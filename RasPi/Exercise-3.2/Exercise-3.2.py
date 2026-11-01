import sys
sys.path.append('..')

from RumiCar import *

# 道なりに走る: 3 つのセンサーで速さと向きを決める
RC_setup()
while True:
    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)
    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2)

    if s0 == -3:            # 20 cm より先には何もない = 遠い
        s0 = 999
    if s1 == -3:
        s1 = 999
    if s2 == -3:
        s2 = 999

    if s1 < 100:
        RC_drive(REVERSE, 150)
    elif s1 < 150:
        RC_drive(FORWARD, 150)
    elif s1 < 200:
        RC_drive(FORWARD, 200)
    else:
        RC_drive(FORWARD, 255)

    if s0 > s2:
        RC_steer(LEFT)
    else:
        RC_steer(RIGHT)
