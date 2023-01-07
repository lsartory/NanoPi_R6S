#!/bin/sh

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <args>"
    exit
fi

xautolock -exit &> /dev/null
xset s off -dpms

mpv "$@"

xset s on +dpms
xautolock -time 5 -locker slock &
