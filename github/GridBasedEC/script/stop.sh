#!/bin/sh
killall -9 tcpreplay
ensemblepid=`ps -ef  |grep En | grep java | awk '{print $2}'` 
if [ ! -z $ensemblepid ];then	
	kill -9 $ensemblepid
fi
killall -9 fcapture
rm -f unclassified*
rm -f log
rm -f fcaplog

