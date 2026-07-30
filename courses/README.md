# RumiCar Simulator — コースカタログ

このフォルダは [RumiCar Simulator](https://www.rumicar.com/simulator/) のコースデータです。

## 構成
- `courses/*.json` — 公式テストコース (1 コース 1 ファイル, スペック形式)
- `courses/index.json` — 上記の索引 (file / name / kind / desc)
- `courses/community/*.json` — **利用者が投稿したコース** (正規化形式: walls/start/finish/bounds)

シミュレータは起動時に `courses/community/` を読み込み、コース選択メニューに
「🌐 名前」として追加します。

## コースを投稿する (PR)
1. シミュレータで「コース編集 ✎」からコースを作成
2. 「GitHubで共有」ボタンを押す → GitHub の新規ファイル作成画面が JSON 入りで開きます
3. そのままコミットするとプルリクエストが作成されます (書込権限が無い場合は自動で fork されます)
4. マージ後、全利用者のメニューに表示されます

## フォーマット (community)
```json
{
  "name": "コース名",
  "desc": "説明 (任意)",
  "bounds": { "w": 3.0, "h": 2.0 },
  "start": { "x": 0.4, "y": 0.3, "theta": 0 },
  "finish": { "x1": 0.4, "y1": 0.0, "x2": 0.4, "y2": 0.6 },
  "walls": [ { "x1": 0, "y1": 0, "x2": 3, "y2": 0 }, ... ]
}
```
壁は線分 (メートル単位) の配列です。RumiCar は左右の壁に追従するため、必ず
内外2本で囲まれた周回路にしてください。
