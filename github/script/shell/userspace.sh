cd "/home"
mailaddress="chjxiao@gmail.com"
title="space warning"
warning="warning"
usedspace="./usedspace"
warninguser1="warninguser1"
warninguser2="warninguser2"
warninguser="warninguser"
userlist1="./userlist1"
userlist2="./userlist2"
quotasize1=90000
quotasize2=90000
maxsize1=100000
maxsize2=100000
>$warninguser
>$warninguser2
>$usedspace
if [ ! -f $warninguser1 ]; then
        > $warninguser1
fi
if [ ! -f $warninguser2 ]; then
        > $warninguser2
fi
for name in `ls`
do
        if [ -d "$name" ] ; then
			> /home/"$name"/$warning
			islist1=`grep $name $userlist1 | wc -l`
			size=`du -s -m ./"$name" | awk '{print $1}'`
			if [ $size -le 100 ];then
				continue
			fi
			if [ $islist1 -gt 0 ] ;then
					quota=$quotasize1
					max=$maxsize1
			elif [ $islist1 -eq 0 ];then
					quota=$quotasize2
					max=$maxsize2
			fi
			echo "$name used space is $size(MB)" >> ./$usedspace
			if [ $size -le $max -a $size -gt $quota ];then
					echo "WARNING:your used userspace:$size(MB) exceeds the quota:$quota(MB), Pl make backup to nfs server" > ./"$name"/$warning
			elif [ $size -gt $max ];then
					echo "FATAL:your used userspace:$size(MB) exceeds the max size:$max(MB), please make some backup to nfs" > ./"$name"/$warning
					echo "$name" >> $warninguser2
			fi
        fi
done
for name in `cat $warninguser2`
do
	result=`grep "$name" $warninguser1|wc -l`
	if [ $result -gt 0 ];then
		echo "$name" >> $warninguser
	fi
done
cat $warninguser2 > $warninguser1

