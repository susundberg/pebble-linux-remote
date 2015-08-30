# pebble-linux-remote

Control your linux computer with your pebble watch!

## Features

* Pebble C-app to send app-messages directly over bluetooth to your linux bluetooth receiver. No android / ios device required.

* Configurable: example key press "page down" and "page up" so that you can use pebble as slide-change device. Or configure the select key to lock the screen! Or run a gpio pin change and control your home automation!

* Currently pebble provides at most 9 separate clicks, that can be turned of if not needed. The 9 comes from combination of "UP", "DOWN", "SELECT" x "SINGLE CLICK", "DOUBLE CLICK", "LONG CLICK"

* Automatic configuration & ping when connected to python script -- vibes & color change on "connection acquired" and "connection failed".

## Usage

Start app on the pebble, start python script on the linux. 
``` 
pebble-linux-remote/host_python$ python pebble_connection.py ./default.ini 
INFO:root:Connection ok, entering to active state..
```

When the pebble-app enters connection ok state, it inverts the colors: when connected the background is white, when disconnected the background is black.

You probably want to modify the default.ini to change the command configuration. When the python script starts up, it sends the configuration (what buttons are in use) to pebble. Currently unsubscribing from events requires re-starting the app on the pebble (since i did not find a easy way to unsubscribe from click event listener).

You might want to disable the multi-click handlers on some buttons to get faster sending of the single click events (When enabled Pebble must wait after single click for timeout if the second click to happen). 

### Example config:
```
[main]
device = /dev/rfcomm0
uuid = 7f1742c48dc94bb7ac8b309faf58985a

[commands]
UP,SINGLE = xdotool key Page_Up ; Emulate page down key press event -- apt-get install xdotool
DOWN,SINGLE = xdotool key Page_Down  ; Emulate page down key press event
UP,LONG = espeak "DO A BARREL ROLL!"  ; apt-get install espeak to get sound out!
DOWN,LONG = espeak "You pressed DOWN with LONG"

; Enabling the double click will make single click register slower, since pebble must wait
; for the other click.
; UP,MULTI= espeak "DO A BARREL ROLL!"
; DOWN,MULTI = espeak "Noup! Its chuck tesla"
SELECT,MULTI = espeak "TROLOLOLOLOLOLO" 

SELECT,SINGLE = espeak "Problem officer"
SELECT,LONG = xdotool key ctrl+alt+l ; Lock the screen by pressing ctrl+alt+l

```



## Known issues

* There is no verification/autentication of the messages from the pebble -> SECURITY ISSUES -> USE WITH CAUTION 
 
* I did not manage to get the bluetooth connection to work with 3.13 kernel, seems like it might be some regression. I updated my kernel to 3.15RC2 and after that the connection works.

* On Kubuntu 15.04 there was modem-manager connecting to the bluetooth device, it will mess up connection.

* The bluetooth connection seems to fail sometimes at the python start-up. Re-starting the python script / and / or disable-enable toggle on the pebble helps. NOTE: If the pebble is connected to other device (say your android phone) the connection does not work. You can have multiple pairings on the pebble, it works fine, but at least my experience was that simultanius connections do not work.


## Installation 

You need to install: 
1. python script to handle the app messages 
2. do the bluetooth binding pebble-linux 
3. install the pebble app

### Part 1: Installing Python script 

#### Option 1.A: Installing with virtualenv
```
virtualenv ./env/
pip install -r requirements.txt
```
#### Option 1.B: Installing with host python
 * Tested with Ubuntu 14.04
 * ```apt-get install python-enum34 python-six python-serial``` 

## Part 2: Installing OS stuff

1. Do normal bluetooth pairing with the device. It worked out of the box for me.
2. do ```sudo rfcomm bind 0 00:00:00:00:00:00 1``` (replace the 00 with real pebble bluetooth address)
3. Add your normal user to 'dialup' or similar group that has write permission to the rfcomm device

## Part 3: Installing the Pebble part
* Install the app from store or compile it on your own.
* No configuration required.

### Option 3.A: Compiling 
  Compiled with Pebble SDK3.0, simple as ```pebble build && pebble install ```

### Option 3.b: From the store
 Search for pebble-linux-remote and install it. 

## Testing & Bug & Support

* Currently no automatical tests are in place.
* Use github for issue reporting & tracking
* Development/manual testing enviroment was Ubuntu 14.04 with updated kernel 3.15.0-031500rc2.
* Pebble firmware version used 2.9.1

## Used libraries
* All of the communications are handled with [libpebble2](https://github.com/pebble/libpebble2). I have stripped down the library a bit for my needs. All sources in libbpebble2 directory are from that project.

