![RumiCarロゴ](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCarロゴ")
![RumiCar写真](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar写真")


# RumiCar

Please check a type of Computer Module (CM) and programming languages. If you use Arduino Nano or ESP32, please go to an "ArduinoAndESP32" folder. If you use Raspberry Pi Zero W, please go to a "RasPi" folder. Python is used for programs in "RasPi."

コンピュータモジュールの種類によりプログラムや使用コンピュータ言語が異なります。

Arduino Nano と ESP32 の場合はフォルダArduinoAndESP32をお使いください。
Raspberry Pi Zero W の場合はフォルダRasPiをお使いください。Raspiはコンピュータ言語はPythonです。

（YouTubeへのリンクです）

[![RumiCar紹介動画](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "RumiCar紹介動画")
[![RumiCarテスト走行動画](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCarテスト走行動画")
[![RumiCar車載カメラ視点動画](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "RumiCar車載カメラ視点動画")
### フォルダの説明

- **ArduinoAndESP32フォルダ**<br>　RumiCarのコンピュータモジュールのArduino Nano版とESP32版用のサンプルプルプグラムです。<br>ハンズオンで使用するプログラムが保存されています。RumiCarハンズ用の資料は「**ハンズオン用テキスト**」フォルダの下に保存されていますのでまずそちらを参照ください。**ハンズオン用テキストはPDF形式で約１０MBほどのファイルサイズです。GitHubから直接参照しようとすると表示できないことがあるようです。一旦ダウンロードしていただいてからご覧ください。**　RumiCarをお持ちの方は以下のフォルダにある各サンプルプログラムをRumiCarにロードしてお楽しみください。

  - **Exercise-1　距離を測ってみよう！**<br>　RumiCarでは車体の前方に3個のレーザー測距モジュールを搭載しています。レーザー測距モジュールは人体に無害のレーザー光線を対象物に当て、その反射が戻ってくるまでの***時間***を計測することによって距離を測定します。音速はマッハと呼ばれ時速1000km以上の速度ですが、光の速さはその音速の***88万倍***も速いです。その光が反射して返ってくるまでのとても短い時間を計って距離を計測する、なんて凄いですね。
  
    - **Exercise-1.1 中央のセンサで測距しよう！**<br>　RumiCarに3個搭載されているレーザー測距モジュールのうちの一つ、中央のセンサーを使って距離を測ってみます。Arduino IDEが持っている機能のシリアルモニタを使って皆さんのパソコンに測定した距離をリアルタイムに表示します。センサの前に手をかざし、手を前後に動かすと手とセンサの間の距離が変わりますのでシリアルモニタに表示される距離の値もリアルタイムに変化します。
    - **Exercise-1.2 3つのセンサで測距だ！**<br>　RumiCarに搭載されている3個のセンサを全部同時に動かしてみましょう。中央、左、右のそれぞれのセンサが測定した距離の値を同時にリアルタイムにパソコンの画面に表示します。それぞれのセンサの前に手をかざして画面の表示値が変化するのをお楽しみください。
    - **Exercise-1.3 シリアルプロッタを使ってみよう**<br>　Arduino IDEにはシリアルプロッタの機能もあります。これはデータを自動でグラフ表示してくれる機能です。プログラムはExercise-1.2と同じものを使いますが測定した距離値がリアルタイムに色分けされてグラフ化表示されます。Exercise-1.2と同じようにセンサの前に手をかざしてグラフが変化するか見てみましょう。どの色のグラフがどのセンサの値を表示しているか確かめましょう。このエクササイズはExercise-1.2のプログラムをそのまま使いますので専用のプログラムはありません。
  - **Exercise-2 モータ制御**<br>　RumiCarはハンドルを曲げる時と走行する時にモータを使います。RumiCarではコンピュータモジュールのコンピュータがプログラムに従ってハンドルを動かしたりRumiCarが走るためにタイヤを回転させたりします。コンピュータモジュールを使ってRumiCarのモータを動かしてみましょう。
    - **Exercise-2.1 ハンドルを切る**<br>　RumiCarの前のタイヤを左右に動かしてみましょう。このプログラムではハンドルを左右に切ってそのまま0.5秒保持します。その動作を繰り返します。つまり、左に切って0.5秒、右に切って0.5秒、また左に切って0.5秒と、ずっと繰り返します。
    - **Exercise-2.2 速度制御**<br>　RumiCarの走る速度をコントロールしてみましょう。コンピュータモジュールからの命令によって走行用車輪、すなわち後ろのタイヤを回転させます。タイヤの回転速度を変化させることによりRumiCarの走るスピードを変化させることができます。ここではプログラムを使ってタイヤの回転速度を段階的に変化させます。モータの回転音が変わりますのでタイヤの回転を見るのと同時にモータの音の変化にも気をつけてみてください。
    - **Exercise-2.3 前進と後進**<br>　本物の自動車は前に進んだり後ろに下がったりします。RumiCarも同様に前進や後進ができます。プログラムを使ってRumiCarを前進させたり後進させたりしてみましょう。RumiCarを走らせる場所が無いときはRumiCarを手にしっかり持って後ろのタイヤが前進方向に回転したり、後進方向に回転するのを確認しましょう。
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
   - **Exercise-3 自立走行基礎**
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
