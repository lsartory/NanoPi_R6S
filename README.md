# Gentoo on NanoPi R6S

This repository contains my configuration files and other useful tools for running Gentoo on the FriendlyElec NanoPi R6S.
Video acceleration is working for X, mpv and Moonlight Embedded.

# eMMC partitioning

  Disk /dev/mmcblk2: 28.91 GiB, 31037849600 bytes, 60620800 sectors
  Units: sectors of 1 * 512 = 512 bytes
  Sector size (logical/physical): 512 bytes / 512 bytes
  I/O size (minimum/optimal): 512 bytes / 512 bytes
  Disklabel type: gpt
  Disk identifier: 3ADEA03D-C612-46ED-CFF9-B79439F11561
  
  Device          Start      End  Sectors  Size Type
  /dev/mmcblk2p1  16384    24575     8192    4M unknown
  /dev/mmcblk2p2  24576   286719   262144  128M EFI System
  /dev/mmcblk2p3 286720 60620766 60334047 28.8G Linux filesystem

