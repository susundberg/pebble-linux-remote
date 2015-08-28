# pebble-linux-remote

Pebble C-app to send app-messages and python side scripts to receive those on linux

## Features

* Configure any linux command to your pebble key -- for example key press "page down" and "page up" so that you can use pebble as slide-change device. Or configure the select key to lock the screen! Or run a gpio pin change and control your home automation!

* Currently pebble provides at most 9 separate clicks, that can be turned of if not needed. The 9 comes from combination of "UP", "DOWN", "SELECT" x "SINGLE CLICK", "DOUBLE CLICK", "LONG CLICK"

* Automatic configuration & ping when connected to python script -- vibes & color change on "connection acquired" and "connection failed"

## Usage

Start app on the pebble, start python script on the linux. 

Then check the default.ini for example how to do the key / command configuration. When the python script starts up, it sends the configuration to pebble. Currently unsubscribing from events requires re-starting the app on the pebble.

You might want to disable the multi-click handlers to get faster sending of the single click events (pebble must wait for timeout of the second click to determine if its multi or single click).

## Known issues

* There is no verification/autentication of the messages from the pebble -> SECURITY ISSUES -> USE WITH CAUTION 
 
* I did not manage to get the bluetooth connection to work with 3.13 kernel, seems like it might be some regression. I updated my kernel to 3.15RC2 and after that the connection works.

* On Kubuntu 15.04 there was modem-manager connecting to the bluetooth device, it will mess up connection.

* The bluetooth connection seems to fail sometimes at the python start-up. Re-starting the python script / and / or disable-enable toggle on the pebble helps.

* If the pebble is connected to other device (say your android phone) the connection does not work. You can have multiple pairings on the pebble, it works fine, but at least my experience was that simultanius connections do not work.



## Installing python part
```
virtualenv ./env/
pip install -r requirements.txt
```

## Installing the pebble part

Compiled with Pebble SDK3.0, simple as ```pebble build && pebble install ```

## Installing OS stuff

1. Do normal bluetooth pairing with the device. It worked out of the box for me.
2. do ```sudo rfcomm bind 0 00:00:00:00:00:00 1``` (where you should replace the 00 with real pebble bluetooth address )
3. Add your normal user to 'dialup' or similar group that has write permission to the rfcomm device

## Testing

Currently no automatical tests are in place. 

* Development/manual testing enviroment was Ubuntu 14.04 with updated kernel from mainline PPA.
* Pebble firmware version 2.9.1

