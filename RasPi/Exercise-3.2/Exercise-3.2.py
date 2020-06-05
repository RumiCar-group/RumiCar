#!/usr/bin/python

# MIT License
# 
# Copyright (c) 2017 John Bryan Moore
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITYz, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
sys.path.append('..')

import time
import pigpio
import VL53L0X


# 操舵用の設定
LEFT = 0
CENTER = 1
RIGHT = 2

# 走行用の設定
FREE = 0
REVERSE = 1
FORWARD = 2
BRAKE = 3

# DRV8835のピン設定
AIN1 = 17
AIN2 = 27
BIN1 = 18
BIN2 = 13

# PWM周波数
AIN_FREQUENCY = 490
BIN_FREQUENCY = 960

# VL53L0Xのピン設定
SHDN0 = 23
SHDN1 = 24
SHDN2 = 25

pi = pigpio.pi()

# 各GPIOの初期化
pi.set_mode(AIN1, pigpio.OUTPUT)
pi.set_mode(AIN2, pigpio.OUTPUT)
pi.set_mode(BIN1, pigpio.OUTPUT)
pi.set_mode(BIN2, pigpio.OUTPUT)
pi.set_mode(SHDN0, pigpio.OUTPUT)
pi.set_mode(SHDN1, pigpio.OUTPUT)
pi.set_mode(SHDN2, pigpio.OUTPUT)

pi.set_PWM_frequency(AIN1, AIN_FREQUENCY)
pi.set_PWM_frequency(AIN2, AIN_FREQUENCY)

def rc_drive(direc, ipwm):
    if direc == FREE:
        pi.hardware_PWM(BIN1, BIN_FREQUENCY, 0)
        pi.hardware_PWM(BIN2, BIN_FREQUENCY, 0)
    elif direc == REVERSE:
        pi.hardware_PWM(BIN1, BIN_FREQUENCY, 0)
        pi.hardware_PWM(BIN2, BIN_FREQUENCY, ipwm)
    elif direc == FORWARD:
        pi.hardware_PWM(BIN1, BIN_FREQUENCY, ipwm)
        pi.hardware_PWM(BIN2, BIN_FREQUENCY, 0)
    elif direc == BRAKE:
        pi.hardware_PWM(BIN1, BIN_FREQUENCY, ipwm)
        pi.hardware_PWM(BIN2, BIN_FREQUENCY, ipwm)
    else:
        return 0

def rc_steer(direc):
    if direc == RIGHT:
        pi.set_PWM_dutycycle(AIN1, 255) # PWM off
        pi.set_PWM_dutycycle(AIN2, 0)
    elif direc == LEFT:
        pi.set_PWM_dutycycle(AIN1, 0)
        pi.set_PWM_dutycycle(AIN2, 255)
    elif direc == CENTER:
        pi.set_PWM_dutycycle(AIN1, 0)
        pi.set_PWM_dutycycle(AIN2, 0)
    else:
        return 0

if __name__ == "__main__":

    # Set all shutdown pins low to turn off each VL53L0X
    pi.write(SHDN0, 0)
    pi.write(SHDN1, 0)
    pi.write(SHDN2, 0)

    # Keep all low for 500 ms or so to make sure they reset
    time.sleep(0.50)

    # Create one object per VL53L0X
    sensor0 = VL53L0X.VL53L0X(address=0x2B)
    sensor1 = VL53L0X.VL53L0X(address=0x2D)
    sensor2 = VL53L0X.VL53L0X(address=0x2F)

    # set SHDN0 to input in order to use VL53L0X's pull-up resistor
    pi.set_mode(SHDN0, pigpio.INPUT)
    time.sleep(0.50)

    # call to start ranging 
    sensor0.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    # set SHDN1 to input in order to use VL53L0X's pull-up resistor
    pi.set_mode(SHDN1, pigpio.INPUT)
    time.sleep(0.50)

    # call to start ranging 
    sensor1.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    # set SHDN2 to input in order to use VL53L0X's pull-up resistor
    pi.set_mode(SHDN2, pigpio.INPUT)
    time.sleep(0.50)

    # call to start ranging 
    sensor2.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)


    timing = sensor1.get_timing()
    if (timing < 20000):
        timing = 20000

    while True:
        try:
            s0 = sensor0.get_distance()
            s1 = sensor1.get_distance()
            s2 = sensor2.get_distance()

            if s1 < 100:
                rc_drive(REVERSE, 700000);
            elif s1 < 150:
                rc_drive(FORWARD, 500000);
            elif s1 < 250:
                rc_drive(FORWARD, 800000);
            else:
                rc_drive(FORWARD, 900000);

            if s0 > s2:
                rc_steer(LEFT);
            else:
                rc_steer(RIGHT);
            
            time.sleep(timing/1000000.00)
        except KeyboardInterrupt:
            sensor0.stop_ranging()
            pi.write(SHDN0, 0)
            sensor1.stop_ranging()
            pi.write(SHDN1, 0)
            sensor2.stop_ranging()
            pi.write(SHDN2, 0)
            pi.set_mode(BIN1, pigpio.INPUT)
            pi.set_mode(BIN2, pigpio.INPUT)
            pi.stop()
