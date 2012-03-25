#! /bin/sh
#for phpagent'
cd /opt/app/server/phpfcgiagentd/log
scan_record='./scan_record'
old_scan_record='./old_scan_record'
temp_scan_record='./temp_old_scan_record'
sent_sms_record='./sent_sms_record'
sendsms='/opt/app/php5/bin/php ../tool/send_sms.php'
statefile='./state_record'
tm=`date +%Y_%m_%d`
errorfile="phpfcgiagentd_error"$tm".log"
#sendsms='echo'
numlist='13524515129  13482027618'

#Try to Get current logfile
logfiles='./tmp_log_file_list'
#list all the log file according to their date and store the result in the logfile.
ls -t phpfcgiagentd-20*.log > ${logfiles}

#list first line of logfile: first log file in the log dir.
logfile=`head -n 1 ${logfiles}`
#tail: print last n line of specific file
#find the latest two logfile and store the elder one in the logfilepre.
logfilepre=`head -n 2 ${logfiles}|tail -n 1` 

#record current time
date>>$sent_sms_record
date>>$errorfile

#Save old_scan_record
#why use ">>",still dont know
echo "" >>${scan_record}
#--reply=yes is deprecated , using -f or -i instead.
#cp --reply=yes ${scan_record} ${old_scan_record}
\cp -f  ${scan_record} ${old_scan_record}

#Get new scan record
#-h no-filename in the result 
grep -h -E 'ERROR|FATAL' ${logfilepre} ${logfile} >${scan_record} 
if test ! -w ${old_scan_record};
then
>${old_scan_record}
echo "old has been deleted."
fi
#grep -E '^<':begin with '<'
diff ${scan_record} ${old_scan_record}|grep -h -E '^< ' |awk -F'<' '{print $2}'> ${temp_scan_record}
laststr=`tail -n 1 ${temp_scan_record}`
laststr=`echo $laststr | awk -F'\' '{print substr($1,41,35)}'`
cat ${temp_scan_record} >> ${errorfile}
ip=`/sbin/ifconfig  | grep 'inet addr:'|grep '10.'|awk '{print substr($2,6)}'`

try_send_sms()
{
echo $0 >>$sent_sms_record
#if [ ! $count -eq 0 ];
if [ ! $needsms -eq 0 ];
then
#send sms
outputstr="$ip:${2}ct:${count} "
	for i in ${numlist};
	do
		$sendsms ${i} "${outputstr}" >>$sent_sms_record
		echo  "SENT SMS::: "${i} "${outputstr}">>$sent_sms_record
	done
else
 	echo "No Message found."
fi
}

count=`wc -l $temp_scan_record | awk '{print $1}'`
if [ ! -w $statefile ]; then
    echo "0 0">$statefile
fi 
needsms=0
state=0
oldstate=`cat $statefile | awk '{print $1}'`
oldcount=`cat $statefile | awk '{print $2}'`
span=0
if [ $count -eq 0 ]; then
    if [ $oldstate -gt 0 ];then
        state=0
        needsms=1
    fi
else
    state=1
    if [ $oldstate -eq 0 ]; then
		needsms=1
    else
		needsms=0
		span=`expr $count - $oldcount`
		if [ $span -gt 50 ];then
		    needsms=1
		fi      
    fi 
fi 

echo $state" "$count > $statefile

if [ $needsms -eq 1 ]; then
	if [ $count -gt  0 ]; then 
		try_send_sms ${count} "PFA:$laststr"
	else
		try_send_sms "" "phpfcgiagentd in Normal state"
	fi
fi

#Check Fpm Request timeout
count=`grep -h 'RECV TIMEOUT' ${temp_scan_record} |grep -h "PhpAgent" |wc -l`
laststr=`grep -h 'RECV TIMEOUT' ${temp_scan_record} |grep -h "PhpAgent" |tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr|awk '{print $4}'`

if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr},Fpm Request timeout"
fi

#Check Request Buf Is Too Large
count=`grep -h 'Buf is Too Large' ${temp_scan_record} |grep -h "RecvFromFpm" |wc -l`
laststr=`grep -h 'Buf is Too Large' ${temp_scan_record} |grep -h "RecvFromFpm" |tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr|awk '{print $4}'`
if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr},Request Buf Is Too Large" 
fi

#Check phpAgent Create Socket to Fpm error
#
count=`grep -h -E 'create  fpm  socket error' ${temp_scan_record} |grep -h "PhpAgent" |wc -l`
laststr=`grep -h -E 'create  fpm  socket error' ${temp_scan_record} |grep -h "PhpAgent" |tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr|awk '{print $4}'`
if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr} create  fpm  socket error" 
fi
