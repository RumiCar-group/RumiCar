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

# ラズパイ用カメラ + OpenCVサンプル
# 物体までまでの距離が100mm以下の時写真を撮影、保存し停止する

import sys
sys.path.append('..')

import time
import pigpio
import VL53L0X
import numpy as np
import cv2
from datetime import datetime

# VL53L0Xのピン設定
BIN1 = 18
BIN2 = 13
SHDN1 = 24

pi = pigpio.pi()

# PWM周波数
BIN_FREQUENCY = 960

# 各GPIOの初期化
pi.set_mode(BIN1, pigpio.OUTPUT)
pi.set_mode(BIN2, pigpio.OUTPUT)
pi.set_mode(SHDN1, pigpio.OUTPUT)

cap = cv2.VideoCapture(0, cv2.CAP_V4L2)

if __name__ == "__main__":

    # Set all shutdown pins low to turn off each VL53L0X
    pi.write(SHDN1, 0)

    # Keep all low for 500 ms or so to make sure they reset
    time.sleep(0.50)

    # Create one object per VL53L0X
    sensor1 = VL53L0X.VL53L0X(address=0x2D)

    # set SHDN1 to input in order to use VL53L0X's pull-up resistor
    pi.set_mode(SHDN1, pigpio.INPUT)
    time.sleep(0.50)

    # call to start ranging 
    sensor1.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)

    timing = sensor1.get_timing()
    if (timing < 20000):
        timing = 20000

    pi.hardware_PWM(BIN1, BIN_FREQUENCY, 750000)
    pi.hardware_PWM(BIN2, BIN_FREQUENCY, 0)

    while True:
        ret, frame = cap.read()
        if (not ret):
            print("failed to get image, please enable camera")
            break

        distance = sensor1.get_distance()
        if (distance < 100):
            now = datetime.now()
            dt_string = now.strftime("%Y%m%d_%H%M%S")
            cv2.imwrite(dt_string + '.jpg', frame)
            pi.hardware_PWM(BIN1, BIN_FREQUENCY, 900000)
            pi.hardware_PWM(BIN2, BIN_FREQUENCY, 900000)
            break

    sensor1.stop_ranging()
    pi.set_mode(BIN1, pigpio.INPUT)
    pi.set_mode(BIN2, pigpio.INPUT)
    pi.stop()

    cap.release()
    cv2.destroyAllWindows()