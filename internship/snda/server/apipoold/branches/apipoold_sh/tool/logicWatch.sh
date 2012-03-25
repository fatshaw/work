#! /bin/sh
#for apipoold
cd /opt/app/server/apipoold/log/
scan_record='./scan_record'
old_scan_record='./old_scan_record'
temp_scan_record='./temp_old_scan_record'
sent_sms_record='./sent_sms_record'
sendsms='/opt/app/php5/bin/php ../tool/send_sms.php'
statefile='./state_record'
tm=`date +%Y_%m_%d`
errorfile="apipoold_error"$tm
#sendsms='echo'
numlist='13524515129 13482027618'

#Try to Get current logfile
logfiles='./tmp_log_file_list'
scan_rpcreport='./rpcreport'
old_scan_rpcreport='./rpcreport_old'
temp_scan_rpcreport="./rpcreport_temp"
#list all the log file according to their date and store the result in the logfile.
ls -t apipoold-20*.log > ${logfiles}

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
\cp -f  ${scan_rpcreport} ${old_scan_rpcreport}

#Get new scan record
#-h no-filename in the result 
grep -h -E 'ERROR|FATAL' ${logfilepre} ${logfile} | grep -v "58.215.44.5" >${scan_record} 
if test ! -w ${old_scan_record};
then
>${old_scan_record}
echo "old has been deleted."
fi
#grep -E '^<':begin with '<'
diff ${scan_record} ${old_scan_record}|grep -h -E '^< ' |awk -F'<' '{print $2}'> ${temp_scan_record}

cat ${temp_scan_record} >> ${errorfile}

grep -h -E 'STATMSG' ${logfilepre} ${logfile} > ${scan_rpcreport}
if test ! -w ${old_scan_rpcreport};then
>${old_scan_rpcreport}
echo "old rpcreport has been deleted"
fi
diff ${scan_rpcreport} ${old_scan_rpcreport}|grep -h -E '^< ' |awk -F'<' '{print $2}'> ${temp_scan_rpcreport}
laststr=`tail -n 1 ${temp_scan_record}`
laststr=`echo $laststr | awk -F'\' '{print substr($1,41,40)}'`

ip=`/sbin/ifconfig  | grep 'inet addr:'|grep '10.'|awk '{print substr($2,6)}' | awk -F. '{print $3"."$4}'`

try_send_sms()
{
echo $0 >>$sent_sms_record
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
    echo "0 0 0">$statefile
fi 
needsms=0
state=0
oldstate=`cat $statefile | awk '{print $1}'`
oldcount=`cat $statefile | awk '{print $2}'`
olderstate=`cat $statefile | awk '{print $3}'`
span=0
#decision principle: the current state is equal to the oldstate but different from the olderstate
if [ $count -le 10 ]; then
 	state=0
    if [ $oldstate -eq 0 -a $olderstate -gt 0 ];then   
        needsms=1
    fi
else
    state=1
    if [ $oldstate -eq 0 -a $olderstate -eq 0 ]; then
		needsms=1
	else 
		needsms=0
		span=`expr $count - $oldcount`
		if [ $span -gt 50 ];then
		    needsms=1
		fi      
    fi 
fi 

olderstate=$oldstate
echo $state" "$count" "$olderstate > $statefile

if [ $needsms -eq 1 ]; then
	if [ $count -gt 10 ]; then 
		try_send_sms ${count} "AP:$laststr"
	fi
	
	if [ $count -eq 0 ]; then 
		try_send_sms "" "APIPOOL in Normal state"
	fi
fi
#Check Fpm Request timeout
count=`grep -h 'request timeout' ${temp_scan_record} |grep -h "FcgiAgentHandler" |wc -l`
laststr=`grep -h 'request timeout' ${temp_scan_record} |grep -h "FcgiAgentHandler" |tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr | awk '{print $4}'`
if [ $needsms -eq 1 -a $count -gt 10 ]; then
	try_send_sms ${count} "${laststr}: Request timeout"
fi

#Check request buf exceed max_pack_len
count=`grep -h 'exceed max_pack_len' ${temp_scan_record} |grep -h "FcgiAgentHandler" |wc -l`
laststr=`grep -h 'exceed max_pack_len' ${temp_scan_record} |grep -h "FcgiAgentHandler" |tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr | awk '{print $4}'`
if [ $needsms -eq 1 -a $count -gt 10 ]; then
	try_send_sms ${count} "${laststr}:FcgiAgentHandler request Exceed max_pack_len" 
fi

#Check onConnectError
count=`grep -h 'onConnectError' ${temp_scan_record} |grep -h "FcgiAgentHandler"|wc -l`
laststr=`grep -h 'onConnectError' ${temp_scan_record} |grep -h "FcgiAgentHandler"|tail -n 1`
echo ${laststr} >> ${errorfile}
laststr=`echo $laststr | awk '{print $4}'`
if [ $needsms -eq 1 -a $count -gt 10 ]; then
	try_send_sms ${count} "${laststr} to FcgiAgentd"
fi

