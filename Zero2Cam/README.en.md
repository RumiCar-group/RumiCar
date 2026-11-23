# Raspberry Pi Zero 2 W + camera version (RumiCar Zero2Cam)

**English** | [日本語](README.md)

The software for the RumiCar that carries a Raspberry Pi Zero 2 W and a Camera Module 2 lives in its own repository, **[RumiCar-group/RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam)**. This folder is only a signpost; it contains no programs.

| | |
|---|---|
| Programs and user guide | [RumiCar-Zero2Cam README](https://github.com/RumiCar-group/RumiCar-Zero2Cam/blob/main/README.en.md) (English) |
| Downloads (SD-card image, PC app) | [Releases](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) |
| Specification | [docs/RumiCar-Zero2Cam 仕様書.docx](https://github.com/RumiCar-group/RumiCar-Zero2Cam/tree/main/docs) (Japanese) |

## What it is

- An autonomous car that avoids walls and obstacles with the three front ToF sensors. Switching it on does not make it drive: it boots into record-only mode (motors braked) and starts driving when you select **自動** (auto) in the driving manager on the PC (since v0.1.3; a setting can make it drive at power-on without a PC).
- The car is a Wi-Fi access point. Connect a PC to it and the driving-manager app (Windows) shows the status and the onboard camera image, switches between auto / manual / record-only, triggers the emergency stop, and drives the car manually (driving pad, scrolling, arrow keys).
- Every received image and status frame is recorded on the PC. Open the recording folder in the Claude desktop app to improve the decision data, then push it to the car from the driving manager — without stopping the car.
- You can also write your own driving program in Python (`drive.py`, in `setup()` / `loop()` form, with the same functions as `RumiCar.py` for the Raspberry Pi) and send it to the car from the driving manager.
- Nothing to build: write the SD-card image and the software is ready.

## What you need

| Item | Notes |
|---|---|
| A RumiCar vehicle with a Raspberry Pi Zero 2 W computer module | The wiring is **the same as the Raspberry Pi Zero W CM** (see "結線図(Raspberry Pi Zero W).png" in [開発用資料](../%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99) and the pin table in [RasPi/README.md](../RasPi/README.md)): steering GPIO 17/27, drive GPIO 18/13 (hardware PWM), ToF XSHUT GPIO 23/24/25, I2C (GPIO 2/3) |
| Camera Module 2 | On the Zero 2 W camera connector (the narrow Zero-size ribbon cable) |
| microSD card | 8 GB or more |
| Windows PC (64-bit, Windows 10/11, with Wi-Fi) | Runs the driving-manager app |

## Getting started

1. From the latest [Release](https://github.com/RumiCar-group/RumiCar-Zero2Cam/releases) download `rumicar-zero2cam-<version>-<date>.img.xz` (SD-card image) and `RumiCarDriveManager-<version>-win-x64.zip` (PC app).
2. Write the `.img.xz` to the SD card with [Raspberry Pi Imager](https://www.raspberrypi.com/software/) (answer **No** to "apply OS customisation settings" — the settings are already in the image).
3. Put the car on a stand and switch it on. The first boot reboots itself once after 2–3 minutes. The car then waits in record-only mode and does not drive yet.
4. Connect the PC's Wi-Fi to `RumiCar` (no password), unzip the app, run `RumiCarDriveManager.exe` and click 接続 (connect).
5. Press **自動** (auto) under 走行モード (drive mode) to start driving. For a first drive, or on a course whose behaviour you do not know yet, set 自動の最大出力 (auto limit) to 30–50 % (77–128) first, and raise it while watching the car.

The full procedure, the screen layout and troubleshooting are in the [RumiCar-Zero2Cam README](https://github.com/RumiCar-group/RumiCar-Zero2Cam/blob/main/README.en.md).

## Compared with the other computer modules

| | Arduino Nano / ESP32 | Raspberry Pi Zero W ([RasPi](../RasPi) folder) | **Zero 2 W + camera (Zero2Cam)** |
|---|---|---|---|
| Language | C++ (Arduino IDE) | Python | C++ on the car / C# driving manager on the PC; your own driving program can also be written in Python (`drive.py`) |
| How programs get onto the car | Upload over USB | Copy Python onto the OS on the SD card | Write the SD-card image (no build); driving logic, decision data and `drive.py` are pushed from the PC and swapped at runtime |
| Camera | None | Optional (video-streaming example) | Standard (320×240 at about 20 fps, streamed to and recorded on the PC) |
| Link to the PC | USB serial / Bluetooth | SSH | Wi-Fi (the car is the access point) |
| Wiring | Arduino diagram | Zero W diagram | Same as Zero W, plus the camera |

Please file issues and pull requests in [RumiCar-Zero2Cam](https://github.com/RumiCar-group/RumiCar-Zero2Cam/issues), in English or Japanese.
