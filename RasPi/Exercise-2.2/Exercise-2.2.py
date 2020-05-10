import RumiCar
import time

if __name__ == "__main__":
    while True:
        try:
            RumiCar.rc_drive(RumiCar.FORWARD, 900000)
            time.sleep(3)
            RumiCar.rc_drive(RumiCar.FORWARD, 700000)
            time.sleep(3)
            RumiCar.rc_drive(RumiCar.FORWARD, 500000)
            time.sleep(3)
            RumiCar.rc_drive(RumiCar.FORWARD, 300000)
            time.sleep(3)
        except KeyboardInterrupt:
            RumiCar.rc_clear()
