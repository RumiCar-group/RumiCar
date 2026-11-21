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
# Arduino library RumiCar-lib v0.7.0 (https://github.com/RumiCar-group/RumiCar-lib),
# so a program written for one computer module reads the same on the other:
#
#   RC_setup()                 initialize the sensors and the motors (call once)
#   RC_steer(direc, ipwm=255)  LEFT / CENTER / RIGHT, ipwm = strength 0..255
#                                                              -> 1, or 0 for a bad argument
#   RC_drive(direc, ipwm)      FREE / REVERSE / FORWARD / BRAKE, ipwm 0..255
#                                                              -> 1, or 0 for a bad argument
#   RC_read(direc)             LEFT / CENTER / RIGHT / REAR    -> 0..limit = distance [mm]
#                                                                 -1 = timeout (no rear sensor: -1)
#                                                                 -2 = bad argument
#                                                                 -3 = out of range (above the limit,
#                                                                      no target, low signal)
#   RC_setReadLimit(limit_mm)  the limit of RC_read for all directions -> 1
#   RC_setReadLimit(direc, limit_mm)
#                              the limit for LEFT / CENTER / RIGHT / REAR only
#                                                              -> 1, or 0 for a bad direction
#                              limit_mm 1..2000, 0 = no limit (the default: RC_read returns up
#                              to 2000 mm); call it in setup().  Readings above it are -3 (far)
#   RC_params()                params.json next to the program as a dict ({} when absent)
#   RC_state(name)             name of the current state (printed when it changes)
#
# The values given to RC_drive() and RC_steer() are the share of the signal:
# 150 means a signal of 150/255 (about 59 %), on every computer module.  They
# are not compensated, so the same value does not give the same speed or the
# same steering angle on every car.
#
# A program is written as on Arduino, with setup() (once) and loop() (again
# and again), and is started through this file, which calls loop() every
# 50 ms (20 times a second, as RumiCar-Zero2Cam does):
#
#   python3 RumiCar.py program.py
#
# Structure.  The file has two layers:
#   - the API layer (the RC_* functions, the constants, the setup()/loop()
#     runner): checks and normalizes the arguments, applies the limit of
#     RC_read (RC_setReadLimit) and gives the same return values everywhere;
#   - a hardware layer that does the output.  _PigpioHardware drives the
#     RumiCar chassis with pigpio (Raspberry Pi Zero W); _Zero2CamHardware is
#     for RumiCar-Zero2Cam, whose core process owns the hardware: there the
#     RC_* functions only exchange the values of one frame with the core
#     (see _use_zero2cam).  pigpio and VL53L0X are imported by the pigpio
#     layer only, so the file also loads where they are not installed.
#
# Hardware of the pigpio layer: DRV8835 (A = steering on GPIO 17/27 with
# software PWM, B = drive on GPIO 18/13 with hardware PWM at 960 Hz) and three
# VL53L0X on I2C bus 1 with XSHUT on GPIO 23/24/25, plus an optional rear
# VL53L0X without XSHUT (auto-detected).  Needs pigpiod
# (sudo systemctl enable --now pigpiod) and python3-smbus.

import atexit
import importlib.util
import json
import os
import signal
import sys
import time

__version__ = "0.7.0"   # major.minor = the RumiCar-lib API it matches (v0.7.0); the patch is this file's own

__all__ = [
    "LEFT", "CENTER", "RIGHT", "REAR",
    "FREE", "REVERSE", "FORWARD", "BRAKE",
    "RC_setup", "RC_steer", "RC_drive", "RC_read", "RC_setReadLimit", "RC_params", "RC_state",
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

PWM_MAX = 255                   # full signal for RC_drive and RC_steer
LOOP_PERIOD = 0.05              # s between the starts of two loop() calls (20 Hz, as RumiCar-Zero2Cam)

# The largest distance RC_read returns [mm]: the maximum range of the VL53L0X.  Readings above it
# (including the about 8190 the sensor reports when nothing reflects) are -3 (out of range).
READ_MAX_MM = 2000


# =============================================================================================
# API layer
# =============================================================================================

_hw = None                      # the active hardware layer (_PigpioHardware unless _use_zero2cam was called)
_warned = set()                 # warnings already printed (each one once, not on every loop)

# The limit of RC_read for each direction [mm] (index LEFT, CENTER, RIGHT, REAR).  0 is no limit
# (up to READ_MAX_MM), the default.  The laser beam spreads out in a cone, so depending on where
# the sensor is mounted it picks up the floor at a distance; how far a reading is trusted is
# decided by the program with RC_setReadLimit, not by the library.  RC_setup does not reset it.
_read_limit = [0, 0, 0, 0]


def _hardware():
    global _hw
    if _hw is None:
        _hw = _PigpioHardware()
    return _hw


def _warn(key, message):
    """Print a warning about a bad argument once (on stderr)."""
    if key not in _warned:
        _warned.add(key)
        print("RumiCar: " + message, file=sys.stderr, flush=True)


def _is_one_of(direc, values):
    """True when direc is one of the constants (True / False are not accepted)."""
    return direc is not True and direc is not False and direc in values


def _pwm(ipwm, function):
    """0..255 from the value given to RC_drive / RC_steer (out-of-range values
    are clamped; a value that is not a number counts as 0)."""
    try:
        ipwm = int(ipwm)
    except (TypeError, ValueError, OverflowError):
        _warn((function, "pwm", repr(ipwm)),
              "%s(..., %r): the value must be a number 0..255; 0 used" % (function, ipwm))
        return 0
    if ipwm < 0:
        return 0
    if ipwm > PWM_MAX:
        return PWM_MAX
    return ipwm


def _limit_mm(limit_mm):
    """0..READ_MAX_MM from the value given to RC_setReadLimit (out-of-range values
    are clamped; a value that is not a number counts as 0, no limit), as _pwm."""
    try:
        limit_mm = int(limit_mm)
    except (TypeError, ValueError, OverflowError):
        _warn(("RC_setReadLimit", "limit", repr(limit_mm)),
              "RC_setReadLimit(..., %r): the limit must be a number 0..%d; 0 (no limit) used"
              % (limit_mm, READ_MAX_MM))
        return 0
    if limit_mm < 0:
        return 0
    if limit_mm > READ_MAX_MM:
        return READ_MAX_MM
    return limit_mm


def RC_setup():
    """RumiCar のセンサとモータの初期化 (最初に 1 回呼ぶ)"""
    _hardware().setup()


def RC_steer(direc, ipwm=PWM_MAX):
    """操舵の関数: LEFT / CENTER / RIGHT, ipwm = 強さ 0..255
    (省略時 255 = いちばん強い, 0 = CENTER と同じ, CENTER では無視)
    戻り値: 1, 引数が不正なら 0"""
    s = _pwm(ipwm, "RC_steer")
    if not _is_one_of(direc, (LEFT, CENTER, RIGHT)):
        _warn(("RC_steer", repr(direc)), "RC_steer(%r): not LEFT / CENTER / RIGHT; ignored" % (direc,))
        return 0
    direc = int(direc)
    if direc == CENTER:
        s = 0                   # the strength is ignored for CENTER
    _hardware().set_steer(direc, s)
    return 1


def RC_drive(direc, ipwm):
    """走行の関数: FREE / REVERSE / FORWARD / BRAKE, ipwm 0..255 (FREE と BRAKE では無視)
    戻り値: 1, 引数が不正なら 0"""
    s = _pwm(ipwm, "RC_drive")
    if not _is_one_of(direc, (FREE, REVERSE, FORWARD, BRAKE)):
        _warn(("RC_drive", repr(direc)),
              "RC_drive(%r, ...): not FREE / REVERSE / FORWARD / BRAKE; ignored" % (direc,))
        return 0
    _hardware().set_drive(int(direc), s)
    return 1


def RC_read(direc):
    """測距の関数: LEFT / CENTER / RIGHT / REAR
    戻り値: 0..上限 = mm (上限は既定 2000, RC_setReadLimit で設定した場合はその値),
            -1 = タイムアウト (リア非搭載も -1), -2 = 引数エラー,
            -3 = 範囲外 (上限超, 対象物なし, 信号が弱い)。走行では「遠い」として扱う"""
    if not _is_one_of(direc, (LEFT, CENTER, RIGHT, REAR)):
        return -2
    direc = int(direc)
    d = _hardware().read(direc)         # distance [mm], or -1 / -3 from the hardware layer
    if d < 0:
        return d
    if d > READ_MAX_MM:
        return -3                       # above 2000 mm, no target
    limit = _read_limit[direc]
    if limit != 0 and d > limit:
        return -3                       # above the limit set with RC_setReadLimit
    return d


def RC_setReadLimit(*args):
    """RC_read の上限 [mm] を決める関数 (setup() で RC_setup() のあとに呼ぶ)
    RC_setReadLimit(limit_mm)         前・左・右・後ろのすべてに同じ上限
    RC_setReadLimit(direc, limit_mm)  その方向だけ: LEFT / CENTER / RIGHT / REAR
    limit_mm: 1..2000, 0 = 上限なし (既定), 範囲外の値は 0..2000 に収める
    上限を超える値は RC_read が -3 (遠い) を返す。RC_setup() は上限を変えない
    REAR はリア非搭載車でも設定できる (RC_read(REAR) は -1 のまま)
    戻り値: 1, 方向が不正なら 0 (何も変えない)"""
    if len(args) == 1:
        limit = _limit_mm(args[0])
        for i in (LEFT, CENTER, RIGHT, REAR):
            _read_limit[i] = limit
        return 1
    if len(args) == 2:
        direc, limit_mm = args
        if not _is_one_of(direc, (LEFT, CENTER, RIGHT, REAR)):
            _warn(("RC_setReadLimit", repr(direc)),
                  "RC_setReadLimit(%r, ...): not LEFT / CENTER / RIGHT / REAR; ignored" % (direc,))
            return 0
        _read_limit[int(direc)] = _limit_mm(limit_mm)
        return 1
    raise TypeError("RC_setReadLimit() takes 1 or 2 arguments (%d given)" % len(args))


def RC_params():
    """params.json in the program's folder as a dict ({} when there is none)."""
    return _hardware().params()


def RC_state(name):
    """Name of what the program is doing now (letters, digits and _, up to 24).
    Printed when it changes.  Returns 1, or 0 for an invalid name."""
    name = str(name).strip().upper().replace(" ", "_")
    if not name or len(name) > 24 or not name.isascii() or not all(c.isalnum() or c == "_" for c in name):
        _warn(("RC_state", name), "RC_state(%r): use letters, digits and _ (up to 24); ignored" % (name,))
        return 0
    _hardware().set_state(name)
    return 1


def _run(path):
    """Run a program written with setup() and loop(): load the file, call
    setup() once (if there is one), then loop() every LOOP_PERIOD.  A loop()
    that takes longer than the period is followed by the next one at once
    (no catching up).  Runs until the program ends or Ctrl+C."""
    path = os.path.abspath(path)
    if not os.path.isfile(path):
        print("RumiCar: %s: no such file" % path, file=sys.stderr)
        return 2
    sys.path.insert(0, os.path.dirname(path))      # the program may import its own modules
    spec = importlib.util.spec_from_file_location("program", path)
    program = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(program)
    loop = getattr(program, "loop", None)
    if not callable(loop):
        print("RumiCar: %s has no def loop():" % os.path.basename(path), file=sys.stderr)
        return 2
    setup = getattr(program, "setup", None)
    if callable(setup):
        setup()
    next_start = time.monotonic()
    while True:
        loop()
        next_start += LOOP_PERIOD
        wait = next_start - time.monotonic()
        if wait > 0:
            time.sleep(wait)
        else:
            next_start = time.monotonic()


def _main(argv):
    """python3 RumiCar.py program.py [arguments of the program]"""
    if len(argv) < 2:
        print("usage: python3 RumiCar.py program.py   (the program has setup() and loop())", file=sys.stderr)
        print("RumiCar library %s" % __version__, file=sys.stderr)
        return 2
    sys.argv = argv[1:]          # the program sees itself as sys.argv[0] (RC_params reads params.json next to it)
    try:
        return _run(argv[1])
    except KeyboardInterrupt:    # Ctrl+C before RC_setup() (RC_setup() handles it itself afterwards)
        return 0


# =============================================================================================
# Hardware layer for the RumiCar chassis with pigpio (Raspberry Pi Zero W)
# =============================================================================================

class _PigpioHardware:
    # DRV8835: A = steering, B = drive
    AIN1 = 17                   # software PWM (pigpio)
    AIN2 = 27
    BIN1 = 18                   # hardware PWM 0
    BIN2 = 13                   # hardware PWM 1
    STEER_FREQUENCY = 500       # Hz (pigpio software PWM; about the 490 Hz of the steering pins of the Arduino Nano)
    DRIVE_FREQUENCY = 960       # Hz (as the Arduino and the Zero2Cam versions)
    DUTY_FULL = 1000000         # pigpio hardware_PWM duty for 100 %

    # VL53L0X: XSHUT of the three front sensors and their I2C addresses
    SHDN = (23, 24, 25)         # LEFT, CENTER, RIGHT
    ADDRESS = (0x2B, 0x2D, 0x2F)    # LEFT, CENTER, RIGHT
    REAR_ADDRESS = 0x17         # rear (optional), one above the front maximum as in RumiCar-lib
    DEFAULT_ADDRESS = 0x29      # every VL53L0X powers up here
    REG_MODEL_ID = 0xC0         # reads 0xEE on a VL53L0X
    REG_I2C_ADDRESS = 0x8A      # the register VL53L0X_SetDeviceAddress writes

    # RC_drive: start kick to overcome static friction (same values as RumiCar-lib)
    KICK_DUTY = 200             # 0..255
    KICK_MS = 20

    def __init__(self):
        self.pi = None
        self.pigpio = None
        self.vl53l0x = None
        self.sensors = [None, None, None, None]   # LEFT, CENTER, RIGHT, REAR (None when the rear is not fitted)
        self.prev_direc = FREE                    # last drive direction (for the start-kick edge)
        self.param_dict = None
        self.state = "-"

    # --- setup / shutdown ---------------------------------------------------------------------

    def setup(self):
        if self.pi is not None:
            return
        import pigpio
        import VL53L0X
        self.pigpio = pigpio
        self.vl53l0x = VL53L0X
        pi = pigpio.pi()
        if not pi.connected:
            raise RuntimeError("pigpiod is not running: sudo systemctl enable --now pigpiod")
        self.pi = pi
        atexit.register(self.shutdown)
        signal.signal(signal.SIGINT, self._on_sigint)

        for gpio in (self.AIN1, self.AIN2, self.BIN1, self.BIN2) + self.SHDN:
            pi.set_mode(gpio, pigpio.OUTPUT)
        for gpio in (self.AIN1, self.AIN2):
            pi.set_PWM_frequency(gpio, self.STEER_FREQUENCY)
            pi.set_PWM_range(gpio, PWM_MAX)           # duty 0..255 = the RC_steer scale
            pi.set_PWM_dutycycle(gpio, 0)
        pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, 0)
        pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, 0)
        self.prev_direc = FREE

        # Hold the three front sensors in reset, then bring them up one by one and
        # give each its own address.  The rear (if fitted) is set up first, while
        # the fronts are in reset, so that the default address is free afterwards.
        for shdn in self.SHDN:
            pi.write(shdn, 0)
        time.sleep(0.5)
        self.sensors[REAR] = self._init_rear_sensor()
        for i in (LEFT, CENTER, RIGHT):
            pi.set_mode(self.SHDN[i], pigpio.INPUT)  # release XSHUT (the board's pull-up turns the sensor on)
            time.sleep(0.5)
            sensor = VL53L0X.VL53L0X(address=self.ADDRESS[i])
            sensor.start_ranging(VL53L0X.VL53L0X_HIGH_SPEED_MODE)  # 20 ms timing budget, as RumiCar-lib (HIGH_SPEED)
            self.sensors[i] = sensor

    def _probe(self, address):
        """True when a VL53L0X answers at the address (model id register reads 0xEE)."""
        try:
            return self.vl53l0x.i2cbus.read_byte_data(address, self.REG_MODEL_ID) == 0xEE
        except OSError:
            return False

    def _init_rear_sensor(self):
        """Detect and start the optional rear sensor.  Call while the three front
        sensors are held in reset, so that the only VL53L0X that can answer at the
        default address (or at the rear's working address, after a warm restart) is
        the rear sensor.  The rear has no XSHUT line and keeps its address across
        program restarts, so it is moved back to the default address first (the
        VL53L0X library can only initialize a sensor at the default address)."""
        if not self._probe(self.DEFAULT_ADDRESS):
            if not self._probe(self.REAR_ADDRESS):
                return None                                   # not fitted
            self.vl53l0x.i2cbus.write_byte_data(self.REAR_ADDRESS, self.REG_I2C_ADDRESS, self.DEFAULT_ADDRESS)
            time.sleep(0.01)
            if not self._probe(self.DEFAULT_ADDRESS):
                print("RumiCar: rear sensor found at 0x%02X but it could not be reset; ignored" % self.REAR_ADDRESS)
                return None
        sensor = self.vl53l0x.VL53L0X(address=self.REAR_ADDRESS)
        sensor.start_ranging(self.vl53l0x.VL53L0X_HIGH_SPEED_MODE)  # init at 0x29, move to REAR_ADDRESS, start
        return sensor

    def shutdown(self):
        """Stop the motors and the sensors (registered with atexit by setup)."""
        if self.pi is None:
            return
        pi = self.pi
        try:
            pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, 0)   # coast
            pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, 0)
            pi.write(self.AIN1, 0)                            # also ends the software PWM
            pi.write(self.AIN2, 0)
            for i, s in enumerate(self.sensors):
                if s is not None:
                    s.stop_ranging()
                if i < 3:
                    pi.set_mode(self.SHDN[i], self.pigpio.OUTPUT)
                    pi.write(self.SHDN[i], 0)
            pi.set_mode(self.BIN1, self.pigpio.INPUT)
            pi.set_mode(self.BIN2, self.pigpio.INPUT)
        finally:
            pi.stop()
            self.pi = None

    @staticmethod
    def _on_sigint(signum, frame):
        sys.exit(0)        # runs the atexit handler without a traceback

    def _require_setup(self):
        if self.pi is None:
            raise RuntimeError("RumiCar: call RC_setup() first (RC_setup() を先に呼んでください)")

    # --- steering -----------------------------------------------------------------------------

    def set_steer(self, direc, s):
        """As RC_steer of RumiCar-lib: s = 0 (or CENTER) leaves both inputs low
        (neutral); otherwise slow decay, the input of the side held high and the
        other carrying 255 - s (s = 255: the other input low, full steering).
        Written in the order AIN1 -> AIN2."""
        self._require_setup()
        if direc == CENTER or s == 0:
            a1, a2 = 0, 0
        elif direc == RIGHT:
            a1, a2 = PWM_MAX, PWM_MAX - s
        else:   # LEFT
            a1, a2 = PWM_MAX - s, PWM_MAX
        self.pi.set_PWM_dutycycle(self.AIN1, a1)
        self.pi.set_PWM_dutycycle(self.AIN2, a2)

    # --- drive --------------------------------------------------------------------------------

    def _duty(self, ipwm):
        """0..255 -> pigpio hardware_PWM duty (0..1000000), proportional."""
        return int(round(ipwm * self.DUTY_FULL / PWM_MAX))

    def _drive_slow_decay(self, direc, s):
        """Slow decay on the DRV8835 (IN/IN): one input held high, the other
        carries the inverted duty, so the motor alternates drive and brake.
        s = 0 is brake, s = 255 is continuous drive."""
        if direc == FORWARD:
            self.pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, self.DUTY_FULL)
            self.pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, self._duty(PWM_MAX - s))
        else:   # REVERSE
            self.pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, self.DUTY_FULL)
            self.pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, self._duty(PWM_MAX - s))

    def set_drive(self, direc, s):
        self._require_setup()
        if direc == FREE:
            self.pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, 0)
            self.pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, 0)
        elif direc == BRAKE:
            self.pi.hardware_PWM(self.BIN1, self.DRIVE_FREQUENCY, self.DUTY_FULL)   # both inputs high (s ignored)
            self.pi.hardware_PWM(self.BIN2, self.DRIVE_FREQUENCY, self.DUTY_FULL)
        else:   # REVERSE / FORWARD
            # Start kick only on the edge from stop / the other direction.
            if self.prev_direc != direc and s > 0:
                self._drive_slow_decay(direc, self.KICK_DUTY)
                time.sleep(self.KICK_MS / 1000)
            self._drive_slow_decay(direc, s)
        self.prev_direc = direc

    # --- sensors, params, state ---------------------------------------------------------------

    def read(self, direc):
        """The reading in mm, or -1 (timeout, or no rear sensor).  The API layer
        turns readings above READ_MAX_MM or the limit of RC_setReadLimit into -3."""
        self._require_setup()
        sensor = self.sensors[direc]
        if sensor is None:
            return -1
        raw = sensor.get_distance()      # -1 on error / timeout; about 8190 when nothing reflects
        if raw < 0:
            return -1
        return int(raw)

    def params(self):
        if self.param_dict is None:
            path = os.path.join(os.path.dirname(os.path.abspath(sys.argv[0])), "params.json")
            if os.path.isfile(path):
                with open(path, encoding="utf-8") as f:
                    self.param_dict = json.load(f)
            else:
                self.param_dict = {}
        return self.param_dict

    def set_state(self, name):
        if name != self.state:
            print("state %s -> %s" % (self.state, name))
            self.state = name


# =============================================================================================
# Hardware layer for RumiCar-Zero2Cam
# =============================================================================================

class _Zero2CamHardware:
    """On RumiCar-Zero2Cam the core process owns the motors and the sensors and
    runs the user's program (drive.py) through its runner, one frame at a time
    (20 Hz).  Here the RC_* functions do no output: before each loop() the
    runner puts the frame's sensor values in `tof`; after it, the runner sends
    `steer`, `steer_pwm`, `drive_dir`, `drive_pwm` and `state` to the core.  The
    values stay until the program changes them, as in an Arduino sketch.

    Values (all already checked by the API layer, except tof):
      tof        [LEFT, CENTER, RIGHT] as the core sends them (distance [mm], -1, -3); RC_read
                 turns distances above READ_MAX_MM or the limit of RC_setReadLimit into -3
      steer      LEFT / CENTER / RIGHT
      steer_pwm  0..255; 0 for CENTER and for RC_steer(LEFT / RIGHT, 0) (neutral)
      drive_dir  FREE / REVERSE / FORWARD / BRAKE
      drive_pwm  0..255 as given (the core ignores it for FREE and BRAKE)
      state      name given to RC_state ("-" until the first call)
    The Zero2Cam car has no rear sensor: RC_read(REAR) returns -1."""

    def __init__(self, param_dict):
        self.param_dict = param_dict
        self.tof = [-1, -1, -1]
        self.steer = CENTER
        self.steer_pwm = 0
        self.drive_dir = FREE
        self.drive_pwm = 0
        self.state = "-"

    def setup(self):
        pass                    # the core has initialized the hardware

    def set_steer(self, direc, s):
        self.steer = direc
        self.steer_pwm = s

    def set_drive(self, direc, s):
        self.drive_dir = direc
        self.drive_pwm = s

    def read(self, direc):
        if direc == REAR:
            return -1
        return self.tof[direc]

    def params(self):
        return self.param_dict

    def set_state(self, name):
        self.state = name


def _use_zero2cam(param_dict):
    """For the runner of RumiCar-Zero2Cam (not part of the API): use the
    Zero2Cam hardware layer, with the pack's params.json as `param_dict`, and
    return it (the runner exchanges the frame's values through it).  Call it
    before the user's program is loaded."""
    global _hw
    _hw = _Zero2CamHardware(param_dict)
    return _hw


if __name__ == "__main__":
    # Run through the module named RumiCar, so that the program's
    # `from RumiCar import *` uses the same library (not a second copy).
    import RumiCar
    sys.exit(RumiCar._main(sys.argv))
