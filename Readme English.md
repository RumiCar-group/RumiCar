![RumiCarロゴ](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCarロゴ")
![RumiCar写真](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar写真")

# RumiCar

Please check a type of Computer Module (CM) and programming languages. If you use Arduino Nano or ESP32, please go to an "ArduinoAndESP32" folder. If you use Raspberry Pi Zero W, please go to a "RasPi" folder. Python is used for programs in "RasPi."

Four images are links to YouTube. Click them to watch.

- Introduction of RumiCar
- RumiCar collision avoidance
- [Part 2 of 2] RumiCar on test course
- RumiCar car camera view at test drive on August 30, 2020

[![RumiCar紹介動画](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "RumiCar紹介動画")
[![RumiCar衝突回避動画](http://img.youtube.com/vi/95pc_4Wf14U/0.jpg)](https://youtu.be/95pc_4Wf14U "RumiCar衝突回避動画")
[![RumiCarテスト走行動画](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCarテスト走行動画")
[![RumiCar車載カメラ視点動画](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "RumiCar車載カメラ視点動画")
### About folders

- **ArduinoAndESP32 folder**<br>There are sample programs for RumiCar's Arduino Nano CM and ESP32 CM in this folder. These programs are used in hands-on events. Slides used in hands-on events are in a folder named **(Material for hands-on) ハンズオン用テキスト**.

  - **Exercise-1 Measure distance**<br>RumiCar has three laser distance measurement modules on the front. Laser distance measurement modules determine distance by time of flight (ToF), which hit harmless lasers objects to calculate difference of ***time*** between when the module emits laser and when reflected laser comes back. Speed of sound is also called Mach, which is 1234.8 km/h (761.2 mph). Moreover, speed of light is 880,000 times as fast as sound of speed. It is incredible that measureing such a high speed, isn't it?
  
    - **Exercise-1.1 Measure by a center sensor**<br>In this exercise, a center senser of three laser distance measurement modules on RumiCar detect distance. Sensor values of distance will be displayed on your computer in real time by using Arduino IDE's Serial Monitor. If you place your hand in front of the sensor, distance between your hand and the sensor is changed, so its sensor values on Serial Monitor is changed at the same time.
    - **Exercise-1.2 Measure by three sensors**<br>In this exercise, all three sensors on RumiCar are used. Sensor values of distance gotten from left, center, and right one are displayed on Serial Monitor in real time.
    - **Exercise-1.3 Serial Plotter**<br>Arduino IDE has Serial Plottor, which generates a graph from sensor values. The program is the same as Exercise-1.2. Each line which indicates each sensor's values is indicated by different color and plotted into graph. Place your hand in from of sensors like you did in Exercise-1.2, and see how it works. Make sure which color shows which sensor valuses. There is no specific program in this exercise because you will use Exercise-1.2.
  - **Exercise-2 Motor control**<br>RumiCar uses motors when turning and driving. A computer inside CM on RumiCar turns the steering and spins motors by following a program. Let's control RumiCar's motors by using a CM.
    - **Exercise-2.1 Turn the steering**<br>In this exercise, RumiCar turns the steering right and left for 0.5 seconds, and it repeats the action. In other words, RumiCar repeats turning to left for 0.5 seconds and to right for 0.5 seconds.
    - **Exercise-2.2 Speed control**<br>In this exercise, RumiCar controls driving speed. Commands from a CM spin a driving motor which spins a back wheel motor. By changing rational sped of a tire, RumiCar changes driving speed. This program controls rational speed of the tire step by step. You will notice that sound is changed by changing rational speed of the motor.
    - **Exercise-2.3 Drive forward and reverse**<br>As a public car drives forward and reverse, so does RumiCar in this exercise. If there is not enough space for driving RumiCar, grab RumiCar by your hand and see how RumiCar's motor driving forward and reverse.
    - **Exercise-2.4 Zigzag drive**<br>In this exercise, RumiCar drives zigzag while driving forward. If there is not enough space for driving RumiCar, grab RumiCar by your hand and see RumiCar drives zigzag.
  - **Exercise-3 Basic of autonomous driving**<br>In this Exercise 3 which is basic of autonomous driving, you will control RumiCar based on sensor values. When RumiCar detects distance among objects in front of RumiCar or walls on the right or left, it stops or turns the steering to right or left.
    - **Exercise-3.1 Safety stop**<br>In this exercise, you will think about a vehicle which stops safely when it finds an obstacle on the front. RumiCar drives straight, and if there is no more obstacles in front, RumiCar will drive again. When an obstacle abroaches RumiCar, it drives reverse for safety. Suppose your hand is an obstacle and place it in front of RumiCar, make sure RumiCar stops without collision. Does RumiCar drive reverse when your hand approach RumiCar while it stops?
    - **Exercise-3.2 A vehicle which drives in a town**<br>In this exercise, RumiCar will drive in a test course. RumiCar dirves by detecting walls on sides and obstacles in front of RumiCar.
- **ESP32 folder**<br>ESP has wireless interference, such as WiFi and Bluetooth. Therefore, an ESP CM, which is one of the RumiCar CMs, has WiFi and Bluetooth. In this folder, there are sample programs to receive sensor values from RumiCar via ESP32's Bluetooth and wirelessly control RumiCar.
    - **BLE folder**<BR>BLE stands for Bluetooth Low Energy. Let's have a smartphone (iPhone) telecommunicate with BLE inside a RumiCar ESP32 CM. Sensor values from three laser distance measurement modules are displayed on BLE Scanner, which is an iPhone's app. BLE can do multiple connection; in other words, there is no need to disconnect other devices. Therefore, multiple devices can connect with one RumiCar at the same time. At an exhibition, you can plan a performance; for instance, visitors install a BLE Scanner app and see sensor values on their smart phone.
    - **Bluetooth folder**<br>Exercise-1.2 is a sample program for ESP32 CM's Bluetooth serial communication. This enables RumiCar to connect with a computer installed Windows and Bluetooth, display sensor values on a computer from laser distance measurement modules, and control RumiCar by sending commands from a computer.
      - Pairing Windos10
      - Settings ->Device ->Bluetooth and other devices->Bluetooth or add other devices ->Bluetooth
      - Select RumiCar_ESP32
      - Device manager ->On Port(COM & LPT), check "Standard Serial over Bluetooth link (COM #)" # is a number.　(There are two numbers.)
      - On setting on Arduino IDE, tool ->On Serial ports, select COM # which you saw at device manager
      - Now sensor values are displayed even though ESP32 is not connected by physical cables.
      - You can control RumiCar by sending commands from your computer.
      - The sample program has commands, "r", "l", "f", and "b". Each command conducts right, left, forward, and reverse, for 0.5 seconds. All comands are small letters. Send commands via serial monitor in Arduino IDE.
      - There is no need to connect by cables, so it is possible to recieve sensor values while RumiCar is driving or operate RumiCar like radio controlled cars by commands from a computer.

- **RasPi folder**<br>In this folder, there are sample programs written in Python for RumiCar RasPi CMs. This folder is for the purpose of using a Raspberry Pi CM. 
  - **Exercise-1 Measure distance**
    - **Exercise-1.1 Measure by a center sensor**
    - **Exercise-1.2 Measure by three sensors**   
  - **Exercise-2 Motor control**   
    - **Exercise-2.1 Turn the steering**    
    - **Exercise-2.2 Speed control**
    - **Exercise-2.3 Drive forward and reverse**
    - **Exercise-2.4 Zigzag drive**  
   - **Exercise-3 Basic of autonomous driving**
     - **Exercise-3.1 Safety stop**   
     - **Exercise-3.2 A vehicle which drives in a town**
 - **Hands-on materials folder**<br>In this folder, there are materials used at hands-on events. You can learn history and machanism of RumiCar, and instructions how to control RumiCar by sample programs. **The size of the materials which are PDF file format is about 10 MB. If you see them on GitHub, it sometimes does not work properly. Dowload them before you read them.**
- **Additional materials for educational institutions folder**<br>In this folder, there are materials in case that RumiCar is used at class or lectures. While RumiCar hands-on materials are designed to learn mechanism or systems of RumiCar, the materials in this folder are for additional assignment for students in research or group works to think more deeply, work on assignment, and develop better algorithms.
- **Materials for development folder**<br>In this folder, there are materials for building RumiCar car bodies and CMs.

1. If you build a RumiCar car on your own, please read "How to make RumiCar," build RumiCar, and verify operation your RumiCar by following an instruction, "How to verify operation of RumiCar." I would be grateful if many people could develop their own RumiCar and enjoy them.<br>1.1 Added the information of processes how to make CMs except Arduino Nano<br>1.2 Added pin assignment 13 in case of replacing sterring with a servomotor
1. If you recieve RumiCar as delivery, follow "How to verify operation of RumiCar" to inspect RumiCar.
1. There are pin assignment talbes for each CM. When you make your own CM, please follow pin assignment due to interchangability of programs.
  - **Wiring diagram folder**
    - Wiring diagram(ArduinoNano).jpg<br>This wiring diagram for Arduino Nano CM is basic of RumiCar. This is recommended for beginners.
    - Wiring diagram(Raspberry Pi Zero W).png<br>This wiring diagram is for a Raspberry Pi Zero W CM. This is recommended for advaned level who want to code in Python or try image recognition, and AI.
    - Wiring diagram RumiCarG3.1_ESP32.jpg<br>This wiring diagram is for an ESP32 CM. This is recommended for advanced level who want to try WiFi, Bluetooth connection, and neural network (NN).
    - Wiring diagram for a car body.jpg<br>This wiring diagram is for inside a RumiCar car body. If you build your own RumiCar car, please follow this interface.
    - Pin assignment Arduino-ESP32-RasPi-Obniz<br>This is a summary of interface of a RumiCar's car and pin assignment for each CM, Arduino Nano, ESP32, Raspberry Pi Zero W, and Obniz.
  - **How to make RumiCar**<br>There are processes of making a RumiCar car for those who build RumiCar on your own. Check up your RumiCar by following "How to verify operation of RumiCar."   
  - **How to verify operation of RumiCar**<br>In this folder, there are materials to check up RumiCar you build or make sure RumiCar works properly after delivery.

  ### Other important information
  - **RumiCar website**<br>This is a RumiCar website. If you would like to edit the website, contact us!<br>https://www.rumicar.com
  - **RumiCar YouTube**<br>This is a RumiCar YouTube channel. There are videos about introductions, event archives, scenery of developing RumiCar. Please subscribe to it. If you can edit videos, please contact us.<br>
  https://www.youtube.com/channel/UCVg3CBSVBcc_00FdC6q2wDg
  - **RumiCar group**<br>This is a Facebook group of RumiCar. Join in a RumiCar community.<br>
  https://www.facebook.com/groups/rumicar
  
  - **connpass**<br>This connpass is an event page for RumiCar test drive, hands-on events, or advertisements for events. If you are interested in attending those events, please join the group.<br> 
   https://rumicar.connpass.com

  - **Event archive for online ALGYAN & RumiCarRumiCar hands-on**<br>This YouTube video is an archive when RumiCar collaborated with ALGYAN to hold an event. In this video, there are explanation for Exercises and demonstration for RumiCar.<br>https://youtu.be/99zH73B8NUo
