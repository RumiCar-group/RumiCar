# Wall Hugger (Python) — ノーマル FR 用  [卓上 Lv1 基準・Python 版]  by Fable 5.1
# 実測 (v11.0.0・完走が定義される卓上 56 コース・ソロ走行・laps=3・rejoin=OFF): 50/56 完走 (C 版 Apex Hunter は 47/56)。
# ★学習の出発点★ RumiCar の 3 つの ToF (左 +65°・中央・右 −65°) は 20 cm より先を見ません (RC_read は -3 を返す)。
#   卓上コースの通路は 20 cm より広いので、真ん中を走ると左右とも -3 になり壁が見えません。
#   そこで「右の壁に寄り、壁に沿って走る」のがこのプログラムの基本です (両側が見える狭い通路では左右の差で真ん中へ)。
# 土台ロジック (4 つ):
#   (1) 前方 C に壁が見えたら (20 cm 以内)、左右で「開いている方」へ切る。-3 (何もない) が最も開いている
#   (2) 右の壁との距離を NEAR〜FARB に保つ (近ければ左へ・遠ければ右へ・ちょうどなら直進で全開)
#   (3) 右の壁を見失ったら右へ切って探す。長く見えないときは半分の時間だけ切って大回り (同じ場所で円を描き続けない)
#   (4) 前が ESC より近づいたら、開いている側へ鼻先が向くよう逆ハンで後退して切り返す (行き止まり脱出)
# ※ FR は駆動 (後輪) と操舵 (前輪) が分かれるので素直に曲がる。曲げるときの速度 TURNV は全開の約 4 割 (110/255) で
#   旋回半径が小さくなる (全開に近いほどアンダーステアで大回りになる)。
# ※ -3 は数としては最も小さいので、比較の前に FAR (999) へ置き換える (RasPi・Zero2Cam・Arduino でも同じ書き方)。

from RumiCar import *

TOP = 240       # 速度: 壁に沿って安定しているとき (255 = 全開)
CRUISE = 180    # 速度: 壁を探す・寄せる・真ん中へ寄せるとき
TURNV = 110     # 速度: 前に壁が見えて曲げるとき (FR は 110 前後で最も小さく回れる)
BACKV = 150     # 速度: 後退
D_TURN = 200    # 前方がこれより近ければ曲げる [mm] (20 cm = RC_read が距離を返す上限なので「見えたら曲げる」)
NEAR = 100      # 右の壁がこれより近ければ左へ [mm]
FARB = 170      # 右の壁がこれより遠ければ右へ [mm]
ESC = 80        # 前方がこれより近ければ後退して切り返す [mm]
BACKT = 8       # 後退を続ける回数 (loop の回数。20 Hz なら 0.4 秒)
SEEK = 4        # 壁を見失ってから全舵で右へ切り続ける回数。その後は半分の時間だけ切る
FAR = 999       # -3 (20 cm 以内に何もない) を比較で「遠い」として扱う値

back = 0        # 後退の残り回数
bdir = CENTER   # 後退中の舵
lost = 0        # 右の壁を見失ってからの回数


def setup():
    global back, lost
    RC_setup()
    back = 0
    lost = 0


def loop():
    global back, bdir, lost
    L = RC_read(LEFT)
    C = RC_read(CENTER)
    R = RC_read(RIGHT)
    if L == -3:
        L = FAR
    if C == -3:
        C = FAR
    if R == -3:
        R = FAR

    # (4) 後退中: 舵を固定したまま下がる (毎回選び直すと左右にぶれて同じ場所を往復する)
    if back > 0:
        back = back - 1
        RC_steer(bdir)
        RC_drive(REVERSE, BACKV)
        return
    if C < ESC:
        back = BACKT
        if L < 120 or R < 120:
            bdir = CENTER      # 横の壁が近いときは真っすぐ下がる (逆ハンだと後ろが壁に当たる)
        elif L > R:
            bdir = RIGHT       # 左が開いている → 右に切って下がると鼻先が左を向く
        else:
            bdir = LEFT
        RC_steer(bdir)
        RC_drive(REVERSE, BACKV)
        return

    # (1) 前に壁: 開いている方へ
    if C < D_TURN:
        if R > L:
            RC_steer(RIGHT)
        else:
            RC_steer(LEFT)
        RC_drive(FORWARD, TURNV)
        return

    # 両側に壁が見える狭い通路: 左右の差で真ん中へ
    if L < FAR and R < FAR:
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
    if R == FAR:
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
