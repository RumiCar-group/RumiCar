# RumiCarライブラリ（このディレクトリのローカル版は廃止されました）

> [!IMPORTANT]
> ## 📢 このディレクトリのライブラリは削除されました / The local library here has been removed
>
> **RumiCarライブラリはArduino公式のLibrary Managerに登録されています。**
> このディレクトリにあった `RumiCar.zip` と `RumiCar/` フォルダ（ローカル版ライブラリ）、および
> 旧手順の `How to install RumiCar library.pdf` は、予告どおり**削除しました**。
> 古いライブラリを誤って使わないよう、以下の方法でインストールしてください。
>
> ### インストール手順（Arduino IDE 内で完結します）
>
> 1. Arduino IDEのメニューから「スケッチ」→「ライブラリをインクルード」→「ライブラリを管理...」を開く
> 2. 検索ボックスに `RumiCar` と入力
> 3. 表示された `RumiCar by RumiCar-group` を選び「インストール」をクリック
> 4. 依存ライブラリ（VL53L0X）のインストール確認が表示されたら「Install all」を選択
>
> ライブラリのソースコード（後継の正式版）: **[RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib)**
>
> ⚠️ 古い資料（ハンズオン用PDFなど）に「本ディレクトリの RumiCar.zip を選択」という手順が
> 残っている場合がありますが、**その手順はもう使えません。** 上記の Library Manager をご利用ください。
> 旧ローカル版には後方センサー（`REAR`）などの新機能が入っていないため、そのまま使うと
> 最新のExerciseが動作しません。
>
> ---
>
> **The RumiCar library is published in the Arduino Library Manager.**
> The `RumiCar.zip`, the `RumiCar/` folder (the local copy of the library) and the outdated
> `How to install RumiCar library.pdf` that used to live in this directory **have been removed**,
> as previously announced, so that nobody installs the outdated library by mistake.
>
> ### Installation steps (all done inside Arduino IDE)
>
> 1. Open **Sketch → Include Library → Manage Libraries...** in Arduino IDE
> 2. Type `RumiCar` in the search box
> 3. Click **Install** on the `RumiCar by RumiCar-group` entry
> 4. When prompted to install the dependency (VL53L0X), choose **Install all**
>
> Library source code (the official successor): **[RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib)**
>
> ⚠️ Some older materials (hands-on PDFs and the like) still say "select the RumiCar.zip in this
> directory". **That procedure no longer works** — please use the Library Manager above. The old
> local copy also lacks newer features such as the rear sensor (`REAR`), so recent Exercises will
> not work with it.

> [!NOTE]
> このREADMEには以前，「[Arduinoパッケージとして公式に登録＆利用できる](https://lang-ship.com/blog/work/arduino-ide-add-library-manager/)ようになればより嬉しくなりそうですが，有識者の皆さまに期待します^^;」と記載されていました．**2026年5月，その願いが実現しました．** 🎉

## 使用方法

ご自身の.inoファイルの先頭に，

```cpp
#include <RumiCar.h>
```

と記述してください．

スケッチ > ライブラリをインクルード > RumiCar を選択することもできます．

## 注意事項

- [VL53L0X library for Arduino](https://github.com/pololu/vl53l0x-arduino)のインストールは必要です．
  Library Manager経由でインストールする場合は，依存関係として自動的にインストールするか確認されます．
- ライブラリ本体の不具合報告・改善提案は後継リポジトリ [RumiCar-group/RumiCar-lib](https://github.com/RumiCar-group/RumiCar-lib) へお願いします．
