![RumiCarロゴ](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCarロゴ")
![RumiCar写真](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar写真")

# RumiCar
The program and the computer language used will vary depending on the type of computer module; please be sure to use the corresponding programming language. If you use Arduino Nano or ESP32, please go to the [ArduinoAndESP32](https://github.com/RumiCar-group/RumiCar/tree/master/ArduinoAndESP32) folder. If you use Raspberry Pi Zero W, please go to the [RasPi](https://github.com/RumiCar-group/RumiCar/tree/master/RasPi) folder (RasPi uses Python).

The following are links to YouTube videos:

- [Introduction of RumiCar](https://youtu.be/DxaY2eCzJzo)
- [RumiCar collision avoidance](https://youtu.be/95pc_4Wf14U)
- [RumiCar test drive footage](https://youtu.be/oHujTh9AwAw)
- [RumiCar test drive footage, camera view](https://youtu.be/16kOgLMo-Tg)

[![RumiCar紹介動画](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "RumiCar紹介動画")
[![RumiCar衝突回避動画](http://img.youtube.com/vi/95pc_4Wf14U/0.jpg)](https://youtu.be/95pc_4Wf14U "RumiCar衝突回避動画")
[![RumiCarテスト走行動画](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCarテスト走行動画")
[![RumiCar車載カメラ視点動画](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "RumiCar車載カメラ視点動画")
### About folders

- [**ArduinoAndESP32**](https://github.com/RumiCar-group/RumiCar/tree/master/ArduinoAndESP32) folder<br>This folder includes sample programs for RumiCar's Arduino Nano CM and ESP32 versions, for use at hands-on events. Presentation slides for such events are in the folder named **[ハンズオン用テキスト](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88)(Material for hands-on)**.

  - **Exercise-1 Measuring distance**<br>RumiCar has three laser distance measurement modules on the front. Laser distance measurement modules determine distance by time of flight (ToF), which emits harmless lasers towards objects to calculate the  ***time*** between the laser's emission and return. The speed of sound is measured by the Mach, which is a unit of 1234.8 km/h (761.2 mph). Meanwhile, the speed of light is 880,000 times as fast as the speed of sound; it is an incredible thing, one must say, that such high speeds can be recorded.
  
    - **Exercise-1.1 Using the Central Sensor**<br>This exercise detects distance with the central unit of the three laser distance measurement modules on RumiCar. By using Arduino IDE's Serial Monitor, the measured values will be displayed on your computer in real time. If you move your hand back and forth in front of the sensor, the distance between your hand and the sensor changes, and so should the values displayed on the monitor.
    - **Exercise-1.2 Using the Triple Sensor**<br>This exercise uses all three sensors on RumiCar. The values of distance observed by the left, center, and right sensors are displayed on the Serial Monitor in real time.
    - **Exercise-1.3 Using the Serial Plotter**<br>Arduino IDE has a Serial Plottor, which generates a graph from sensor values, in which each line indicates each sensor's values in a different color. Wave your hand in front of the sensors like you did in Exercise-1.2, and see how it works. Note which color shows which sensor's values. There is no specific program for this exercise because you will use the same code as Exercise-1.2.
  - **Exercise-2 Speed Control**<br>RumiCar uses motors when turning and driving. A computer inside CM on RumiCar turns the steering and spins motors by following a program. Let's control RumiCar's motors by using a CM.
    - **Exercise-2.1 Steering**<br>In this exercise, RumiCar repeats the motions of turning its steering wheel, then maintaining its position for 0.5 seconds. In other words, RumiCar repeats turning left for 0.5 seconds and turning right for 0.5 seconds.
    - **Exercise-2.2 Speed control**<br>This is an exercise that controls driving speed. The Computer Module's commands should make the back wheels spin. Changing the tire's rotation speed should also change driving speed. This program controls the rational speed of the tire in steps. You will also notice differences in the motor's sound.
    - **Exercise-2.3 Driving Forward and Reverse**<br>As a public car drives forward and reverse, so does RumiCar in this exercise. If there is not enough space for driving RumiCar, grab RumiCar by your hand and see how RumiCar's motor drives forward and reverse.
    - **Exercise-2.4 Zigzagging**<br>In this exercise, RumiCar drives forward in a zig-zagging pattern while driving forward. If there is not enough space to RumiCar, grab RumiCar by your hand and make sure it is making the expected motions.
  - **Exercise-3 Basics of Autonomous Driving**<br>In these exercises, you will control RumiCar based on sensor values. When RumiCar detects distance among objects in front of RumiCar or walls on the right or left, it stops or turns the steering to right or left.
    - **Exercise-3.1 Stopping the Car safely**<br>This exercise imagines a vehicle which stops safely when it finds an obstacle in front of it. When an obstacle approaches RumiCar, it drives in reverse for safety. If there are no more obstacles in front, RumiCar will drive straight again. Use your hand as an obstacle and place it in front of RumiCar. Make sure RumiCar stops without collision. Does the RumiCar drive in reverse when your hand approaches?
    - **Exercise-3.2 Driving in Urban Areas**<br>In this exercise, RumiCar will drive in a test course, while detecting walls on sides and obstacles in front of it.
- [**ESP32**](https://github.com/RumiCar-group/RumiCar/tree/master/ESP32) folder<br>Out of RumiCar's CMs, ESP32 has built-in communication functions such as WiFi and Bluetooth. In this folder, there are sample programs to receive sensor values from RumiCar via the ESP32's Bluetooth function, and wirelessly control RumiCar.
    - **BLE folder**<BR>Let's have a smartphone (iPhone) telecommunicate with BLE (BLE stands for Bluetooth Low Energy.) inside a RumiCar ESP32 CM. Sensor values from three laser distance measurement modules are displayed on BLE Scanner, which is an iPhone app. BLE can make multiple connections; there is no need to disconnect other devices. Therefore, multiple devices can connect with one RumiCar at the same time. At an exhibition, you can plan a performance; for instance, visitors install a BLE Scanner app and see sensor values on their smartphone.
    - **Bluetooth folder**<br>Exercise-1.2 is a sample program for ESP32 CM's Bluetooth serial communication. This enables RumiCar to connect with a Windows PC via Bluetooth, to display the values from the laser distance measurement modules, and control RumiCar by sending commands.
      - Pairing with Windos10
      - Settings ->Device ->Bluetooth and Other Devices->Add Bluetooth or Other Devices ->Bluetooth
      - Select RumiCar_ESP32
      - In Device Manager ->Port(COM & LPT), check "Standard Serial over Bluetooth link (COM #)" (Substitute # for the numbers displayed here; there should be two of them.)
      - Now go to Arduino IDE setting and select. Tool ->Serial Ports, then select the same numbers as the COM #
      - Now the distance measurement values should be displayed even though ESP32 is not connected by physical cables.
      - You can control RumiCar by sending commands from your computer.
      - The sample program supports the commands, "r", "l", "f", and "b". Each command conducts right, left, forward, and reverse, for 0.5 seconds. All commands are case-sensitive, and use lower-case letters. The commands are sent by sending the serial monitor of the Arduino IDE.
      - No cable connection is required, so it is possible to receive data while RumiCar is driving, or send commands from the PC side to operate the system like a radio controller.

- [**RasPi**](https://github.com/RumiCar-group/RumiCar/tree/master/RasPi) folder<br>This is a sample program for the RasPi version of the RumiCar computer module, written in Python. The program is written in Python and can be used to run RumiCar using RasPi CM. 
  - **Exercise-1 Measuring distance**
    - **Exercise-1.1 Using the Central Sensor**
    - **Exercise-1.2 Using Triple Sensor**   
  - **Exercise-2 Speed control**   
    - **Exercise-2.1 Steering**    
    - **Exercise-2.2 Speed Control**
    - **Exercise-2.3 Driving Forward and Reverse**
    - **Exercise-2.4 Zigzagging drive**  
   - **Exercise-3 Basics of Autonomous Driving**
     - **Exercise-3.1 Stopping the Car Safely**   
     - **Exercise-3.2 Driving in Urban Areas**
 - [**Hands-on materials**](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88)<br>In this folder, there are materials used at hands-on events. You can learn the history and machanism of RumiCar, and sample programs of how to control RumiCar. **The PDF file is about 10 MB, so if you try to directly access them on GitHub, you may not be able to view them properly. Please download the file before reading it.**
- [**Additional Materials for Educational Institutions**](https://github.com/RumiCar-group/RumiCar/tree/master/%E6%95%99%E8%82%B2%E6%A9%9F%E9%96%A2%E5%90%91%E3%81%91%E8%BF%BD%E5%8A%A0%E8%AA%B2%E9%A1%8C)<br>In this folder, there are materials for educational institutions using RumiCar as a teaching tool in their classes or lectures. These are designed to help students learn mechanisms and systems of RumiCar, and are for additional assignments such as research projects and group work, so students can think more deeply, overcome challenges, and develop better algorithms.
- [**Development Documents Folder**](https://github.com/RumiCar-group/RumiCar/tree/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99)<br>This folder contains materials for building your own RumiCar body and CMs.

1. If you build a RumiCar car on your own, please read "How to make RumiCar" first, and after making build RumiCar, verify its operation by following "How to verify operation of RumiCar." We hope many people will develop their own original RumiCars and enjoy them.<br>1.1 Added the information of processes how to make CMs other than Arduino Nano<br>1.2 Added pin assignment 13 for steering with a servomotor
1. If you receive RumiCar as delivery, follow "How to verify operation of RumiCar" to inspect if RumiCar has been damaged due to transportation.
1. There are pin assignment tables for each CM. When you make your own CM, please follow the tables to ensure compatibility.
  - [**Wiring Diagram Folder**](https://github.com/RumiCar-group/RumiCar/tree/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99/%E7%B5%90%E7%B7%9A%E5%9B%B3)
    - Wiring diagram(ArduinoNano).jpg<br>This is the basic wiring diagram for the Arduino Nano CM version of RumiCar, recommended for beginners.
    - Wiring diagram(Raspberry Pi Zero W).png<br>This wiring diagram is for the Raspberry Pi Zero W version CM. This is recommended for advanced users who want to code in Python or use image recognition and AI.
    - Wiring diagram RumiCarG3.1_ESP32.jpg<br>This wiring diagram is for an ESP32 CM. This is recommended for advanced users who want to try WiFi and Bluetooth connections, and neural network (NN) programming, etc.
    - Car Body Connection Diagram.jpg<br>This wiring diagram for the RumiCar car body. If you build your own RumiCar, please use this interface.
    - Pin assignment Arduino-ESP32-RasPi-Obniz<br>This is a table showing the interface of RumiCar and the pin assignment for the Arduino Nano, ESP32, RasPi, and Obniz versions.
  - [**How to make RumiCar**](https://github.com/RumiCar-group/RumiCar/blob/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99/RumiCar%E3%81%AE%E4%BD%9C%E3%82%8A%E6%96%B9.pdf)<br>Tthis document will help you build a RumiCar on your own. Once it is complete, check its functionality by following "How to Verify Operation of RumiCar."   
  - [**How to verify operation of RumiCar**](https://github.com/RumiCar-group/RumiCar/blob/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99/RumiCar%E3%81%AE%E5%8B%95%E4%BD%9C%E7%A2%BA%E8%AA%8D%E6%96%B9%E6%B3%95.pdf)<br> This is a document that outlines the steps to check if your RumiCar is working properly when you build a new one, or if it has been damaged due to transportation. Please refer to this document to make sure your RumiCar is working properly.

  ### Other important information
  - **RumiCar website**<br>This is the website for RumiCar. We would like to develop this website in the future, so if you are interested in editing the website, please contact us!<br>https://www.rumicar.com
  - **RumiCar YouTube**<br>RumiCar's YouTube channel, with videos of the car's introduction, various driving videos, and videos of various tests during development. Subscriptions are appreciated! If you are interested in editing our videos, please contact us.<br>
  https://www.youtube.com/channel/UCVg3CBSVBcc_00FdC6q2wDg
  - **RumiCar group**<br>This is a Facebook group of RumiCar. We look forward to your participation.<br>
  https://www.facebook.com/groups/rumicar
  
  - **connpass**<br>This connpass is an event page for RumiCar test drive, hands-on events, or advertisements for events. Please join if you are interested in attending.<br> 
   https://rumicar.connpass.com

  - **Event archive for online ALGYAN & RumiCarRumiCar hands-on**<br>This YouTube video is an archive of the RumiCar even in collaboration with ALGYAN. Watch this for explanation and demonstrationsfor RumiCar exercises.<br>https://youtu.be/99zH73B8NUo
