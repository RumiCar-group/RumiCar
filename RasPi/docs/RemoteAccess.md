# Remote Access

allows using RumiCar without monitor and keyboard.

## Change WiFi with Command Line
This step is needed if Wifi does not work. 

### Easy Way
* Just run RasPi Imager again setting other WiFi.

### Hard Way
[**RasPi Config**](https://www.raspberrypi.com/documentation/computers/configuration.html#using-the-command-line)
* Insert SD card into other linux PC.
* Edit `<SD card mount>/etc/wpa_supplicant/wpa_supplicant.conf`
* Insert SD card back, start the RasPi and check it is connected to WiFi.

### Notes
* The RasPi needs to be in the same LAN as your remote PC. For example, both PC has address `192.168.1.* / 24`.
* Sometimes WiFi can isolate PC from each other, so in this case either disable the isolation or use another WiFi.

## ssh
```
# Remote PC
ssh pi@rcar.local  # <user>@<host>.local
```
On windows, [**Bonjour**](https://support.apple.com/kb/dl999) is needed to use `.local` hostname instead of ip address.

## Copy Sources
```
# Remote PC
scp -r RasPi pi@rcar.local:
```
