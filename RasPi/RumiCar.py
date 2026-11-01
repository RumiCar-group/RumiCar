#!/usr/bin/python3
# RumiCar library for Raspberry Pi (Python)
#
# MIT License
# Copyright (c) 2026 RumiCar-group
#
# The VL53L0X bring-up (XSHUT sequence, VL53L0X.py and bin/vl53l0x_python.so)
# follows the example of VL53L0X_rasp_python:
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
#
# The API (names, constants, values and return values) is the same as the
# Arduino library RumiCar-lib (https://github.com/RumiCar-group/RumiCar-lib),
# so a program written for one computer module reads the same on the other:
#
#   RC_setup()                 initialize the sensors and the motors (call once)
#   RC_steer(direc)            LEFT / CENTER / RIGHT           -> 1, or 0 for a bad argument
#   RC_drive(direc, ipwm)      FREE / REVERSE / FORWARD / BRAKE, ipwm 0..255
#                                                              -> 1, or 0 for a bad argument
#   RC_read(direc)             LEFT / CENTER / RIGHT / REAR    -> 0..200 = distance [mm]
#                                                                 -1 = timeout (no rear sensor: -1)
#                                                                 -2 = bad argument
#                                                                 -3 = out of range (> 200 mm)
#   RC_params()                params.json next to the program as a dict ({} when absent)
#   RC_state(name)             name of the current state (printed when it changes)
#
# The value given to RC_drive() is the PWM duty: 150 means a signal of 150/255
# (about 59 %) to the drive motor, on every computer module.  The drive uses
# slow decay with a start kick, as RumiCar-lib does (see RC_drive).
#
# Hardware: DRV8835 (A = steering on GPIO 17/27, B = drive on GPIO 18/13 with
# hardware PWM at 960 Hz) and three VL53L0X on I2C bus 1 with XSHUT on GPIO
# 23/24/25, plus an optional rear VL53L0X without XSHUT (auto-detected).
# Needs pigpiod (sudo systemctl enable --now pigpiod) and python3-smbus.

import atexit
import json
import os
import signal
import sys
import time

import pigpio
import VL53L0X

__all__ = [
    "LEFT", "CENTER", "RIGHT", "REAR",
    "FREE", "REVERSE", "FORWARD", "BRAKE",
    "RC_setup", "RC_steer", "RC_drive", "RC_read", "RC_params", "RC_state",
]

# 操舵用の設定 (RC_steer) と測距の方向 (RC_read)
LEFT = 0
CENTER = 1
RIGHT = 2
REAR = 3       # RC_read only (optional rear sensor)

# 走行用の設定 (RC_drive)
FREE = 0
REVERSE = 1
FORWARD = 2
BRAKE = 3

# --- hardware (not part of the API) ---------------------------------------------------------
# DRV8835: A = steering, B = drive
_AIN1 = 17
_AIN2 = 27
_BIN1 = 18     # hardware PWM 0
_BIN2 = 13     # hardware PWM 1
_DRIVE_FREQUENCY = 960          # Hz (as the Arduino and the Zero2Cam versions)
_DUTY_FULL = 1000000            # pigpio hardware_PWM duty for 100 %

# VL53L0X: XSHUT of the three front sensors and their I2C addresses
_SHDN = (23, 24, 25)            # LEFT, CENTER, RIGHT
_ADDRESS = (0x2B, 0x2D, 0x2F)   # LEFT, CENTER, RIGHT
_REAR_ADDRESS = 0x17            # rear (optional), one above the front maximum as in RumiCar-lib
_DEFAULT_ADDRESS = 0x29         # every VL53L0X powers up here
_REG_MODEL_ID = 0xC0            # reads 0xEE on a VL53L0X
_REG_I2C_ADDRESS = 0x8A         # the register VL53L0X_SetDeviceAddress writes
_RANGING_MODE = VL53L0X.VL53L0X_HIGH_SPEED_MODE   # 20 ms timing budget, as RumiCar-lib (HIGH_SPEED)

_RANGE_MAX_MM = 200             # readings above this are not trusted (the beam reaches the floor)

# RC_drive: start kick to overcome static friction (same values as RumiCar-lib)
_KICK_DUTY = 200                # 0..255
_KICK_MS = 20

_pi = None
_sensors = [None, None, None, None]   # LEFT, CENTER, RIGHT, REAR (None when the rear is not fitted)
_prev_direc = FREE                    # last drive direction (for the start-kick edge)
_params = None
_state = "-"


def _duty(ipwm):
    """0..255 -> pigpio hardware_PWM duty (0..1000000), proportional."""
    return int(round(ipwm * _DUTY_FULL / 255))


def _probe(address):
    """True when a VL53L0X answers at the address (model id register reads 0xEE)."""
    try:
        return VL53L0X.i2cbus.read_byte_data(address, _REG_MODEL_ID) == 0xEE
    except OSError:
        return False


def _init_rear_sensor():
    """Detect and start the optional rear sensor.  Call while the three front
    sensors are held in reset, so that the only VL53L0X that can answer at the
    default address (or at the rear's working address, after a warm restart) is
    the rear sensor.  The rear has no XSHUT line and keeps its address across
    program restarts, so it is moved back to the default address first (the
    VL53L0X library can only initialize a sensor at the default address)."""
    if not _probe(_DEFAULT_ADDRESS):
        if not _probe(_REAR_ADDRESS):
            return None                                       # not fitted
        VL53L0X.i2cbus.write_byte_data(_REAR_ADDRESS, _REG_I2C_ADDRESS, _DEFAULT_ADDRESS)
        time.sleep(0.01)
        if not _probe(_DEFAULT_ADDRESS):
            print("RumiCar: rear sensor found at 0x%02X but it could not be reset; ignored" % _REAR_ADDRESS)
            return None
    sensor = VL53L0X.VL53L0X(address=_REAR_ADDRESS)
    sensor.start_ranging(_RANGING_MODE)                       # init at 0x29, move to _REAR_ADDRESS, start
    return sensor


def _shutdown():
    """Stop the motors and the sensors (registered with atexit by RC_setup)."""
    global _pi
    if _pi is None:
        return
    try:
        _pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, 0)          # coast
        _pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, 0)
        _pi.write(_AIN1, 0)
        _pi.write(_AIN2, 0)
        for i, s in enumerate(_sensors):
            if s is not None:
                s.stop_ranging()
            if i < 3:
                _pi.set_mode(_SHDN[i], pigpio.OUTPUT)
                _pi.write(_SHDN[i], 0)
        _pi.set_mode(_BIN1, pigpio.INPUT)
        _pi.set_mode(_BIN2, pigpio.INPUT)
    finally:
        _pi.stop()
        _pi = None


def _on_sigint(signum, frame):
    sys.exit(0)        # runs the atexit handler without a traceback


def RC_setup():
    """RumiCar のセンサとモータの初期化"""
    global _pi, _prev_direc
    if _pi is not None:
        return
    pi = pigpio.pi()
    if not pi.connected:
        raise RuntimeError("pigpiod is not running: sudo systemctl enable --now pigpiod")
    _pi = pi
    atexit.register(_shutdown)
    signal.signal(signal.SIGINT, _on_sigint)

    for gpio in (_AIN1, _AIN2, _BIN1, _BIN2) + _SHDN:
        pi.set_mode(gpio, pigpio.OUTPUT)
    pi.write(_AIN1, 0)
    pi.write(_AIN2, 0)
    pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, 0)
    pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, 0)
    _prev_direc = FREE

    # Hold the three front sensors in reset, then bring them up one by one and
    # give each its own address.  The rear (if fitted) is set up first, while
    # the fronts are in reset, so that the default address is free afterwards.
    for shdn in _SHDN:
        pi.write(shdn, 0)
    time.sleep(0.5)
    _sensors[REAR] = _init_rear_sensor()
    for i in (LEFT, CENTER, RIGHT):
        pi.set_mode(_SHDN[i], pigpio.INPUT)       # release XSHUT (the board's pull-up turns the sensor on)
        time.sleep(0.5)
        sensor = VL53L0X.VL53L0X(address=_ADDRESS[i])
        sensor.start_ranging(_RANGING_MODE)       # init at 0x29, move to its address, start continuous ranging
        _sensors[i] = sensor


def RC_steer(direc):
    """操舵の関数: LEFT / CENTER / RIGHT"""
    if direc == RIGHT:
        _pi.write(_AIN1, 1)
        _pi.write(_AIN2, 0)
    elif direc == LEFT:
        _pi.write(_AIN1, 0)
        _pi.write(_AIN2, 1)
    elif direc == CENTER:
        _pi.write(_AIN1, 0)
        _pi.write(_AIN2, 0)
    else:
        return 0
    return 1


def _drive_slow_decay(direc, s):
    """Slow decay on the DRV8835 (IN/IN): one input held high, the other
    carries the inverted duty, so the motor alternates drive and brake.
    s = 0 is brake, s = 255 is continuous drive."""
    if direc == FORWARD:
        _pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, _DUTY_FULL)
        _pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, _duty(255 - s))
    else:   # REVERSE
        _pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, _DUTY_FULL)
        _pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, _duty(255 - s))


def RC_drive(direc, ipwm):
    """走行の関数: FREE / REVERSE / FORWARD / BRAKE, ipwm 0..255"""
    global _prev_direc
    ipwm = int(ipwm)
    if ipwm < 0:
        ipwm = 0
    if ipwm > 255:
        ipwm = 255

    if direc == FREE:
        _pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, 0)
        _pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, 0)
    elif direc == REVERSE or direc == FORWARD:
        # Start kick only on the edge from stop / the other direction.
        if _prev_direc != direc and ipwm > 0:
            _drive_slow_decay(direc, _KICK_DUTY)
            time.sleep(_KICK_MS / 1000)
        _drive_slow_decay(direc, ipwm)
    elif direc == BRAKE:
        _pi.hardware_PWM(_BIN1, _DRIVE_FREQUENCY, _DUTY_FULL)    # both inputs high (ipwm ignored)
        _pi.hardware_PWM(_BIN2, _DRIVE_FREQUENCY, _DUTY_FULL)
    else:
        return 0

    _prev_direc = direc
    return 1


def RC_read(direc):
    """測距の関数: LEFT / CENTER / RIGHT / REAR
    戻り値: 0..200 = mm, -1 = タイムアウト (リア非搭載も -1), -2 = 引数エラー, -3 = 範囲外 (200 mm 超)"""
    if direc not in (LEFT, CENTER, RIGHT, REAR):
        return -2
    sensor = _sensors[direc]
    if sensor is None:
        return -1
    raw = sensor.get_distance()      # -1 on error / timeout; about 8190 when nothing reflects
    if raw < 0:
        return -1
    if raw > _RANGE_MAX_MM:
        return -3
    return int(raw)


def RC_params():
    """params.json in the program's folder as a dict ({} when there is none)."""
    global _params
    if _params is None:
        path = os.path.join(os.path.dirname(os.path.abspath(sys.argv[0])), "params.json")
        if os.path.isfile(path):
            with open(path, encoding="utf-8") as f:
                _params = json.load(f)
        else:
            _params = {}
    return _params


def RC_state(name):
    """Name of what the program is doing now (letters, digits and _, up to 24).
    Printed when it changes.  Returns 1, or 0 for an invalid name."""
    global _state
    name = str(name).strip().upper().replace(" ", "_")
    if not name or len(name) > 24 or not name.isascii() or not all(c.isalnum() or c == "_" for c in name):
        return 0
    if name != _state:
        print("state %s -> %s" % (_state, name))
        _state = name
    return 1
