#!/bin/sh
function back(){
	cd ..;cd -;
}

function ipa(){
	IP=`/sbin/ifconfig | grep "inet addr" | grep "10." | awk -F: '{print $2}' | awk '{print $1}'`
	echo $IP
}

function rip(){
	if [ "$#" -gt 0 ];
	then
		ip=`ipa`;
		sed -i "s/localhost/$ip/g" "$1"
	fi
}

function auto(){
	if [ "$#" -eq 0 ];
	then
		sh ~/auto/auto.sh "ai"
	else
		sh ~/auto/auto.sh "$1"
	fi
}

function rmbak()
{
#	find . -name "*.bak" -o -name "*~" | xargs rm -f ; 
	find . -regex ".*\.bak\|.*~\|.*\.orig" -exec rm {} \;
}

function bak(){
	if [ "$#" -gt 0 ];
	then
			cp "$1" "$1".bak
	fi
}

function ps(){
	if [ "$#" -gt 0 ];
	then
		/bin/ps -ef | grep "$1";
	else
		/bin/ps;
	fi
}

function sm(){
	a=0;
	while read i
	do
		a=$((a+i))
	done
	echo $a
}

function scpm1()
{
	pushd .
	cd ~/tmp/onlineconf/;
	mkdir -p m1;
	cd m1;
	scp xiaochaojie@m1-prodb-core00.m1:/home/work/local/prodb/pic-core/conf/* .;
	scp -r xiaochaojie@m1-prodb-core00.m1:/home/work/local/prodb/pic-core/conf/api_config .;
	popd
}

function scpcq01()
{
	pushd .
	cd ~/tmp/onlineconf/;
	mkdir -p cq01;
	cd cq01;
	scp xiaochaojie@cq01-prodb-core00.cq01:/home/work/local/prodb/pic-core/conf/* .;
	scp -r xiaochaojie@cq01-prodb-core00.cq01:/home/work/local/prodb/pic-core/conf/api_config .;
	popd
}

function rmspace(){
	sed -i 's/ //g' "$1"
}

function dec(){
	echo $(($1-$2))
}
function add(){
	echo $(($1+$2))
}

function dosunix(){
	for dir in `ls`	
	do
		if [ -d "$dir" ];then
			cd $dir;
			dos2unix *.java
			dos2unix *.cpp
			dos2unix *.cc
			dos2unix *.h
			cd ..;
		fi
	done
}

function svnimport(){
	svn import "$1" https://10.32.49.19/svn/test
}

function ctag()
{
	ctags -R -h "*.cc","*.h" --fields=+iaS --c++-kinds=+p --c-kinds=+p  --extra=+q
}

function tstotime()
{
	string="$1"
	echo $string | gawk -v str=$string '{day = strftime("%Y-%M-%d %H:%M:%S", str); print day}'
}

function cppformat()
{
	sh ~/script/shell/cppformatter.sh
}

function getrsection()
{
	FILE=$1
	rindex=$2
	delim=$3
	if [ -z "$FILE" ];then
		echo "input FILE cannot be null"
		return
	fi

	if [[ "$str" =~ 'helper' ]];then
		echo "Usage : getrsection FILE rindex delim"
		return
	fi

	if [ -z "$rindex" ];then
		rindex=1
	fi

	if [ -z "$delim" ] ;then
		delim=' '
	fi

	awk -F "$delim" '{split($0,a);print a[length(a)-1]}' $FILE
}
