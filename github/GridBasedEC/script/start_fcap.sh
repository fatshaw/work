#!/bin/sh
/root/share/fcapture/bin/fcapture -I 10 -l unclassified -i eth0 -m /root/share/data/ -f "portrange 1024-65535 or port 80"  > fcaplog 2>&1 &