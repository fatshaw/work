LOG="/tmp/log"
mispid=`ps -d |grep MISServer|awk '{print $1}'`
queueid=`ps -d | grep MQueue |awk '{print $1}'`
if [ -n "$mispid" ]; then
	echo "mispid = $mispid"
	kill -9 $mispid
fi
if [ -n "$queueid" ];then
	echo "queueid = $queueid"
	kill -USR1 $queueid
fi 


pid="$!"
echo $pid>./pid
pid ='cat ./pid'
kill -9 pid