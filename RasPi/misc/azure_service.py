# Azure IoT Hub Serviceサンプル
# PCなどラズパイを制御する側の端末で実行するPythonスクリプトです。
# fを入力すると前進、sを入力するとRumiCarが停止します。

import sys
import asyncio
from azure.iot.hub import IoTHubRegistryManager
from azure.iot.hub.models import Twin, TwinProperties

# 各自のIoT Hubの情報を入力
IOTHUB_CONNECTION_STRING = ""
DEVICE_ID = ""

async def main():

    # define behavior for halting the application
    def stdin_listener():
        iothub_registry_manager = IoTHubRegistryManager(IOTHUB_CONNECTION_STRING)
        twin = iothub_registry_manager.get_twin(DEVICE_ID)

        while True:
            selection = input("Q: quit, F: forward, S: stop/free\n")
            if selection == "Q" or selection == "q":
                print("Quitting...")
                break
            elif selection == "F" or selection == "f":
                twin_patch = Twin(properties=TwinProperties(desired={'command' : 'forward'}))
                twin = iothub_registry_manager.update_twin(DEVICE_ID, twin_patch, twin.etag)
            elif selection == "S" or selection == "s":
                twin_patch = Twin(properties=TwinProperties(desired={'command' : 'stop'}))
                twin = iothub_registry_manager.update_twin(DEVICE_ID, twin_patch, twin.etag)

    # Run the stdin listener in the event loop
    loop = asyncio.get_running_loop()
    user_finished = loop.run_in_executor(None, stdin_listener)

    # Wait for user to indicate they are done listening for messages
    await user_finished


if __name__ == "__main__":
    asyncio.run(main())
    # If using Python 3.6 or below, use the following code instead of asyncio.run(main()):
    # loop = asyncio.get_event_loop()
    # loop.run_until_complete(main())
    # loop.close()