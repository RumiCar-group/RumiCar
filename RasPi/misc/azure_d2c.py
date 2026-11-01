# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for full license information.

# Azure IoT Hubサンプル
# D2Cでセンサーから物体までの距離を送信

import sys
sys.path.append('..')

from RumiCar import *

# Using the Python Device SDK for IoT Hub:
#   https://github.com/Azure/azure-iot-sdk-python
# The sample connects to a device-specific MQTT endpoint on your IoT Hub.
from azure.iot.device import IoTHubDeviceClient, Message

# The device connection string to authenticate the device with your IoT hub.
# Using the Azure CLI:
# az iot hub device-identity show-connection-string --hub-name {YourIoTHubName} --device-id MyNodeDevice --output table
CONNECTION_STRING = ""

# Define the JSON message to send to IoT Hub.
MSG_TXT = '{{"distance": {distance}, "control": {control}}}'

client = IoTHubDeviceClient.create_from_connection_string(CONNECTION_STRING)

# Exercise-3.1 と同じ走行 (安全に止まる) をしながら、距離と制御を IoT Hub へ送る
RC_setup()
while True:
    distance = RC_read(CENTER)
    if distance == -3:          # 20 cm より先には何もない
        RC_drive(FREE, 0)
        control = 'FREE'
    elif distance > 120:
        RC_drive(FORWARD, 255)
        control = 'FORWARD'
    elif distance < 80:
        RC_drive(REVERSE, 255)
        control = 'REVERSE'
    else:
        RC_drive(BRAKE, 255)
        control = 'BRAKE'
    message = Message(MSG_TXT.format(distance=distance, control=control))
    client.send_message(message)
