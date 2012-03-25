cd /home/xcj/server/stateserver/trunk/log
file="./a"
filelist=`ls -l --time-style=+%Y%m%d *.log | awk '{print $6" "$7}' > $file`
tarend=`date +%Y%m%d  -d "$1 4day ago"`
end=`date +%Y%m%d  -d "$1 2day ago"`
tardir="tar"
mkdir -p $tardir
while read line
do
    if [ "$line" != "" ];then  
        date=`echo $line | awk '{print $1}'`
        name=`echo $line | awk '{print $2}'`
        span=`expr $tarend - $date`
        if [ $span -gt 0 ];then
            rm -f $name
        else
	        span=`expr $end - $date`
	        if [ $span -gt 0 ];then
	        		mv $name $tardir
	        fi
	      fi
    fi
done < $file

tar -zvcf log$end.tar.gz $tardir
rm -fr $tardir

tarlist=`ls -l --time-style=+%Y%m%d *.tar.gz | awk '{print $6" "$7}' > $file`
end=`date +%Y%m%d  -d "$1 2day ago"`
while read line
do
    if [ "$line" != "" ];then  
        date=`echo $line | awk '{print $1}'`
        name=`echo $line | awk '{print $2}'`
        span=`expr $end - $date`
        if [ $span -gt 0 ];then
            rm -f $name
        fi
    fi
done < $file