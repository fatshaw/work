#!/bin/bash
pushfiles_dir="/home/work/dataio/pushfiles"
#pushfiles_dir="/home/disk1/cjxiao/tmp/basecheck"
cd $pushfiles_dir;
for i in `ls`
do
	if [ -d "$i" ];
	then
		pushfiles_file=`ls "$i"|tail -1`
		file_time=`stat -c %Y "$i/$pushfiles_file"`
		sys_time=`date +%s`
		pushfiles_time=$(($sys_time - $file_time))
	fi
done



echo "pushfiles_time:$pushfiles_time"
echo "BDEOF"
