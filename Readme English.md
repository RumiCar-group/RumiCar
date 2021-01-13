![RumiCarロゴ](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCarロゴ")
![RumiCar写真](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar写真")

# RumiCar

Please check a type of Computer Module (CM) and programming languages. If you use Arduino Nano or ESP32, please go to an "ArduinoAndESP32" folder. If you use Raspberry Pi Zero W, please go to a "RasPi" folder. Python is used for programs in "RasPi."

（An image is a link to YouTube.）

1. Introduction of RumiCar, 2.RumiCar collision avoidance, 3.[Part 2 of 2] RumiCar on test course, 4.RumiCar car camera view at test drive on August 30, 2020

[![RumiCar紹介動画](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "RumiCar紹介動画")
[![RumiCar衝突回避動画](http://img.youtube.com/vi/95pc_4Wf14U/0.jpg)](https://youtu.be/95pc_4Wf14U "RumiCar衝突回避動画")
[![RumiCarテスト走行動画](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCarテスト走行動画")
[![RumiCar車載カメラ視点動画](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "RumiCar車載カメラ視点動画")
### About folders

- **ArduinoAndESP32 folder**<br>There are sample programs for RumiCar's Arduino Nano CM and ESP32 CM in this folder. These programs are used in hands-on events. Slides used in hands-on events are in a folder named **ハンズオン用テキスト**, and it is a 10 MB PDF. Browsing it on GitHub sometimes deos not work, so dowload it to read it. If you have a RumiCar, download sample programs, load them into RumiCar, and enjoy.

  - **Exercise-1 Measure distance**<br>RumiCar has three laser distance measurement modules on front. Laser distance measurement modules determine distance by time of flight, which is hitting harmless lasers objects to calculate difference of ***time*** between when the module emits laser and when reflected laser comes back. Speed of sound is also called Mach, which is 1234.8 km/h (761.2 mph). Moreover, speed of light is 880,000 times as fast as sound of speed. It is incredible that measureing such a high speed of light.
  
    - **Exercise-1.1 Measure by a center sensor**<br>In this exercise, a center senser of three laser distance measurement modules on RumiCar detect distance. Sensor values of distance will be displayed on your computer in real time by using Arduino IDE's Serial Monitor. If you place your hand in front of the sensor, distance between your hand and the sensor is changed, so its sensor values on Serial Monitor is changed at the same time.
    - **Exercise-1.2 Measure by three sensors**<br>In this exercise, all three sensors on RumiCar are activated. Sensor values of distance gotten from left, center, and right one are displayed on Serial Monitor in real time.
    - **Exercise-1.3 Serial Plotter**<br>Arduino IDE has Serial Plottor, which indicates graphs from values. The program is the same as Exercise-1.2, and values of each sensor are classifed as different colors and plotted as graph. Place your hand in from of sensors like you did in Exercise-1.2, and see how it works. Make sure which color shows which sensor valuse. There is no specific program in this exercise because you will use Exercise-1.2.
  - **Exercise-2 Motor control**<br>RumiCar uses motors when turning and driving. A computer inside CM on RumiCar turns handle and spins motors by following a program. Let's control RumiCar's motors by using a CM.
    - **Exercise-2.1 Turn the steering**<br>In this exercise, RumiCar turns the steering right and left for 0.5 seconds, and it repeats the action. In other words, RumiCar repeats turning to left for 0.5 seconds and to right for 0.5 seconds.
    - **Exercise-2.2 Speed control**<br>In this exercise, RumiCar controls driving speed. Commands from a CM spins a driving motor which is back wheel. By changing rational sped of a tire, RumiCar changes driving speed. This program controls rational speed of the tire step by step. You will notice that sound is changed by changing rational speed of the motor.
    - **Exercise-2.3 Drive forward and reverse**<br>As a public car drives forward and reverse, so does RumiCar. In this exercise, プログラムを使ってRumiCarを前進させたり後進させたりしてみましょう。RumiCarを走らせる場所が無いときはRumiCarを手にしっかり持って後ろのタイヤが前進方向に回転したり、後進方向に回転するのを確認しましょう。
    - **Exercise-2.4 ジグザグ走行**<br> RumiCarをジグザグに走行させてみましょう。ハンドルを左右に切りながら前進します。RumiCarを走らせられる場所が無いときはRumiCarをしっかり手に持って期待通りの動きになっていることを確認しましょう。
  - **Exercise-3 自立走行基礎**<br>　自立走行の基礎としてセンサ等の情報を元に判断してRumiCarを制御してみます。前方の障害物や左右の壁などの距離を測定して、停止したり左右にハンドルを切りながらRumiCarを走行させてみます。
    - **Exercise-3.1 安全に停止する車**<br>　前方に障害物を発見して安全に停止する車を考えます。RumICarが直進中に前方に障害物を発見したら停止し、障害物が無くなったらまた走行再開します。障害物が近づいてきたら(?)、安全のために後進します。あなたの手を障害物と仮定してRumiCarがあなたの手にぶつからずに停止するか試してみましょう。RumICarが停止中にあなたの手をRumicarに近づけたら安全確保のためにRumiCarが後進しますか？
    - **Exercise-3.2 市街地を走る車**<br> この章では実際に走行コースを設置してRumiCarを走らせます。左右の壁や前方の障害物を検知しながらRumiCarが走行します。
- **ESP32フォルダ**<br>　RumiCarのコンピュータモジュールのうち、ESP32はWiFiやBlueToothなどの通信機能を内蔵しています。ここではESP32のBlueTooth機能を使ってRumiCarのセンサ測距値を無線で取得して表示させたり、RumiCarをワイヤレスで制御するサンプルプログラムが入っています。
     - **BLEフォルダ**<BR>　Bluetooth Low Energy(BLE)での通信。RumiCarのESP32版コンピュータモジュールに搭載されているBLEを使ってスマホ(iPhone)と通信してみました。3個の測距モジュールでの距離計測値をiPhoneのBLEScannerとのアプリで表示させています。マルチコネクトが可能なので、個別の機器と接続や切断が不要ですので、一度に複数の機器から1台のRumiCarに接続することができます。展示会などで来場者に各自のスマホからRumiCarに接続してもらって自分のスマホで計測値を表示させて楽しんで頂くことなどが可能です。
    - **Bluetoothフォルダ**<br>　Exercise-1.2はRumiCar　ESP32版用Bluetoothシリアル通信のサンプルプログラムです。RumiCarをWindows PCとBluetoothで接続してRumiCarのレーザー測距モジュールの測定距離値をパソコンに表示したりパソコンからコマンドを送ってRumiCarを操作することができます。
      - Windos10とペアリング
       - 設定->デバイス->Bluetoothとその他のデバイス->Bluetoothまたはその他のデバイスを追加する->Bluetooth
      - RumiCar_ESP32を選択
      - デバイスマネージャー->ポート(COMとLPT)で"Bluetooth リンク経由の標準シリアル(COMn)"　を確認する"n"は数字　(2個表示される)
      - Arduino IDEの設定でツール->シリアルポート:　で、上記COMnに合わせる
      - ESP32が「ケーブルをつないでいない」のに測距値がシリアルモニタに表示できます
      - PC側からコマンドを送ってRumiCarをコントロールできます
      - サンプルプログラムはコマンド「r」、「l」、「f」、「b」に対応、それぞれ右、左、前進、後進を0.5秒実行します。コマンドは全て小文字です。 Arduino IDEのシリアルモニタの送信でコマンドを送ります
      - ケーブル接続が不要になりますので走行中に実際の測距データをPC側で取得したりPC側からコマンドを送ってラジコンのように操作したり、ができるようになります

- **RasPiフォルダ**<br>　RumiCarのコンピュータモジュールのラズパイ版のサンプルプログラムです。Pythonで記述されています。ラズパイ版コンピュータモジュールを使ってRumiCarを動かす場合にお使いください。
  - **Exercise-1　距離を測ってみよう！**
    - **Exercise-1.1 中央のセンサで測距しよう！**
    - **Exercise-1.2 3つのセンサで測距だ！**   
  - **Exercise-2 モータ制御**   
    - **Exercise-2.1 ハンドルを切る**    
    - **Exercise-2.2 速度制御**
    - **Exercise-2.3 前進と後進**
    - **Exercise-2.3 前進と後進**
    - **Exercise-2.4 ジグザグ走行**  
   - **Exercise-3 自律走行基礎**
     - **Exercise-3.1 安全に停止する車**   
     - **Exercise-3.2 市街地を走る車**
 - **ハンズオン用テキストフォルダ**<br>　RumiCarのハンズオン用テキストが保存されているフォルダです。RumiCarの歴史や仕組み、サンプルプログラムを使ってRumiCarを動作させる方法や例が載っています。**ハンズオン用テキストはPDF形式で約１０MBほどのファイルサイズです。GitHubから直接参照しようとすると表示できないことがあるようです。一旦ダウンロードしていただいてからご覧ください。**
- **教育機関向け追加課題フォルダ**<br> RumiCarを教材として教育機関にて授業や演習で利用する場合の追加課題です。Rumicarハンズオンの資料はRumiCarの仕組みや構成を学べるように作られています。学生が自ら更に深く考えて課題を克服しより良いアルゴリズムを開発できるように研究課題の基礎やグループワークに使えるように追加の課題をまとめました。是非ご利用ください
- **開発用資料フォルダ**<br>　RumiCarの車体やコンピュータモジュールを自作する場合の開発用資料です。

1. RumiCarを自作する場合はまず「RumiCarの作り方」をご覧頂きRumiCarを作製後、作製したコンピュータモジュールとRumiCarが正しく作製できているかの確認のために「RumiCarの動作確認方法」の内容に従ってご確認ください。是非多くの人たちにオリジナルのRumiCarを作成頂きRumiCarの世界を楽しんで頂けると嬉しいです<br>1.1 コンピュータモジュールの作製においてArduino Nano以外のコンピュータモジュール作成のための情報を追加しました<br>1.2 ステアリングをサーボ化する場合のピンアサイン(13番ピン)を追加しました
1. RumiCarを受け取ったり運送等により故障していないかを確認する場合は「RumiCarの動作確認方法」の内容に従ってRumiCarが正常動作するかご確認ください
1. 各コンピュータモジュール別のピンアサイン表があります。プログラムの互換性確保のためコンピュータモジュールを自作する場合の結線はそれに従ってください
  - **結線図フォルダ**
    - 結線図(ArduinoNano).jpg RumiCarの基本、Arduino Nano版コンピュータモジュールの結線図です。RumiCarをこれから始める人向け
    - 結線図(Raspberry Pi Zero W).png ラズパイZeroW版コンピュータモジュールの結線図です。Pythonを使いたい方、画像認識、AI等挑戦されたい方(上級者向け)
    - 結線図RumiCarG3.1_ESP32.jpg ESP32版コンピュータモジュール結線図、WiFIやBluetooth接続、NN(ニューラルネットワーク)プログラム等に挑戦されたいかた(上級者向け)
    - 車体接続結線表.jpg RumiCar車体側の結線図。車体を自作されたい方はこのインターフェイスに合わせて作製ください
    - ピンアサインArduino-ESP32-RasPi-Obniz Rumicarの車体インターフェイスと、各コンピュータモジュールのArduino nano、ESP32、ラズパイ、Obnizのピンアサインを一つの表にまとめたものです
  - **RumiCarの作り方**<br>　RumiCarの車体の開発方法や各種コンピュータモジュールの作製方法が掲載されています。RumiCarの自作を考えられている方はこの資料を参考にぜひオリジナルのRumiCarを作製してみてください。作成したRumiCarは「RumiCarの動作確認方法.pdf」の内容に従って動作確認ができます。   
  - **RumiCarの動作確認方法**<br>　RumiCarを新規に作製した場合や、運送などにより故障が発生していないか、のためにRumICarが正常に動作するか確認するための手順をまとめた資料です。こちらを参考にRumiCarの動作確認をしましょう。
  - **RumiCar Youtube** RumiCarのYouTubeチャンネルです。RumiCarの紹介動画や各種走行動画、開発途中の各種テストの動画などが保存されています。ぜひチャンネル登録をお願いします<br>
  https://www.youtube.com/channel/UCVg3CBSVBcc_00FdC6q2wDg/videos?view_as=subscriber
  - **FaceBook** RumiCarのfaceBookグループです。皆様のご参加をおまちしてます<br>
  https://www.facebook.com/groups/rumicar
  
  - **connpass** RumiCarの走行会や開発教室、オンラインセミナーなどイベント告知や参加者募集のページです。RumiCarイベントへ参加ご希望の方はメンバー登録をお願いします<br> 
   https://rumicar.connpass.com/
   
Slides (資料)：https://www.slideshare.net/ssuser4d5ccc/rumicar-handsonalgyan20200425202004242

Video about online RumiCar hands-on event (RumiCarハンズオン中継の録画)：https://youtu.be/99zH73B8NUo
