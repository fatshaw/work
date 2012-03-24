cd /opt/app/server/apipoold/log
count=`ls -t *.log | wc -l`
count=`expr $count - 30`
if [ $count -gt 0 ]; then
	lastloglist=`ls -t *.log | tail -n $count`
	for file in $lastloglist;
	do
		rm -f $file
	done
fi

