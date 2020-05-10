import RumiCar
import time

if __name__ == "__main__":
    while True:
        try:
            RumiCar.rc_drive(RumiCar.FORWARD, 900000)
            time.sleep(3)
            RumiCar.rc_drive(RumiCar.REVERSE, 900000)
            time.sleep(3)
        except KeyboardInterrupt:
            RumiCar.rc_clear()
