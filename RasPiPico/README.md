# Raspberry Pi Pico
The build is using [Arduino-Pico](https://arduino-pico.readthedocs.io/en/latest/).

As name suggests, it allows executing Arduino code on Pico!

## Setup PIO
https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html

## Build
```
pio run
```

## Build, Upload and Run
1. Press BOOTSEL
2. Connect USB
3. Unpress BOOTSEL
4. Run:
```
pio run -t upload
```

## Serial Output
```
pio device monitor
```

## Examples
Please, follow `../ArduinoAndESP32/`
