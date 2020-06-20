# -------------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License. See License.txt in the project root for
# license information.
# --------------------------------------------------------------------------

# Azure IoT Hubサンプル
# Desired propety でラズパイを制御

import os
import asyncio
from six.moves import input
import threading
from azure.iot.device.aio import IoTHubDeviceClient
import pigpio

# DRV8835のピン設定
BIN1 = 18
BIN2 = 13

# PWM周波数
BIN_FREQUENCY = 960

async def main():
    # The connection string for a device
    conn_str = ""
    # The client object is used to interact with your Azure IoT hub.
    device_client = IoTHubDeviceClient.create_from_connection_string(conn_str)

    pi = pigpio.pi()

    # 各GPIOの初期化
    pi.set_mode(BIN1, pigpio.OUTPUT)
    pi.set_mode(BIN2, pigpio.OUTPUT)

    # connect the client.
    await device_client.connect()

    # define behavior for receiving a twin patch
    async def twin_patch_listener(device_client):
        while True:
            patch = await device_client.receive_twin_desired_properties_patch()  # blocking call
            # print("the data in the desired properties patch was: {}".format(patch))
            # print(patch)
            command = patch['command']

            if command == 'stop':
                pi.hardware_PWM(BIN1, BIN_FREQUENCY, 0)
                pi.hardware_PWM(BIN2, BIN_FREQUENCY, 0)
            elif command == 'forward':
                pi.hardware_PWM(BIN1, BIN_FREQUENCY, 900000)
                pi.hardware_PWM(BIN2, BIN_FREQUENCY, 0)

    # define behavior for halting the application
    def stdin_listener():
        while True:
            selection = input("Press Q to quit\n")
            if selection == "Q" or selection == "q":
                print("Quitting...")
                break
            else:
              print("Input was..." + selection)

    # Schedule task for twin patch
    asyncio.create_task(twin_patch_listener(device_client))

    # Run the stdin listener in the event loop
    loop = asyncio.get_running_loop()
    user_finished = loop.run_in_executor(None, stdin_listener)

    # Wait for user to indicate they are done listening for messages
    await user_finished

    # Finally, disconnect
    await device_client.disconnect()


if __name__ == "__main__":
    # asyncio.run(main())
    # If using Python 3.6 or below, use the following code instead of asyncio.run(main()):
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    loop.close()
