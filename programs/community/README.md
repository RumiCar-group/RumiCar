# programs/community — 利用者が投稿した走行プログラム

[RumiCar Simulator](https://cpo.tokyo/rumicar/) で改変したプログラムを **別名で** ここに
投稿する場所です。シミュレータは起動時にこのフォルダを読み込み、各車の「走行」メニューの
**「🌐 みんなの投稿」** に並べます。誰でも選んで走らせ、再利用できます。

## 投稿のしかた
1. シミュレータでプログラムを編集し、走らせて納得いくまで調整する
2. その車の「🌐保存」ボタンを押し、保存名を入力する
3. GitHub の新規ファイル作成画面が開くので、そのままコミット → プルリクエストが作成されます
   (書込権限が無い場合は自動で fork されます)
4. マージされると、全利用者の「🌐 みんなの投稿」に並びます

## ルール (ゆるく)
- **上書きせず別名で**追加してください (同名でもファイルを増やせます)
- 不要になったファイルの**削除は GitHub 上**で行います
- 実車でも動くよう Arduino C++ (.ino) 推奨。Python (.py) / JavaScript (.js) も可
- 走行 API: `RC_setup()` / `RC_drive(FORWARD, pwm)` / `RC_steer(LEFT/CENTER/RIGHT)` /
  `sensor0/1/2.readRangeSingleMillimeters()`
