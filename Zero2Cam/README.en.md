# Raspberry Pi Zero 2 W + camera version (RumiCar Zero2Cam)

**English** | [日本語](README.md)

The software for the RumiCar that carries a Raspberry Pi Zero 2 W and a Camera Module 2 lives in its own repository, **[RumiCar-group/RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam)**. This folder is only a signpost; it contains no programs.

| | |
|---|---|
| Programs and user guide | [RumiCar-Zero2Cam README](https://github.com/RumiCar-group/RumiCar-Zero2Cam/blob/main/README.en.md) (English) |
| Downloads (SD-card image, PC app) | [Releases](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) |
| Specification | [docs/RumiCar-Zero2Cam 仕様書.docx](https://github.com/RumiCar-group/RumiCar-Zero2Cam/tree/main/docs) (Japanese) |

## What it is

- An autonomous car that starts driving as soon as it is switched on (the three front ToF sensors keep it away from walls and obstacles — the same way the Arduino version works).
- The car is a Wi-Fi access point. Connect a PC to it and the driving-manager app (Windows) shows the status and the onboard camera image, switches between auto / manual / record-only, triggers the emergency stop, and drives the car with the arrow keys.
- Every received image and status frame is recorded on the PC. Open the recording folder in the Claude desktop app to improve the decision data, then push it to the car from the driving manager — without stopping the car.
- Nothing to build: write the SD-card image and go.

## What you need

| Item | Notes |
|---|---|
| A RumiCar vehicle with a Raspberry Pi Zero 2 W computer module | The wiring is **the same as the Raspberry Pi Zero W CM** (see "結線図(Raspberry Pi Zero W).png" in [開発用資料](../%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99) and the pin table in [RasPi/README.md](../RasPi/README.md)): steering GPIO 17/27, drive GPIO 18/13 (hardware PWM), ToF XSHUT GPIO 23/24/25, I2C |
| Camera Module 2 | On the Zero 2 W camera connector (the narrow Zero-size ribbon cable) |
| microSD card | 8 GB or more |
| Windows PC (64-bit, Windows 10/11, with Wi-Fi) | Runs the driving-manager app |

## Getting started

1. From the latest [Release](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) download `rumicar-zero2cam-<version>-<date>.img.xz` (SD-card image) and `RumiCarDriveManager-<version>-win-x64.zip` (PC app).
2. Write the `.img.xz` to the SD card with [Raspberry Pi Imager](https://www.raspberrypi.com/software/) (answer **No** to "apply OS customisation settings" — the settings are already in the image).
3. Put the car on a stand and switch it on. The first boot reboots itself once after 2–3 minutes, then the car starts driving.
4. Connect the PC's Wi-Fi to `RumiCar` (no password), unzip the app, run `RumiCarDriveManager.exe` and click 接続 (connect).

The full procedure, the screen layout and troubleshooting are in the [RumiCar-Zero2Cam README](https://github.com/RumiCar-group/RumiCar-Zero2Cam/blob/main/README.en.md).

## Compared with the other computer modules

| | Arduino Nano / ESP32 | Raspberry Pi Zero W ([RasPi](../RasPi) folder) | **Zero 2 W + camera (Zero2Cam)** |
|---|---|---|---|
| Language | C++ (Arduino IDE) | Python | C++ on the car / C# driving manager on the PC |
| How programs get onto the car | Upload over USB | Copy Python onto the OS on the SD card | Write the SD-card image (no build); driving logic is pushed from the PC and swapped at runtime |
| Camera | None | Optional (video-streaming example) | Standard (320×240 at about 20 fps, streamed to and recorded on the PC) |
| Link to the PC | USB serial / Bluetooth | SSH | Wi-Fi (the car is the access point) |
| Wiring | Arduino diagram | Zero W diagram | Same as Zero W, plus the camera |

Please file issues and pull requests in [RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam/issues), in English or Japanese.
