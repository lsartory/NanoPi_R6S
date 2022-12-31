#!/bin/sh
xautolock -disable
xrandr --output HDMI-1 --mode 1920x1080 --rate 60
moonlight-embedded stream -app Desktop -platform rk -1080 -bitrate 80000 "$1" > /dev/null
xrandr --output HDMI-1 --mode 3840x2160 --rate 60
xautolock -enable
