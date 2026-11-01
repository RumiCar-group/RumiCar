# ラズパイ用カメラ + OpenCVサンプル
# 物体までまでの距離が100mm以下の時写真を撮影、保存し停止する

import sys
sys.path.append('..')

from RumiCar import *
import cv2
from datetime import datetime

cap = cv2.VideoCapture(0, cv2.CAP_V4L2)

RC_setup()
RC_drive(FORWARD, 191)           # 全開の約 75 % の信号で前進
while True:
    ret, frame = cap.read()
    if not ret:
        print("failed to get image, please enable camera")
        break

    distance = RC_read(CENTER)
    if 0 <= distance < 100:      # 100 mm 以内に物体: 写真を保存して停止
        now = datetime.now()
        cv2.imwrite(now.strftime("%Y%m%d_%H%M%S") + '.jpg', frame)
        RC_drive(BRAKE, 255)
        break

cap.release()
cv2.destroyAllWindows()
