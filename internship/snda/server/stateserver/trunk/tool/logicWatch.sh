#! /bin/sh
#for phpagent'
cd /home/xcj/server/stateserver/trunk/log
scan_record='./scan_record'
old_scan_record='./old_scan_record'
temp_scan_record='./temp_old_scan_record'
sent_sms_record='./sent_sms_record'
sendsms='/opt/app/php5/bin/php ../tool/send_sms.php'
statefile='./state_record'
tm=`date +%Y_%m_%d`
errorfile="stateserver_error"$tm".log"
#sendsms='echo'
#numlist='13524515129 13482027618'
numlist='13482027618'
#Try to Get current logfile
logfiles='./tmp_log_file_list'
#list all the log file according to their date and store the result in the logfile.
ls -t stateserver-20*.log > ${logfiles}

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
grep -h -E '[ERROR]' ${logfilepre} ${logfile} >${scan_record} 
if test ! -w ${old_scan_record};
then
>${old_scan_record}
echo "old has been deleted."
fi
#grep -E '^<':begin with '<'
diff ${scan_record} ${old_scan_record}|grep -h -E '^< ' |awk -F'<' '{print $2}'> ${temp_scan_record}
laststr=`tail -n 1 ${temp_scan_record}`
laststr=`echo $laststr | awk -F'\' '{print substr($1,67,35)}'`
cat ${temp_scan_record} >> ${errorfile}
ip=`/sbin/ifconfig  | grep 'inet addr:'|grep Bcast | grep '10.'|awk '{print substr($2,6)}'`

try_send_sms()
{
echo $0 >>$sent_sms_record
#if [ ! $count -eq 0 ];
if [ ! $needsms -eq 0 ];
then
#send sms
outputstr="$ip:${2}:ct:${count} "
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
		try_send_sms ${count} "SS has error"
	else
		try_send_sms "" "stateserver in Normal state"
	fi
fi

#Check MONGOD CLIENT OPEN ERROR
count=`grep -h 'MONGO CLIENT OPEN ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |wc -l`
laststr=`grep -h 'MONGO CLIENT OPEN ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |tail -n 1`
laststr=`echo $laststr|awk '{print $5}'`
if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr} MONGOD OPEN ERROR"
fi

#Check MONGO CLIENT UPDAET ERROR
count=`grep -h 'MONGO CLIENT UPDAET ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |wc -l`
laststr=`grep -h 'MONGO CLIENT UPDAET ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |tail -n 1`
laststr=`echo $laststr|awk '{print $5}'`
if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr}MONGO UPDAET ERROR" 
fi

#Check MONGO CLIENT FINDONE ERROR
#
count=`grep -h -E 'MONGO CLIENT FINDONE ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |wc -l`
laststr=`grep -h -E 'MONGO CLIENT FINDONE ERROR' ${temp_scan_record} |grep -h "StateServerMsgAnalyser" |tail -n 1`
laststr=`echo $laststr|awk '{print $5}'`
if [ $needsms -eq 1 -a $count -gt 0 ]; then
	try_send_sms ${count} "${laststr} MONGO FINDONE ERROR" 
fi
