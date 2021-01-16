![RumiCarロゴ](https://www.rumicar.com/wp-content/uploads/2020/08/IMG_0184.png "RumiCarロゴ")
![RumiCar写真](https://www.rumicar.com/wp-content/uploads/2020/05/rumicar_logo.jpg "RumiCar写真")

# RumiCar

**If you prefer English, please read** [**Readme English**](https://github.com/RumiCar-group/RumiCar/blob/master/Readme%20English.md).

コンピュータモジュールの種類によりプログラムや使用コンピュータ言語が異なります。
Arduino Nano と ESP32 の場合はフォルダArduinoAndESP32をお使いください。
Raspberry Pi Zero W の場合はフォルダRasPiをお使いください。Raspiはコンピュータ言語はPythonです。

画像はYouTubeへのリンクです。クリックしてご覧ください。

- [RumiCar紹介動画](https://youtu.be/DxaY2eCzJzo)
- [衝突回避](https://youtu.be/95pc_4Wf14U)
- [Part 2 of 2 RumiCar on test course](https://youtu.be/oHujTh9AwAw)
- [車載カメラ視点](https://youtu.be/16kOgLMo-Tg)

[![RumiCar紹介動画](http://img.youtube.com/vi/DxaY2eCzJzo/0.jpg)](https://www.youtube.com/watch?v=DxaY2eCzJzo "RumiCar紹介動画")
[![RumiCar衝突回避動画](http://img.youtube.com/vi/95pc_4Wf14U/0.jpg)](https://youtu.be/95pc_4Wf14U "RumiCar衝突回避動画")
[![RumiCarテスト走行動画](http://img.youtube.com/vi/oHujTh9AwAw/0.jpg)](https://www.youtube.com/watch?v=oHujTh9AwAw "RumiCarテスト走行動画")
[![RumiCar車載カメラ視点動画](http://img.youtube.com/vi/16kOgLMo-Tg/0.jpg)](https://www.youtube.com/watch?v=16kOgLMo-Tg "RumiCar車載カメラ視点動画")
### フォルダの説明

- [**ArduinoAndESP32**](https://github.com/RumiCar-group/RumiCar/tree/master/ArduinoAndESP32)フォルダ<br>RumiCarのコンピュータモジュール(CM)のArduino Nano版とESP32版用のサンプルプルプグラムです。<br>ハンズオンで使用するプログラムが保存されています。RumiCarハンズ用の資料は[**ハンズオン用テキスト**](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88)フォルダの下に保存されていますのでまずそちらを参照ください。

  - **Exercise-1 距離を測ってみよう！**<br>RumiCarには車体の前方に3個のレーザー測距モジュールを搭載しています。レーザー測距モジュールは人体に無害のレーザー光線を対象物に当て、その反射光が戻ってくるまでの***時間***を計測することによって距離を測定します。音速はマッハと呼ばれ時速1000km以上の速度ですが、光の速さはその音速の***88万倍***も速いです。その光が反射して返ってくるまでのとても短い時間を計って距離を計測するなんて凄いですね。
  
    - **Exercise-1.1 中央のセンサで測距しよう！**<br>RumiCarに3個搭載されているレーザー測距モジュールのうちの一つ、中央のセンサーを使って距離を測ってみます。Arduino IDEが持っているシリアルモニタの機能を使い、測定した距離を皆さんのパソコンにリアルタイムで表示します。センサの前に手をかざして手を前後に動かすと、手とセンサの間の距離が変わりますのでシリアルモニタに表示される距離の値もリアルタイムで変化します。
    - **Exercise-1.2 3つのセンサで測距だ！**<br>RumiCarに搭載されている3個のセンサを全部同時に動かしてみましょう。左、中央、右のそれぞれのセンサが測定した距離の値を同時にリアルタイムでパソコンの画面に表示します。
    - **Exercise-1.3 シリアルプロッタを使ってみよう**<br>Arduino IDEにはシリアルプロッタの機能もあります。これはデータを自動でグラフ表示してくれる機能です。プログラムはExercise-1.2と同じものを使いますが測定した距離値がリアルタイムに色分けされてグラフ化表示されます。Exercise-1.2と同じようにセンサの前に手をかざしてグラフが変化するか見てみましょう。どの色のグラフがどのセンサの値を表示しているか確かめましょう。このExerciseではExercise-1.2のプログラムをそのまま使いますので専用のプログラムはありません。
  - **Exercise-2 モータ制御**<br>RumiCarはハンドルを曲げる時と走行する時にモータを使います。RumiCarではCMのコンピュータがプログラムに従って、ハンドルを動かしたりRumiCarが走るためにタイヤを回転させたりします。コンピュータモジュールを使ってRumiCarのモータを動かしてみましょう。
    - **Exercise-2.1 ハンドルを切る**<br>RumiCarの前のタイヤを左右に動かしてみましょう。このプログラムではハンドルを左右に切ってそのまま0.5秒保持します。その動作を繰り返します。つまり、左に切って0.5秒、右に切って0.5秒、また左に切って0.5秒と、ずっと繰り返します。
    - **Exercise-2.2 速度制御**<br>RumiCarの走る速度をコントロールしてみましょう。コンピュータモジュールからの命令によって走行用車輪、すなわち後ろのタイヤを回転させます。タイヤの回転速度を変化させることによりRumiCarの走るスピードを変化させることができます。ここではプログラムを使ってタイヤの回転速度を段階的に変化させます。モータの回転音が変わりますのでタイヤの回転を見るのと同時にモータの音の変化にも気をつけてみてください。
    - **Exercise-2.3 前進と後進**<br>実際の自動車は前に進んだり後ろに下がったりします。RumiCarも同様に前進や後進ができます。プログラムを使ってRumiCarを前進させたり後進させてみましょう。RumiCarを走らせる場所が無い時は、RumiCarを手にしっかり持って後ろのタイヤが前進方向に回転したり、後進方向に回転するのを確認しましょう。
    - **Exercise-2.4 ジグザグ走行**<br>RumiCarをジグザグに走行させてみましょう。ハンドルを左右に切りながら前進します。RumiCarを走らせられる場所が無い場合はRumiCarをしっかり手に持って期待通りの動きになっていることを確認しましょう。
  - **Exercise-3 自律走行基礎**<br>自律走行の基礎としてセンサ等が取得した情報を元に判断してRumiCarを制御してみます。前方の障害物や左右の壁などの距離を測定し、停止したり左右にハンドルを切りながらRumiCarを走行させてみます。
    - **Exercise-3.1 安全に停止する車**<br>前方に障害物を発見して安全に停止する車を考えます。RumiCarが直進中に前方に障害物を発見したら停止し、障害物が無くなったら走行そ再開します。障害物が近づいてきたら、安全のために後進します。あなたの手を障害物と仮定してRumiCarがあなたの手にぶつからずに停止するか試してみましょう。RumiCarが停止中にあなたの手をRumiCarに近づけたら安全確保のためにRumiCarが後進しますか？
    - **Exercise-3.2 市街地を走る車**<br>このExerciseでは実際に走行コースを設置してRumiCarを走らせます。左右の壁や前方の障害物を検知しながらRumiCarが走行します。
- **[ESP32](https://github.com/RumiCar-group/RumiCar/tree/master/ESP32)フォルダ**<br>RumiCarのCMのうち、ESP32はWiFiやBluetoothなどの通信機能を内蔵しています。ここではESP32のBluetooth機能を使ってRumiCarのセンサ測距値を無線で取得して表示させたり、RumiCarをワイヤレスで制御するサンプルプログラムが入っています。
     - **BLEフォルダ**<BR>Bluetooth Low Energy(BLE)での通信。RumiCarのESP32版CMに搭載されているBLEを使ってスマホ(iPhone)と通信してみましょう。3個の測距モジュールでの距離計測値をiPhoneのBLE Scannerとのアプリで表示させています。マルチコネクトが可能なので、個別の機器と接続や切断が不要ですので、一度に複数の機器から1台のRumiCarに接続することができます。展示会などで来場者に各自のスマホからRumiCarに接続してもらい、自分のスマホで計測値を表示させて楽しんでいただくことなどが可能です。
    - **Bluetoothフォルダ**<br>Exercise-1.2はRumiCar ESP32版CM用Bluetoothシリアル通信のサンプルプログラムです。RumiCarをWindows PCとBluetoothで接続してRumiCarのレーザー測距モジュールの測定距離値をパソコンに表示したり、パソコンからコマンドを送ってRumiCarを操作することができます。
      - Windos10とペアリング
      - 設定->デバイス->Bluetoothとその他のデバイス->Bluetoothまたはその他のデバイスを追加する->Bluetooth
      - RumiCar_ESP32を選択
      - デバイスマネージャー->ポート(COMとLPT)で"Bluetooth リンク経由の標準シリアル(COM #)"　を確認する。 #は数字です。　(2個表示される)
      - Arduino IDEの設定でツール->シリアルポート　で、上記COM #に合わせる
      - ESP32が「ケーブルを繋いでいない」のに測距値がシリアルモニタに表示できます。
      - PC側からコマンドを送ってRumiCarをコントロールできます。
      - サンプルプログラムはコマンド「r」、「l」、「f」、「b」に対応しています。それぞれ右、左、前進、後進を0.5秒実行します。コマンドは全て小文字です。 Arduino IDEのシリアルモニタの送信でコマンドを送ります。
      - ケーブル接続が不要なので、走行中に実際の測距データをPC側で取得したり、PC側からコマンドを送ってラジコンのような操作をすることができるようになります。

- **[RasPi](https://github.com/RumiCar-group/RumiCar/tree/master/RasPi)フォルダ**<br>RumiCarのコンピュータモジュールのラズパイ版のサンプルプログラムです。Pythonで記述されています。ラズパイ版CMを使ってRumiCarを動かす場合にお使いください。
  - **Exercise-1 距離を測ってみよう！**
    - **Exercise-1.1 中央のセンサで測距しよう！**
    - **Exercise-1.2 3つのセンサで測距だ！**   
  - **Exercise-2 モータ制御**   
    - **Exercise-2.1 ハンドルを切る**    
    - **Exercise-2.2 速度制御**
    - **Exercise-2.3 前進と後進**
    - **Exercise-2.4 ジグザグ走行**  
   - **Exercise-3 自律走行基礎**
     - **Exercise-3.1 安全に停止する車**   
     - **Exercise-3.2 市街地を走る車**
 - **[ハンズオン用テキスト](https://github.com/RumiCar-group/RumiCar/tree/master/%E3%83%8F%E3%83%B3%E3%82%BA%E3%82%AA%E3%83%B3%E7%94%A8%E3%83%86%E3%82%AD%E3%82%B9%E3%83%88)フォルダ**<br>RumiCarのハンズオン用テキストが保存されているフォルダです。RumiCarの歴史や仕組み、サンプルプログラムを使ってRumiCarを動作させる方法や例が載っています。ハンズオン用テキストはPDF形式で約10MBほどのファイルサイズです。GitHubから直接参照しようとすると表示できないことがあるようです。一旦ダウンロードしていただいてからご覧ください。
- **[教育機関向け追加課題](https://github.com/RumiCar-group/RumiCar/tree/master/%E6%95%99%E8%82%B2%E6%A9%9F%E9%96%A2%E5%90%91%E3%81%91%E8%BF%BD%E5%8A%A0%E8%AA%B2%E9%A1%8C)フォルダ**<br>RumiCarを教材として教育機関にて授業や演習で利用する場合の追加課題です。Rumicarハンズオンの資料はRumiCarの仕組みや構成を学べるように作られています。学生が自ら更に深く考えて課題を克服し、より良いアルゴリズムを開発できるように研究課題の基礎やグループワークに使えるように追加の課題をまとめました。是非ご利用ください。
- **[開発用資料](https://github.com/RumiCar-group/RumiCar/tree/master/%E9%96%8B%E7%99%BA%E7%94%A8%E8%B3%87%E6%96%99)フォルダ**<br>RumiCarの車体やCMを自作する場合の開発用資料です。

1. RumiCarを自作する場合はまず「RumiCarの作り方」をご覧いただき、RumiCarを作製後、作製したCMとRumiCarが正しく動作できているかの確認のために、「RumiCarの動作確認方法」の内容に従ってご確認ください。是非多くの人たちにオリジナルのRumiCarを作製いただき、RumiCarの世界を楽しんで頂けると嬉しいです<br>1.1 CMの作製においてArduino Nano以外のCM作製のための情報を追加しました。<br>1.2 ステアリングをサーボ化する場合のピンアサイン(13番ピン)を追加しました。
1. RumiCarを受け取ったり、運送等により故障していないかを確認する場合は「RumiCarの動作確認方法」の内容に従ってRumiCarが正常動作するかご確認ください。
1. 各CM別のピンアサイン表があります。プログラムの互換性確保のためCMを自作する場合の結線はそれに従ってください。
  - **結線図フォルダ**
    - 結線図(ArduinoNano).jpg<br>RumiCarの基本である、Arduino Nano版CMの結線図です。RumiCarをこれから始める人向け。
    - 結線図(Raspberry Pi Zero W).png<br>ラズパイZero W版CMの結線図です。Pythonを使いたい方、画像認識、AI等挑戦されたい方向け(上級者向け)。
    - 結線図RumiCarG3.1_ESP32.jpg<br>ESP32版CM結線図です。WiFiやBluetooth接続、ニューラルネットワーク(NN)プログラム等に挑戦されたい方向け(上級者向け)。
    - 車体接続結線表.jpg<br>RumiCar車体側の結線図です。車体を自作されたい方は、このインターフェイスに合わせて作製ください。
    - ピンアサインArduino-ESP32-RasPi-Obniz<br>Rumicar車体のインターフェイスと、各CMのArduino nano、ESP32、ラズパイ、Obnizのピンアサインを一つの表にまとめたものです。
  - **RumiCarの作り方**<br>RumiCar車体の開発方法や各種CMの作製方法が掲載されています。RumiCarの自作を考えられている方は、この資料を参考に是非オリジナルのRumiCarを作製してみてください。作製したRumiCarは「RumiCarの動作確認方法.pdf」の内容に従って動作確認ができます。   
  - **RumiCarの動作確認方法**<br>RumiCarを新規に作製した場合や、運送などにより故障が発生していないか、のためにRumiCarが正常に動作するか確認するための手順をまとめた資料です。こちらを参考にRumiCarの動作確認をしましょう。

  ### その他大事なこと
  - [**RumiCar website**](https://www.rumicar.com)<br>RumiCarのwebisteです。今後このwebisteを発展させたいので、websiteの編集をしてくださる方は是非ご連絡ください!
  - [**RumiCar YouTube**](https://www.youtube.com/channel/UCVg3CBSVBcc_00FdC6q2wDg)<br>RumiCarのYouTubeチャンネルです。RumiCarの紹介動画や各種走行動画、開発途中の各種テストの動画などがアップロードされています。是非チャンネル登録をお願いします。また、動画編集をしてくださる方も是非ご連絡ください。
  - [**RumiCar group**](https://www.facebook.com/groups/rumicar)<br>RumiCarのFaceBookグループです。皆様のご参加をお待ちしてます。
  - [**connpass**](https://rumicar.connpass.com)<br>RumiCarの走行会や開発教室、オンラインセミナーなどイベント告知や参加者募集のページです。RumiCarイベントに参加ご希望の方は是非メンバー登録をお願いいたします。
  - [**RumiCarハンズオン中継の録画**](https://youtu.be/99zH73B8NUo)<br>ALGYANとコラボしたRumiCarイベントのアーカイブです。イベント内でExerciseの解説やデモをしています。
