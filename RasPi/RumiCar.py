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

def read_all_sensors():

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

    # Set shutdown pin high for the first VL53L0X then 
    # call to start ranging 
    pi.write(SHDN0, 1)
    time.sleep(0.50)
    sensor0.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    # Set shutdown pin high for the second VL53L0X then 
    # call to start ranging 
    pi.write(SHDN1, 1)
    time.sleep(0.50)
    sensor1.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    # Set shutdown pin high for the second VL53L0X then 
    # call to start ranging 
    pi.write(SHDN2, 1)
    time.sleep(0.50)
    sensor2.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    timing = sensor0.get_timing()
    if (timing < 20000):
      timing = 20000
    print ("Timing %d ms" % (timing/1000))

    for count in range(1,101):
      distance = sensor0.get_distance()
      if (distance > 0):
          print ("sensor %d - %d mm" % (sensor0.my_object_number, distance))
      else:
          print ("%d - Error" % sensor0.my_object_number)

    distance = sensor1.get_distance()
    if (distance > 0):
        print ("sensor %d - %d mm" % (sensor1.my_object_number, distance))
    else:
        print ("%d - Error" % sensor1.my_object_number)

    distance = sensor2.get_distance()
    if (distance > 0):
        print ("sensor %d - %d mm" % (sensor2.my_object_number, distance))
    else:
        print ("%d - Error" % sensor2.my_object_number)

    time.sleep(timing/1000000.00)

    sensor0.stop_ranging()
    pi.write(SHDN0, 0)
    sensor1.stop_ranging()
    pi.write(SHDN1, 0)
    sensor2.stop_ranging()
    pi.write(SHDN2, 0)

def read_one_sensor():

    # Set all shutdown pins low to turn off each VL53L0X
    pi.write(SHDN1, 0)

    # Keep all low for 500 ms or so to make sure they reset
    time.sleep(0.50)

    # Create one object per VL53L0X
    sensor1 = VL53L0X.VL53L0X(address=0x2D)

    # Set shutdown pin high for the second VL53L0X then 
    # call to start ranging 
    pi.write(SHDN1, 1)
    time.sleep(0.50)
    sensor1.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    timing = sensor1.get_timing()
    if (timing < 20000):
        timing = 20000
    print ("Timing %d ms" % (timing/1000))

    for count in range(100):
        distance = sensor1.get_distance()
        if (distance > 0):
            print ("sensor %d - %d mm" % (sensor1.my_object_number, distance))
        else:
            print ("%d - Error" % sensor1.my_object_number)

    sensor1.stop_ranging()
    pi.write(SHDN0, 0)

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

def rc_clear():
    pi.set_mode(BIN1, pigpio.INPUT)
    pi.set_mode(BIN2, pigpio.INPUT)
    pi.stop()
