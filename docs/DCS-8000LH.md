Works with DCS-8000LH firmware `v2.02.02`

**Note that persistent RTSP access can be enabled with original unmodified D-Link firmware, using the Bluetooth --rtsp option. This modifies the necessary settings. The rtspd service is already started by default in the original firmware.**

**So there is no need to mess with the firmware at all if all you want is RTSP.**

```
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --essid Kjellerbod --wifipw redacted
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --rtsp
```


### Requirements

 * a Linux PC with a Bluetooth controller
 * python3 with @IanHarvey's
  [**bluepy**](https://ianharvey.github.io/bluepy-doc/index.html)
  library
 * WiFi network with WPA2-PSK and a known password
 * mksquashfs from the squashfs-tools package
 * a tftp server or web server accepting file uploads (for backups)
 * guts :-)
 
 The **PIN Code** and **MAC** is also printed on the code card that came with the camera.
 
 #### Real session excample after a clean upgrade to firmware v2.02.02, followed by factory reset 

1. Start by making sure the camera can see our WiFi network.  This
   also verifies that we can connect and authenticate against the
   Bluetooth LE IPCam service, without making any changes to any
   camera settings:

```
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --survey
Connecting to B0:C5:54:AA:BB:CC...
Verifying IPCam service
Connected to 'DCS-8000LH-BBCC'
DCS-8000LH-BBCC is scanning for WiFi networks...
{'I': 'AirLink126FD4', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '47'}
{'I': 'Antiboks', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '73'}
{'I': 'ASV17', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '47'}
{'I': 'ASV17-dlink', 'M': '0', 'C': '6', 'S': '4', 'E': '2', 'P': '57'}
{'I': 'DIRECT-33-HP%20ENVY%205000%20series', 'M': '0', 'C': '1', 'S': '4', 'E': '2', 'P': '46'}
{'I': 'fjorde123', 'M': '0', 'C': '1', 'S': '4', 'E': '2', 'P': '55'}
{'I': 'JOJ', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '48'}
{'I': 'Kjellerbod', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '75'}
{'I': 'Landskap_24', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '46'}
{'I': 'mgmt', 'M': '0', 'C': '1', 'S': '4', 'E': '2', 'P': '72'}
{'I': 'Rindedal', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '68'}
{'I': 'risikovirus', 'M': '0', 'C': '1', 'S': '4', 'E': '2', 'P': '45'}
{'I': 'risikovirus%20WIFI', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '45'}
{'I': 'Stavik2014', 'M': '0', 'C': '6', 'S': '4', 'E': '2', 'P': '47'}
{'I': 'TomterNett1', 'M': '0', 'C': '6', 'S': '4', 'E': '2', 'P': '44'}
{'I': 'VIF', 'M': '0', 'C': '11', 'S': '4', 'E': '2', 'P': '47'}
Done.
```

2. We're going to use the 'Kjellerbod' network, so that looks good.
   Select it and give the associated WiFi password to the camera:

```
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --essid Kjellerbod --wifipw redacted
Connecting to B0:C5:54:AA:BB:CC...
Verifying IPCam service
Connected to 'DCS-8000LH-BBCC'
DCS-8000LH-BBCC is scanning for WiFi networks...
Will configure: M=0;I=Kjellerbod;S=4;E=2;K=redacted
Done.
```

3. Verify that the camera connected to the Wifi network and got an
   address.  If not, go back and try again, making sure you are using
   the correct WiFi password:

```
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --netconf
Connecting to B0:C5:54:AA:BB:CC...
Verifying IPCam service
Connected to 'DCS-8000LH-BBCC'
wifi link is Up
wifi config: {'M': '0', 'I': 'Kjellerbod', 'S': '4', 'E': '2'}
ip config: {'I': '192.168.2.37', 'N': '255.255.255.0', 'G': '192.168.2.1', 'D': '148.122.16.253'}
Done.
```


**WARNING**: You must make a backup of your device at this point if
you haven't done so already.  See the [**Backup**](#Backup) section
below.  I only skipped it in this example because I already had a
complete backup of my camera.

  ### <a name="Streaming"></a>Streaming video locally

Which was the whole point of all this... We can now stream directly
from the camera using for example:


#### HTTP or HTTPS
```
vlc https://192.168.2.37/video/mpegts.cgi
vlc https://192.168.2.37/video/flv.cgi
```

Authenticate using the **admin** user with **PIN Code** as password

AFAICS, this camera does not support MJPEG encoding. But you can
always use ffmpeg to transcode the H.264 anyway.  Looking closer at a
stream sample:


```
$ curl --insecure -u admin:123456 https://192.168.2.37/video/mpegts.cgi>/tmp/stream
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0 93.1G    0  438k    0     0  92872      0  12d 11h  0:00:04  12d 11h 92853^C

$ mediainfo /tmp/stream 
General
ID                                       : 1 (0x1)
Complete name                            : /tmp/stream
Format                                   : MPEG-TS
File size                                : 500 KiB
Duration                                 : 5 s 433 ms
Overall bit rate mode                    : Variable
Overall bit rate                         : 752 kb/s

Video
ID                                       : 257 (0x101)
Menu ID                                  : 1 (0x1)
Format                                   : AVC
Format/Info                              : Advanced Video Codec
Format profile                           : High@L4
Format settings, CABAC                   : Yes
Format settings, ReFrames                : 1 frame
Format settings, GOP                     : M=1, N=30
Codec ID                                 : 27
Duration                                 : 5 s 450 ms
Width                                    : 1 280 pixels
Height                                   : 720 pixels
Display aspect ratio                     : 16:9
Frame rate mode                          : Variable
Color space                              : YUV
Chroma subsampling                       : 4:2:0
Bit depth                                : 8 bits
Scan type                                : Progressive

Audio
ID                                       : 256 (0x100)
Menu ID                                  : 1 (0x1)
Format                                   : AAC
Format/Info                              : Advanced Audio Codec
Format version                           : Version 2
Format profile                           : LC
Muxing mode                              : ADTS
Codec ID                                 : 15
Duration                                 : 3 s 456 ms
Bit rate mode                            : Variable
Channel(s)                               : 1 channel
Channel positions                        : Front: C
Sampling rate                            : 16.0 kHz
Frame rate                               : 15.625 FPS (1024 spf)
Compression mode                         : Lossy
```


#### RTSP

Direct RTSP access is also supported, using the same **admin** user.

The RTSP URLs are configurable, so the proper way to use RTSP is to
first check the URL of the wanted profile using the NIPCA API:

```
$ curl -u admin:123456 --insecure 'https://192.168.2.37/config/rtspurl.cgi?profileid=1'
profileid=1
urlentry=live/profile.0
video_codec=H264
audio_codec=OPUS
```

and then connect to this RTSP URL:

```
$ vlc rtsp://192.168.2.37/live/profile.0
```

Note that persistent RTSP access can be enabled with original
unmodified D-Link firmware, using the Bluetooth **--rtsp** option.
This modifies the necessary settings.  The **rtspd** service is
already started by default in the original firmware.

So there is no need to mess with the firmware at all if all you want
is RTSP.


### <a name="Backup"></a>Backup

Create a backup of everything *before* you mess up.  Restoring will be
hard anyway, so don't rely on that.  But you can forget about
restoring at all unless you have a backup, so make it anyway.

Note that the [**pib**](#Partitions) partition contains data which are
specific to **your** camera, and cannot be restored from any other
source!  This includes
 * model number
 * hardware revision
 * mac address
 * feature bits
 * private keys, pincode and passwords

Well, OK, we can restore most of the [**pib**](#Partitions)  using information from
the [camera label](https://www.mork.no/~bjorn/dcs8000lh/dcs8000lh-label.jpg), but
it's better to avoid having to do that...

A backup is also useful for analyzing the file systems offline.

Making a backup without networking is inconvenient, so setup
networking first. In theory, you could dump the flash to the serial
console. But this would be very time consuming and tiresome.

The D-Link firmware provides a selection of network file transfer
tools. Pick anyone you like:
 * tftp
 * wget
 * curl
 * ...and probably more

I've been using tftp for my backups because it is simple. You'll
obviously need a tftp server for this. Google for instructions on
setting that up.  You could alternatively set up a web server and use
wget or curl to post the files there, but this is more complx to set
up IMHO.

Here is one example of how to enable temporary telnet access and
copying all camera flash partitions to a tftp server:

```
$ ./dcs8000lh-configure.py B0:C5:54:AA:BB:CC 123456 --telnetd
Connecting to B0:C5:54:AA:BB:CC...
Verifying IPCam service
Connected to 'DCS-8000LH-BBCC'
Adding the 'admin' user as an alias for 'root'
Attempting to run 'grep -Eq ^admin: /etc/passwd||echo admin:x:0:0::/:/bin/sh >>/etc/passwd' on DCS-8000LH-BBCC by abusing the 'set admin password' request
Setting the 'admin' user password to '123456'
Attempting to run 'grep -Eq ^admin:x: /etc/passwd&&echo admin:123456|chpasswd' on DCS-8000LH-BBCC by abusing the 'set admin password' request
Starting telnetd
Attempting to run 'pidof telnetd||telnetd' on DCS-8000LH-BBCC by abusing the 'set admin password' request
 

Attempting to run '[ $(tdb get HTTPServer Enable_byte) -eq 1 ] || tdb set HTTPServer Enable_byte=1' on DCS-8000LH-BBCC by abusing the 'set admin password' request
Attempting to run '/etc/rc.d/init.d/extra_lighttpd.sh start' on DCS-8000LH-BBCC by abusing the 'set admin password' request
Done.


$ telnet 192.168.2.37
Trying 192.168.2.37...
Connected to 192.168.2.37.
Escape character is '^]'.
localhost login: admin
Password: 


BusyBox v1.22.1 (2019-02-14 17:06:35 CST) built-in shell (ash)
Enter 'help' for a list of built-in commands.


# for i in 0 1 2 3 4 5 6 7 8; do tftp -l /dev/mtd${i}ro -r mtd$i -p 192.168.2.1; done`
```

Change 192.168.2.37 to the address of your camera and 192.168.2.1 to
the address of your tftp server. Note that most tftp servers require
existing and writable destination files. Refer to your tftp server docs
for details.
