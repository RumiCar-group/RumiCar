import pigpio
import time

# 走行用の設定
FREE = 0
REVERSE = 1
FORWARD = 2
BRAKE = 3

FREQUENCY = 960

BIN1 = 18
BIN2 = 13

pi = pigpio.pi()
pi.set_mode(BIN1, pigpio.OUTPUT)
pi.set_mode(BIN2, pigpio.OUTPUT)

def rc_drive(direc, ipwm):
    if direc == FREE:
        pi.hardware_PWM(BIN1, FREQUENCY, 0)
        pi.hardware_PWM(BIN2, FREQUENCY, 0)
    elif direc == REVERSE:
        pi.hardware_PWM(BIN1, FREQUENCY, 0)
        pi.hardware_PWM(BIN2, FREQUENCY, ipwm)
    elif direc == FORWARD:
        pi.hardware_PWM(BIN1, FREQUENCY, ipwm)
        pi.hardware_PWM(BIN2, FREQUENCY, 0)
    elif direc == BRAKE:
        pi.hardware_PWM(BIN1, FREQUENCY, ipwm)
        pi.hardware_PWM(BIN2, FREQUENCY, ipwm)
    else:
        return 0

def rc_clear():
    pi.set_mode(BIN1, pigpio.INPUT)
    pi.set_mode(BIN2, pigpio.INPUT)
    pi.stop()
    