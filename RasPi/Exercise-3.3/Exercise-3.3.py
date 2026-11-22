from RumiCar import *

# なめらかに走る車: 状況に分けて、「ずれ」の大きさでハンドルを切る
#   両側に壁 → 真ん中を走る / 右の壁だけ → 左へ切って離れる / 左の壁だけ → 右へ切って離れる
#   どちらにも壁が見えない → まっすぐ / センサーが応答しない (-1) → 止まる
# ハンドルの強さは走りながら自分で調整する (ずれが縮まらなければ 1.3 倍、目標を越えたら半分)
# Arduino 版 (RumiCar-lib v0.7.1 の Exercise-3.3) と同じ動き
# 実行: python3 ../RumiCar.py Exercise-3.3.py

# 仮の値。自分の車で変えるときは、1 つずつ変える
LIMIT_MM = 200          # RC_read が信じる距離の上限 [mm]。これより先は RC_read が -3 (範囲外) を返す
                        # RumiCar モデル 32 型は約 30 cm で床を捉えるので、200 mm は余裕を見た値
BAND_MM = 30            # 両側に壁があるとき、左右の差がこれ以下ならまっすぐ [mm]
                        # (センサーの誤差 1〜2 cm より大きくする)

ipower = 128            # ハンドルの強さ 0〜255 (半分から始める。走りながら自分で変わる。
                        # 1.3 倍・半分ずつ。どちらも目安)
last_side = CENTER      # 前回切った側
last_err = 0            # 前回のずれ [mm]
last_case = -1          # 前回の状況 (下を参照)


def setup():
    RC_setup()
    RC_setReadLimit(LIMIT_MM)   # LIMIT_MM より先は RC_read が -3 を返す


def loop():
    global ipower, last_side, last_err, last_case

    s0 = RC_read(LEFT)
    s1 = RC_read(CENTER)
    s2 = RC_read(RIGHT)

    # RC_read が返した値のまま表示する (-3 は範囲外で、距離ではない)
    print("Sensor0:", s0, " Sensor1:", s1, " Sensor2:", s2, " Power:", ipower)

    if s0 < 0 or s1 < 0 or s2 < 0:              # ステータス: 負の数は距離ではない
        if s0 == -1 or s1 == -1 or s2 == -1:    # -1: センサーが応答しない → 止まる
            RC_drive(BRAKE, 0)
            return
        # -3 (範囲外): その方向の LIMIT_MM より手前に測れた物が無い。
        # 以下では「その側に壁が見えない」という意味で使う

    # 速さ: 前 (中央) のセンサーで決める
    if s1 == -3:                # LIMIT_MM より手前に何も無い
        RC_drive(FORWARD, 255)
    elif s1 < 100:              # これらのしきい値は LIMIT_MM 以下にする
        RC_drive(REVERSE, 150)
    elif s1 < 150:
        RC_drive(FORWARD, 150)
    elif s1 < 200:
        RC_drive(FORWARD, 200)
    else:
        RC_drive(FORWARD, 255)

    # 向き: 状況ごとに、切る側とずれを決める
    see_left = (s0 >= 0)        # 左に壁が見える (RC_read が距離を返した)
    see_right = (s2 >= 0)
    # the_case: 状況、side: 切る側 (CENTER = まっすぐ)、err: ずれ [mm] (目標からどれだけ離れているか)
    if see_left and see_right:          # 両側に壁: 真ん中を走る
        the_case = 0
        err = abs(s0 - s2)
        if err <= BAND_MM:
            side = CENTER
        elif s0 > s2:
            side = LEFT
        else:
            side = RIGHT
    elif see_right:                     # 右の壁だけ: その壁から離れる
        the_case = 1
        err = LIMIT_MM - s2     # 壁が近いほど、ずれが大きい
        side = LEFT
    elif see_left:                      # 左の壁だけ: その壁から離れる
        the_case = 2
        err = LIMIT_MM - s0
        side = RIGHT
    else:                               # どちらにも壁が無い: まっすぐ
        the_case = 3
        err = 0
        side = CENTER
    # 見えない壁 (-3) がどれだけ遠いかは分からないので、その距離を推測しない:
    # 壁が片側だけのときは、見えている壁から離れるだけにする

    if the_case != last_case:
        # 状況が変わった (壁や柱が現れた・消えた)。この変化は自分の切り方の結果ではないので、
        # 前回と比べて強さを判断しない
        last_side = CENTER

    if side == CENTER:
        RC_steer(CENTER)
    else:
        if last_side != CENTER and side != last_side:
            ipower = ipower // 2            # 目標を越えた: 強すぎるので半分にする
        elif side == last_side and err >= last_err:
            ipower = ipower * 13 // 10      # ずれが縮まらない: 弱いので 1.3 倍にする
        ipower = max(40, min(ipower, 255))  # 1.3 倍でまた増やせるよう 40 以上に保つ
        RC_steer(side, ipower)
        last_side = side
    last_err = err
    last_case = the_case
