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
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import time
import pigpio
import VL53L0X

# Setup GPIO for shutdown pins on each VL53L0X
SHDN0 = 23
SHDN1 = 24

pi = pigpio.pi()
pi.set_mode(SHDN0, pigpio.OUTPUT)
pi.set_mode(SHDN1, pigpio.OUTPUT)

# Set all shutdown pins low to turn off each VL53L0X
pi.write(SHDN0, 0)
pi.write(SHDN1, 0)

# Keep all low for 500 ms or so to make sure they reset
time.sleep(0.50)

# Create one object per VL53L0X
sensor0 = VL53L0X.VL53L0X()
sensor1 = VL53L0X.VL53L0X()

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

def read_all_sensors():

    #TODO: ADD Third Sensor

    timing = sensor0.get_timing()
    if (timing < 20000):
        timing = 20000
    print ("Timing %d ms" % (timing/1000))

    for count in range(1,101):
        distance = sensor0.get_distance()
        if (distance > 0):
            print ("sensor %d - %d mm" % (sensor0.my_object_number, distance))
        else:
            print ("%d - Error" % tof.my_object_number)

        distance = sensor1.get_distance()
        if (distance > 0):
            print ("sensor %d - %d mm" % (sensor1.my_object_number, distance))
        else:
            print ("%d - Error" % tof.my_object_number)

        time.sleep(timing/1000000.00)

    sensor0.stop_ranging()
    pi.write(SHDN0, 0)
    sensor1.stop_ranging()
    pi.write(SHDN1, 0)

def read_one_sensor():
  
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
    pi.write(SHDN1, 0)
