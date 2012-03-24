#pid=`cat pid`

sendsms='perl /home/sms/mcd_sms.pl'
numlist='13524515129 13472479353'
pid=`ps -d |grep "apipoold" | awk '{print $1}'`
pid_retry1=`ps -d |grep "apipoold" | awk '{print $1}'`
pid_retry2=`ps -d |grep "apipoold" | awk '{print $1}'`

cd /opt/app/server/apipoold/bin
date>>"log_process_check.log"
echo $pid $pid_retry1 $pid_retry2 >>"log_process_check.log"
if [ -z  "$pid" ] && [ -z "$pid_retry1" ] && [ -z "$pid_retry2" ]; then
    echo "restart process" >>"log_process_check.log"	
    sh ./ApipooldCtrl.sh restart
fi
