#!/bin/bash
# This is a test, this is only a test
SNAPNAME=`date +%Y.%m.%d-%H.%M.%S`
#xfs_freeze -f /data/shadow_share/
lvcreate -L10M -s -n $SNAPNAME /dev/shadowvol/sh_test
#xfs_freeze -u /data/shadow_share/
mkdir /data/shadow_share/@GMT-$SNAPNAME
mount /dev/shadowvol/$SNAPNAME \
       /data/shadow_share/@GMT-$SNAPNAME -onouuid,ro

