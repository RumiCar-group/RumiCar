# Wall Hugger FF (Python) — ノーマル FF 用  [卓上 Lv3 弱点を補う・Python 版]  by Fable 5.1
# 実測 (v12.1.0・完走が定義される卓上 56 コース・ソロ走行・laps=3・rejoin=OFF): 48/56 完走 (C 版 Steady Nose は 47/56)。
# FR 版「Wall Hugger」の土台 (先頭の LIMIT_MM と setup() の RC_setReadLimit で上限を決める・右の壁に沿う・前に壁が見えたら開いている方へ・見失ったら探す・詰まったら後退) はそのまま。
# ★補う弱点★ FF は前輪が駆動と操舵を兼ねるので、アクセルを踏んだまま曲げるとほとんど曲がらない
#   (実測の旋回半径: PWM 200 で走ってから全舵で切り、その PWM に切り替えたとき、240 で約 1.8 m・70 まで抜くと約 0.8 m。FR は 0.8 m → 0.45 m)。
#   ∴ (a) 曲げるときはアクセルを抜く (TURNV=70)。(b) 切り始めたら HOLD 回は切り続ける (切ったり戻したりでは曲がりきれない)。
#   (c) 右の壁には FR より近く寄る (NEAR/FARB を小さく)。大回りになる車ほど、曲がり始める位置を外側に取れないので、
#       壁の近くを走って前方の壁を早めに (遠い位置で) 見つける。(d) 直線の TOP も少し抑える (曲げる前に落とす速度が小さくて済む)。
# ※ RC_read の値の扱いは FR 版と同じ: まず 0 以上 (距離) か負の数 (ステータス) かで分け、-1 (センサーが応答しない) は止まり、
#   -3 (範囲外) は比べる前に FAR_MM へ置き換える (FAR_MM は大小の比較にだけ使う。左右の差は両側とも距離のときだけ)。

from RumiCar import *

LIMIT_MM = 200  # RC_read の値を信じる距離の上限 [mm]。これより先は RC_read が -3 (範囲外) を返す。上限を変えるときはこの 1 行を直す
FAR_MM = 9999   # 範囲外 (-3) を「遠い」として比べるための値。RC_read が返すどの距離 (卓上は最大 2000 mm) よりも大きい＝上限を変えても直さなくてよい
TOP = 220       # 速度: 壁に沿って安定しているとき (255 = 全開)
CRUISE = 180    # 速度: 壁を探す・寄せる・真ん中へ寄せるとき
TURNV = 70      # 速度: 前に壁が見えて曲げるとき (FF は抜かないと曲がらない)
BACKV = 150     # 速度: 後退
D_TURN = 200    # 前方がこれより近ければ曲げる [mm] (LIMIT_MM 以下にする。LIMIT_MM と同じ値なら「見えたら曲げる」)
NEAR = 85       # 右の壁がこれより近ければ左へ [mm] (FR より壁に寄る)
FARB = 150      # 右の壁がこれより遠ければ右へ [mm]
ESC = 80        # 前方がこれより近ければ後退して切り返す [mm]
BACKT = 8       # 後退を続ける回数 (loop の回数。20 Hz なら 0.4 秒)
SEEK = 4        # 壁を見失ってから全舵で右へ切り続ける回数。その後は半分の時間だけ切る
HOLD = 2        # (b) 前の壁で切り始めてから、壁が見えなくなっても切り続ける回数

back = 0        # 後退の残り回数
bdir = CENTER   # 後退中の舵
lost = 0        # 右の壁を見失ってからの回数
hold = 0        # 切り続ける残り回数
hdir = CENTER   # 切り続ける向き


def setup():
    global back, lost, hold
    RC_setup()
    RC_setReadLimit(LIMIT_MM)   # LIMIT_MM より先は RC_read が -3 (範囲外) を返す
    back = 0
    lost = 0
    hold = 0


def loop():
    global back, bdir, lost, hold, hdir
    L = RC_read(LEFT)
    C = RC_read(CENTER)
    R = RC_read(RIGHT)
    if L < 0 or C < 0 or R < 0:
        # ステータス (負の数は距離ではない): ステータスの処理はこのブロックにまとめる
        if L == -1 or C == -1 or R == -1:
            RC_drive(BRAKE, 0)  # -1: センサーが応答しない → 止まる (この回は判断しない)
            return
        if L == -3:             # -3: 範囲外 (上限より手前に測れた物が無い) を「遠い」として比べる
            L = FAR_MM
        if C == -3:
            C = FAR_MM
        if R == -3:
            R = FAR_MM
    # ここから先の L・C・R は距離 [mm] (範囲外は FAR_MM)

    # (4) 後退中: 舵を固定したまま下がる (毎回選び直すと左右にぶれて同じ場所を往復する)
    if back > 0:
        back = back - 1
        RC_steer(bdir)
        RC_drive(REVERSE, BACKV)
        return
    if C < ESC:
        back = BACKT
        hold = 0
        if L < 120 or R < 120:
            bdir = CENTER      # 横の壁が近いときは真っすぐ下がる (逆ハンだと後ろが壁に当たる)
        elif L > R:
            bdir = RIGHT       # 左が開いている → 右に切って下がると鼻先が左を向く
        else:
            bdir = LEFT
        RC_steer(bdir)
        RC_drive(REVERSE, BACKV)
        return

    # (1) 前に壁: 開いている方へ。(b) 見えなくなっても HOLD 回は同じ向きに切り続ける
    if C < D_TURN:
        if R > L:
            hdir = RIGHT
        else:
            hdir = LEFT
        hold = HOLD
        RC_steer(hdir)
        RC_drive(FORWARD, TURNV)
        return
    if hold > 0:
        hold = hold - 1
        RC_steer(hdir)
        RC_drive(FORWARD, TURNV)
        return

    # 両側に壁が見える狭い通路: 左右の差で真ん中へ (差を使うのは、両側とも距離のときだけ)
    if L < FAR_MM and R < FAR_MM:
        lost = 0
        if L + 20 < R:
            RC_steer(RIGHT)
        elif R + 20 < L:
            RC_steer(LEFT)
        else:
            RC_steer(CENTER)
        RC_drive(FORWARD, CRUISE)
        return

    # (3) 右の壁が見えない: 右へ切って探す
    if R == FAR_MM:
        lost = lost + 1
        if lost <= SEEK or lost % 2 == 0:
            RC_steer(RIGHT)
        else:
            RC_steer(CENTER)
        RC_drive(FORWARD, CRUISE)
        return

    # (2) 右の壁に沿う
    lost = 0
    if R < NEAR:
        RC_steer(LEFT)
        RC_drive(FORWARD, CRUISE)
    elif R > FARB:
        RC_steer(RIGHT)
        RC_drive(FORWARD, CRUISE)
    else:
        RC_steer(CENTER)
        RC_drive(FORWARD, TOP)
