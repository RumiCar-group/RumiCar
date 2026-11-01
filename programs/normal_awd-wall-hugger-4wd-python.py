# Wall Hugger 4WD (Python) — ノーマル 4WD 用  [卓上 Lv2 強みを使う・Python 版]  by Fable 5.1
# 実測 (v11.0.0・完走が定義される卓上 56 コース・ソロ走行・laps=3・rejoin=OFF): 53/56 完走 (C 版 Traction Blitz は 48/56)。
# FR 版「Wall Hugger」の土台 (右の壁に沿う・前に壁が見えたら開いている方へ・見失ったら探す・詰まったら後退) はそのまま。
# ★引き出すロジック★ 4WD は 4 輪で駆動するので、壁に沿って安定したら迷わず TOP まで加速してよい (トラクションがある)。
#   弱点は「アクセルを踏んだまま曲げると前輪が押されて大回りになる」こと (実測の旋回半径: PWM 200 で走ってから全舵で切り、その PWM に
#   切り替えたとき、240 で約 1.3 m・70 まで抜くと約 0.7 m。FR は 0.8 m → 0.45 m)。∴ 曲げるときはアクセルを強く抜く (TURNV=70) ことで FR と同じ通路を通れる。
# ★足したロジック★ (5) 切り始めたら HOLD 回は切り続ける。前方の扇は壁の端で見えたり見えなかったりするので、
#   切ったり戻したりを繰り返すと曲がりきれない。4WD は切り始めてから曲がり始めるまでが遅いので、この「粘り」が効く。
# ※ -3 は数としては最も小さいので、比較の前に FAR (999) へ置き換える (RasPi・Zero2Cam・Arduino でも同じ書き方)。
#   -1 (読めない) は置き換えずそのまま比較に入る＝「近い」扱いで減速・後退する (安全側)。

from RumiCar import *

TOP = 240       # 速度: 壁に沿って安定しているとき (255 = 全開)
CRUISE = 180    # 速度: 壁を探す・寄せる・真ん中へ寄せるとき
TURNV = 70      # 速度: 前に壁が見えて曲げるとき (4WD は強く抜くほど小さく回れる)
BACKV = 150     # 速度: 後退
D_TURN = 200    # 前方がこれより近ければ曲げる [mm] (20 cm = RC_read が距離を返す上限なので「見えたら曲げる」)
NEAR = 100      # 右の壁がこれより近ければ左へ [mm]
FARB = 170      # 右の壁がこれより遠ければ右へ [mm]
ESC = 80        # 前方がこれより近ければ後退して切り返す [mm]
BACKT = 6       # 後退を続ける回数 (loop の回数。20 Hz なら 0.3 秒)
SEEK = 4        # 壁を見失ってから全舵で右へ切り続ける回数。その後は半分の時間だけ切る
HOLD = 2        # (5) 前の壁で切り始めてから、壁が見えなくなっても切り続ける回数
FAR = 999       # -3 (20 cm 以内に何もない) を比較で「遠い」として扱う値

back = 0        # 後退の残り回数
bdir = CENTER   # 後退中の舵
lost = 0        # 右の壁を見失ってからの回数
hold = 0        # 切り続ける残り回数
hdir = CENTER   # 切り続ける向き


def setup():
    global back, lost, hold
    RC_setup()
    back = 0
    lost = 0
    hold = 0


def loop():
    global back, bdir, lost, hold, hdir
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

    # (1) 前に壁: 開いている方へ。(5) 見えなくなっても HOLD 回は同じ向きに切り続ける
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
