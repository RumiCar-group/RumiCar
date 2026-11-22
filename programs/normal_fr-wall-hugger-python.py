# Wall Hugger (Python) — ノーマル FR 用  [卓上 Lv1 基準・Python 版]  by Fable 5.1
# 実測 (v12.1.0・完走が定義される卓上 56 コース・ソロ走行・laps=3・rejoin=OFF): 50/56 完走 (C 版 Apex Hunter は 47/56)。
# ★学習の出発点★ 3 つの ToF (左 +65°・中央・右 −65°) の値をどこまで信じるかは、プログラムが決めます。先頭の定数 LIMIT_MM に
#   「上限」を書き、setup() の RC_setReadLimit(LIMIT_MM) で決めると、RC_read は上限より先を -3 (範囲外) で返します。実機の ToF は
#   取り付け位置によって、ある距離から先で床を拾うためです (値の決め方は物理モデル解説 docs/physics_model.md §12「上限の決め方」)。
#   卓上コースの通路はこの上限より広いので、真ん中を走ると左右とも -3 になり壁が見えません。
#   そこで「右の壁に寄り、壁に沿って走る」のがこのプログラムの基本です (両側が見える狭い通路では左右の差で真ん中へ)。
# 土台ロジック (4 つ):
#   (1) 前方 C に壁が見えたら (D_TURN より近い)、左右で「開いている方」へ切る。-3 (範囲外) の側を最も開いているとみなす
#   (2) 右の壁との距離を NEAR〜FARB に保つ (近ければ左へ・遠ければ右へ・ちょうどなら直進で全開)
#   (3) 右の壁を見失ったら右へ切って探す。長く見えないときは半分の時間だけ切って大回り (同じ場所で円を描き続けない)
#   (4) 前が ESC より近づいたら、開いている側へ鼻先が向くよう逆ハンで後退して切り返す (行き止まり脱出)
# ※ FR は駆動 (後輪) と操舵 (前輪) が分かれるので素直に曲がる。曲げるときの速度 TURNV は全開の約 4 割 (110/255) で
#   旋回半径が小さくなる (アクセルを開けるほどアンダーステアで大回りになる。実測: PWM 200 で走ってから全舵で切り、
#   その PWM に切り替えたときの旋回半径は 240 で約 0.8 m・70 で約 0.45 m)。
# ※ RC_read の値は、まず 0 以上 (距離) か負の数 (ステータス) かで分け、ステータスの処理は loop() の最初の 1 つのブロックにまとめる
#   (RumiCar-lib v0.7.1 のスケッチ例・RasPi の Exercise-3.1〜3.3 と同じ書き方)。-1 (センサーが応答しない) は止まる。-3 (範囲外＝その方向の
#   上限より手前に測れた物が無い) は数としては最も小さいので、比べる前に FAR_MM へ置き換える (「遠い」とみなすのはこのプログラムの決めごと)。
#   FAR_MM は大小の比較にだけ使い、左右の差は両側とも距離のときだけ計算する。
#   シミュレータの前・左・右のセンサーは -1 を返さない (実機ではセンサーのタイムアウトで返る)。

from RumiCar import *

LIMIT_MM = 200  # RC_read の値を信じる距離の上限 [mm]。これより先は RC_read が -3 (範囲外) を返す。上限を変えるときはこの 1 行を直す
FAR_MM = 9999   # 範囲外 (-3) を「遠い」として比べるための値。RC_read が返すどの距離 (卓上は最大 2000 mm) よりも大きい＝上限を変えても直さなくてよい
TOP = 240       # 速度: 壁に沿って安定しているとき (255 = 全開)
CRUISE = 180    # 速度: 壁を探す・寄せる・真ん中へ寄せるとき
TURNV = 110     # 速度: 前に壁が見えて曲げるとき (FR は 110 前後で最も小さく回れる)
BACKV = 150     # 速度: 後退
D_TURN = 200    # 前方がこれより近ければ曲げる [mm] (LIMIT_MM 以下にする。LIMIT_MM と同じ値なら「見えたら曲げる」)
NEAR = 100      # 右の壁がこれより近ければ左へ [mm]
FARB = 170      # 右の壁がこれより遠ければ右へ [mm]
ESC = 80        # 前方がこれより近ければ後退して切り返す [mm]
BACKT = 8       # 後退を続ける回数 (loop の回数。20 Hz なら 0.4 秒)
SEEK = 4        # 壁を見失ってから全舵で右へ切り続ける回数。その後は半分の時間だけ切る

back = 0        # 後退の残り回数
bdir = CENTER   # 後退中の舵
lost = 0        # 右の壁を見失ってからの回数


def setup():
    global back, lost
    RC_setup()
    RC_setReadLimit(LIMIT_MM)   # LIMIT_MM より先は RC_read が -3 (範囲外) を返す
    back = 0
    lost = 0


def loop():
    global back, bdir, lost
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
