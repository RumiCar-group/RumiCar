import sys
sys.path.append('..')

import RumiCar
import time

if __name__ == "__main__":
    while True:
        try:
            RumiCar.rc_drive(RumiCar.FORWARD, 900000)
            RumiCar.rc_steer(RumiCar.RIGHT)
            time.sleep(3)
            RumiCar.rc_steer(RumiCar.LEFT)
            time.sleep(3)
        except KeyboardInterrupt:
            RumiCar.rc_clear()
