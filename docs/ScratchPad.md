CREATE CONTINUOUS QUERY cq_2m ON poulailler BEGIN SELECT mean(*) INTO  poulailler.semaine.:MEASUREMENT FROM /.*/ GROUP BY time(2m) END

CREATE CONTINUOUS QUERY cq_15m ON poulailler BEGIN SELECT mean(*) INTO  poulailler.semaine.:MEASUREMENT FROM /.*/ GROUP BY time(15m) END



CREATE DATABASE poulailler WITH DURATION 1d REPLICATION 1 NAME jour
CREATE RETENTION POLICY semaine ON "poulailler" DURATION 1w REPLICATION 1
CREATE RETENTION POLICY annee ON "poulailler" DURATION 52w REPLICATION 1
USE poulailler
CREATE CONTINUOUS QUERY cq_2m ON poulailler BEGIN SELECT mean(*) INTO  poulailler.semaine.:MEASUREMENT FROM /.*/ GROUP BY time(2m),* END
CREATE CONTINUOUS QUERY cq_15m ON poulailler BEGIN SELECT mean(*) INTO  poulailler.annee.:MEASUREMENT FROM /.*/ GROUP BY time(15m),* END


[How to make Continuous Query inherit tags from the source time series? #126](https://github.com/influxdata/influxdb.com/issues/126)



sudo systemctl list-unit-files
sudo systemctl start grafana-server
sudo systemctl enable grafana-server
sudo systemctl unmask grafana-server.service
sudo systemctl unmask grafana-server


usrgroup -aG video pi

docker \
  run \
    --name="motioneye" \
    -p 8765:8765 \
    --hostname="motioneye" \
    -v /etc/localtime:/etc/localtime:ro \
    -v /etc/motioneye:/etc/motioneye \
    -v /var/lib/motioneye:/var/lib/motioneye \
    --restart="always" \
    --detach=true \
    --device=/dev/video0:/dev/video0 \
    --device=/dev/video1:/dev/video1 \
    ccrisan/motioneye:master-armhf



v4l2-ctl --all -d /dev/video1

{Manual USB camera settings in Linux](https://www.kurokesu.com/main/2016/01/16/manual-usb-camera-settings-in-linux/)


ffmpeg -f video4linux2 -i /dev/v4l/by-id/usb-045e_USB_camera-video-index0 -r 30 -s 640x480 -f mjpeg -qscale 5  -f rtp rtp://234.5.5.5:1234

[Live Webcam Streaming using VLC on the Command Line](https://sandilands.info/sgordon/live-webca-streaming-using-vlc-command-line)
cvlc -vvv v4l2:///dev/video0 --sout '#transcode{vcodec=mp2v,vb=800,acodec=none}:rtp{sdp=rtsp://:8554/}'






https://trac.ffmpeg.org/wiki/Capture/Webcam
Linux

Uses the video4linux2 (or simply v4l2) input device to capture live input such as from a webcam. See the v4l2 input device documentation for more information.
List devices

To list the supported, connected capture devices you can use the v4l-ctl tool. This example shows two connected webcams: /dev/video0 and /dev/video1.

$ v4l2-ctl --list-devices
USB2.0 PC CAMERA (usb-0000:00:1d.7-1):
        /dev/video1

UVC Camera (046d:0819) (usb-0000:00:1d.7-2):
        /dev/video0

List device capabilities

To list available formats (supported pixel formats, video formats, and frame sizes) for a particular input device:

$ ffmpeg -f v4l2 -list_formats all -i /dev/video0
…
[video4linux2,v4l2 @ 0xf07d80] Raw       :   yuyv422 :     YUV 4:2:2 (YUYV) : 640x480 160x120 176x144 320x176 320x240 352x288 432x240 544x288 640x360
[video4linux2,v4l2 @ 0xf07d80] Compressed:     mjpeg :                MJPEG : 640x480 160x120 176x144 320x176 320x240 352x288 432x240 544x288 640x360

Alternatively you could use v4l2-ctl --list-formats-ext to list available formats.
Encoding example

Example to encode video from /dev/video0:

ffmpeg -f v4l2 -framerate 25 -video_size 640x480 -i /dev/video0 output.mkv

Adjusting camera functions

Brightness, zoom, focus, etc, can be adjusted with v4l2-ctl. Display all controls and their menus:

v4l2-ctl -L

Then adjust the value:

v4l2-ctl -c <option>=<value>





Microsoft LifeCam VX-1000
