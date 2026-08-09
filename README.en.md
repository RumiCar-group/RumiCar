![RumiCar logo](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCar logo")
![RumiCar photo](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar photo")

# RumiCar

**English** | [日本語](README.md)

> [!IMPORTANT]
> ## 📢 Announcement
>
> **The RumiCar Arduino library has been published to the Arduino Library Manager.**
>
> Installation steps (all done inside Arduino IDE):
>
> 1. Open **Sketch → Include Library → Manage Libraries...** in Arduino IDE
> 2. Type `RumiCar` in the search box
> 3. Click **Install** on the `RumiCar` entry
> 4. When prompted to install the dependency (VL53L0X), choose **Install all**
>
> Library source code (for reference): [RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib)
>
> The local copy that used to be under `ArduinoAndESP32/Libraries/` **has been removed**. Older materials may still say "select RumiCar.zip" — that procedure no longer works; please use the Library Manager above.

RumiCar is a platform to develop autonomous driving algorithms with a palm-sized vehicle. The program and the programming language depend on the computer module (CM) you use: for Arduino Nano and ESP32, use the [ArduinoAndESP32](https://github.com/RumiCar-group/RumiCar/tree/master/ArduinoAndESP32) folder; for Raspberry Pi Zero W, use the [RasPi](https://github.com/RumiCar-group/RumiCar/tree/master/RasPi) folder (the RasPi version is written in Python).

The images below link to YouTube videos — click to watch:

- [Introduction of RumiCar](https://youtu.be/DxaY2eCzJzo)
- [RumiCar collision avoidance](https://youtu.be/95pc_4Wf14U)
- [RumiCar on the test course](https://youtu.be/oHujTh9AwAw)
- [Onboard camera view](https://youtu.be/16kOgLMo-Tg)

[![Introduction of RumiCar](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "Introduction of RumiCar")
[![RumiCar collision avoidance](http://img.youtube.com/vi/95pc_4Wf14U/0.jpg)](https://youtu.be/95pc_4Wf14U "RumiCar collision avoidance")
[![RumiCar test drive](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCar test drive")
[![Onboard camera view](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "Onboard camera view")

### Related repositories (the RumiCar family)

<!-- AUTO-REPOS-EN:START -->
<!-- この表は自動生成です。手で編集しないでください(毎日、公開リポジトリ一覧から再生成されます)。
     説明文・表示順の変更は .github リポジトリの profile/repos-meta.json を編集してください。
     This table is auto-generated daily from the org's public repositories.
     Edit profile/repos-meta.json in the .github repository to change descriptions/order. -->
| Repository | Description | ★ | Updated |
|---|---|---|---|
| [**RumiCar**](https://github.com/RumiCar-group/RumiCar) | Main repository: sample programs, hands-on textbooks (PDF), wiring diagrams, build docs, simulator programs and courses | 35 | 2026-08-07 |
| [**RumiCar-lib**](https://github.com/RumiCar-group/RumiCar-lib) | Arduino library for RumiCar (published to the Arduino Library Manager) | - | 2026-06-22 |
| [**RumiCar-Simulator**](https://github.com/RumiCar-group/RumiCar-Simulator) | Browser-based autonomous-driving simulator (no install, real-car compatible) | - | 2026-08-07 |
| [**RumiCar-PhotoStrip**](https://github.com/RumiCar-group/RumiCar-PhotoStrip) | Bandwidth-friendly photo strip for any website (generic web component, MIT) | - | 2026-08-09 |
| [**RumiCarRos**](https://github.com/RumiCar-group/RumiCarRos) | ROS 2 support for RumiCar | 3 | 2026-05-10 |
| [**RumiCarEX**](https://github.com/RumiCar-group/RumiCarEX) | Fork of a related project (derived vehicle) | - | 2020-09-05 |
| [**micro-airc**](https://github.com/RumiCar-group/micro-airc) | Fork of a related project (AI RC car) | 1 | 2020-07-19 |
| [**rumicar-duck**](https://github.com/RumiCar-group/rumicar-duck) | Fork of a related project (duck-style car) | - | 2020-04-25 |
<!-- AUTO-REPOS-EN:END -->

### Folder guide

- [**ArduinoAndESP32**](https://github.com/RumiCar-group/RumiCar/tree/master/ArduinoAndESP32)<br>Sample programs for the Arduino Nano and ESP32 versions of the RumiCar computer module (CM). These are the programs used in our hands-on workshops. The hands-on textbook lives in the [ハンズオン用テキスト (hands-on text)](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88) folder — start there.

  - **Exercise 1 — Let's measure distance!**<br>RumiCar carries three laser ranging (ToF) modules at the front of the vehicle. They shine a human-safe laser at an object and measure distance from the ***time*** it takes the reflection to come back. Sound travels at over 1,000 km/h, but light is about ***880,000 times*** faster — measuring that tiny interval to get a distance is remarkable.
    - **Exercise 1.1 — Measure with the centre sensor**<br>Use the centre one of the three ranging modules and display the measured distance in real time on your PC with the Arduino IDE Serial Monitor. Move your hand back and forth in front of the sensor and watch the value change.
    - **Exercise 1.2 — All three sensors at once**<br>Run the left, centre, and right sensors simultaneously and display all three distances in real time.
    - **Exercise 1.3 — Try the Serial Plotter**<br>The Arduino IDE can also plot data as a live graph. Using the same program as Exercise 1.2, the three distances are drawn as colour-coded curves. Wave your hand in front of the sensors and work out which colour belongs to which sensor. (No dedicated program — reuse Exercise 1.2.)
  - **Exercise 2 — Motor control**<br>RumiCar uses motors for steering and for driving. Let the CM move them under program control.
    - **Exercise 2.1 — Turn the steering**<br>Swing the front wheels left and right, holding each side for 0.5 seconds, forever.
    - **Exercise 2.2 — Speed control**<br>Drive the rear wheels and change their rotation speed step by step. Listen to the motor pitch change as you watch the wheels.
    - **Exercise 2.3 — Forward and reverse**<br>Drive RumiCar forwards and backwards. If you have no space to run it, hold the car firmly and watch the rear wheels change direction.
    - **Exercise 2.4 — Zig-zag driving**<br>Drive forward while steering left and right. Again, holding the car in your hand works fine for checking the behaviour.
  - **Exercise 3 — Autonomous driving basics**<br>Make decisions from the sensor readings: measure the distance to obstacles ahead and walls on both sides, stop, and steer while driving.
    - **Exercise 3.1 — A car that stops safely**<br>The car stops when it sees an obstacle ahead, resumes when the obstacle is gone, and backs away if the obstacle keeps approaching. Use your hand as the obstacle: does RumiCar stop without hitting it? Does it retreat when your hand closes in?
    - **Exercise 3.2 — Driving through town**<br>Set up a real course and let RumiCar drive it, watching the walls on both sides and obstacles ahead.
- [**ESP32**](https://github.com/RumiCar-group/RumiCar/tree/master/ESP32)<br>The ESP32 CM has built-in Wi-Fi and Bluetooth. Sample programs here read the distance sensors wirelessly and control RumiCar over the air.
  - **BLE**<br>Bluetooth Low Energy communication with a smartphone (iPhone + the BLE Scanner app displays the three distance readings). Multi-connect is supported, so several phones can connect to one RumiCar at the same time — great for letting visitors connect with their own phones at exhibitions.
  - **Bluetooth**<br>Bluetooth serial (Exercise 1.2 for the ESP32 CM): pair RumiCar with a Windows PC, read distances without a cable, and send single-letter commands to drive it like an RC car. The sample understands `r`, `l`, `f`, `b` (right, left, forward, back — lower case, 0.5 s each), sent from the Serial Monitor.
    - Pairing with Windows 10: Settings → Devices → Bluetooth & other devices → Add Bluetooth or other device → Bluetooth → select **RumiCar_ESP32**; then check Device Manager → Ports (COM & LPT) for "Standard Serial over Bluetooth link (COM #)" (two entries appear), and set Tools → Serial Port in the Arduino IDE to that COM port.
- [**RasPi**](https://github.com/RumiCar-group/RumiCar/tree/master/RasPi)<br>Sample programs for the Raspberry Pi Zero W version of the CM, written in Python. The exercise line-up mirrors the Arduino one: Exercise 1 (1.1–1.2), Exercise 2 (2.1–2.4), Exercise 3 (3.1–3.2).
- [**ハンズオン用テキスト (hands-on text)**](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88)<br>The hands-on textbook (Japanese): RumiCar's history and mechanics, and how to run the sample programs. It is a PDF of about 10 MB; GitHub may fail to preview it, so download it first.
- [**教育機関向け追加課題 (extra exercises for schools)**](https://github.com/RumiCar-group/RumiCar/tree/master/%E6%95%99%E8%82%B2%E6%A9%9F%E9%96%A2%E5%90%91%E3%81%91%E8%BF%BD%E5%8A%A0%E8%AA%B2%E9%A1%8C)<br>Additional assignments for classes and seminars that use RumiCar as teaching material — research-project seeds and group-work tasks that push students to think deeper and build better algorithms.
- [**開発用資料 (build documentation)**](https://github.com/RumiCar-group/RumiCar/tree/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99)<br>Documentation for building your own RumiCar vehicle or CM (Japanese).
  1. To build your own RumiCar, start with "RumiCarの作り方" (How to build a RumiCar), then verify your build with "RumiCarの動作確認方法" (How to verify a RumiCar). We would love to see your original RumiCars!
  2. Received a RumiCar, or worried about shipping damage? Follow the same verification guide.
  3. Per-CM pin assignment tables are provided — follow them when wiring a home-built CM so the programs stay compatible.
  - **Wiring diagrams**: Arduino Nano (the standard starting point), Raspberry Pi Zero W (for Python / image recognition / AI), ESP32 (for Wi-Fi, Bluetooth, neural networks), the vehicle-side wiring table, and a combined pin-assignment sheet covering Arduino Nano / ESP32 / RasPi / Obniz.

- **RumiCar Simulator (browser-based)**<br>Write and test RumiCar driving programs in your browser → [**RumiCar Simulator**](https://www.rumicar.com/simulator/). No hardware needed; experience the handling differences between drivetrains (FF/FR/4WD × normal/drift). Arduino C++ programs written in the simulator **run unmodified on the real RumiCar**. Source code: [**RumiCar-group/RumiCar-Simulator**](https://github.com/RumiCar-group/RumiCar-Simulator) (self-hosting instructions included).
  - [**programs**](https://github.com/RumiCar-group/RumiCar/tree/master/programs)<br>Driving programs for the simulator: six "Fable Racing Line" programs tuned per drivetrain, with their **design rationale, the meaning of every constant, and real-car tuning steps** explained in [programs/README.md](https://github.com/RumiCar-group/RumiCar/tree/master/programs) (each .ino also documents why it is set up the way it is). Modified programs can be contributed to [programs/community](https://github.com/RumiCar-group/RumiCar/tree/master/programs/community) by pull request and become reusable by everyone from the simulator's "🌐 Community" list.
  - [**courses**](https://github.com/RumiCar-group/RumiCar/tree/master/courses)<br>Course data for the simulator: 30 test courses plus user-contributed courses ([courses/community](https://github.com/RumiCar-group/RumiCar/tree/master/courses/community)). See [courses/README.md](https://github.com/RumiCar-group/RumiCar/tree/master/courses) for how to contribute.

### Other important links

- [**RumiCar website**](https://www.rumicar.com)<br>We want to keep growing the website — if you would like to help edit it, please get in touch!
- [**RumiCar YouTube**](https://www.youtube.com/channel/UCVg3CBSVBcc_00FdC6q2wDg)<br>Introduction videos, driving footage, and development-test videos. Subscriptions welcome — and so are video editors!
- [**RumiCar group (Facebook)**](https://www.facebook.com/groups/rumicar)<br>Join us!
- [**connpass**](https://rumicar.connpass.com)<br>Announcements and sign-ups for driving meetups, development classes, and online seminars (Japan).
- [**Recorded hands-on stream**](https://youtu.be/99zH73B8NUo)<br>An archived RumiCar event in collaboration with ALGYAN, with exercise walk-throughs and demos.

Issues and pull requests are welcome **in English or Japanese**.

---

*Original English translation assistance: Emi Ito. Updated 2026-08-07 to match the current Japanese README.*
