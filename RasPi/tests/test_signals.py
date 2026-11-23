"""SIGINT, SIGTERM and SIGHUP after RC_setup() stop the motors.

pigpio and VL53L0X are replaced by small fakes, so this runs without a
Raspberry Pi or pigpiod:

    python3 -m unittest discover -s RasPi/tests
"""
import os
import signal
import subprocess
import sys
import unittest

RASPI = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Runs in the child: fake pigpio and VL53L0X, RC_setup(), then wait for a signal.
CHILD = r"""
import signal, sys, time, types

def log(*a):
    print(*a, flush=True)

pigpio = types.ModuleType("pigpio")
pigpio.OUTPUT, pigpio.INPUT = 1, 0

class _Pi:
    connected = True
    def set_mode(self, gpio, mode): pass
    def set_PWM_frequency(self, gpio, f): pass
    def set_PWM_range(self, gpio, r): pass
    def set_PWM_dutycycle(self, gpio, d): pass
    def write(self, gpio, level): log("write", gpio, level)
    def hardware_PWM(self, gpio, f, duty): log("hardware_PWM", gpio, duty)
    def stop(self): log("pi.stop")

pigpio.pi = _Pi
sys.modules["pigpio"] = pigpio

vl = types.ModuleType("VL53L0X")
vl.VL53L0X_HIGH_SPEED_MODE = 4

class _Bus:
    def read_byte_data(self, address, register): raise OSError("no device")   # no rear sensor

class _Sensor:
    def __init__(self, address): pass
    def start_ranging(self, mode): pass
    def stop_ranging(self): pass

vl.i2cbus = _Bus()
vl.VL53L0X = _Sensor
sys.modules["VL53L0X"] = vl

sleep = time.sleep
time.sleep = lambda s: None          # skip the 0.5 s XSHUT waits of setup()
sys.path.insert(0, sys.argv[1])
from RumiCar import *
RC_setup()
RC_drive(FORWARD, 150)
log("ready")
while True:
    sleep(0.1)
"""


class ShutdownOnSignal(unittest.TestCase):

    def run_and_signal(self, signum):
        p = subprocess.Popen([sys.executable, "-c", CHILD, RASPI],
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        try:
            lines = []
            for line in iter(p.stdout.readline, ""):
                lines.append(line.strip())
                if lines[-1] == "ready":
                    break
            if "ready" not in lines:
                self.fail("child did not finish RC_setup(): %s" % p.communicate(timeout=10)[1])
            p.send_signal(signum)
            out, err = p.communicate(timeout=10)
        finally:
            if p.poll() is None:
                p.kill()
                p.wait()
        return p.returncode, out, err

    def check(self, signum):
        code, out, err = self.run_and_signal(signum)
        lines = out.splitlines()
        self.assertIn("pi.stop", lines, "shutdown() did not run on %s (exit %s)" % (signum.name, code))
        stopped = lines[:lines.index("pi.stop")]       # output after RC_setup() and RC_drive()
        for gpio in (18, 13):                     # drive PWM back to 0
            self.assertIn("hardware_PWM %d 0" % gpio, stopped)
        for gpio in (17, 27):                     # steering PWM off
            self.assertIn("write %d 0" % gpio, stopped)
        self.assertEqual(code, 0, err)

    def test_sigint(self):
        self.check(signal.SIGINT)

    def test_sigterm(self):
        self.check(signal.SIGTERM)

    def test_sighup(self):
        self.check(signal.SIGHUP)


if __name__ == "__main__":
    unittest.main()
