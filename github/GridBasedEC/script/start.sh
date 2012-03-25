#!/bin/sh
sh stop.sh
sleep 1
sh /root/share/fcapture/bin/start_fcap.sh 
sh /root/share/gec/src/start_grid.sh 
sleep 1

