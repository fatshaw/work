#!/bin/bash
root="/opt/wwwroot/i.sdo.com/sdk/uc/test/"
cd $root
#if [ ! -d "log" ]; then
#	mkdir -p log
#fi 
tm=`date +%Y_%b_%w`
testpath="log/"$tm"/models/"
mkdir -p $testpath
cd $root"models/"
path=$root$testpath
test()
{
	local cur=$1
	for name in `ls`
	do
		if [ -d "$name" ];then
			mkdir -p $cur"$name/"
			cd $name
			test $cur"$name/"			
			cd ..
		fi
		if [ -f "$name" ];then
			echo "$name is a flie"
			phpunit --log-tap $cur$name".log" $name
		fi
	done
	return 0
}

test $path
