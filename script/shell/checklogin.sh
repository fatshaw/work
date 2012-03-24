check()
{
	cd ~
	if [ -f ./warning ];then
			cont=`cat ./warning`
			type=`echo $cont | awk -F: '{print $1}'`
			if [ $type ==  "FATAL" ];then
				echo "FATAL:your space is above than limit. You cannot login.Pl contact administrator"
				sleep 2
				username=`whoami`
				pid=`ps -ef | grep $username |grep bash| awk '{print $2}' | awk '{print $1}'`
				kill -9 $pid
			else
			
				echo $cont
			fi
	fi
}

check