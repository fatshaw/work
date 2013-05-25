#/bin/bash 
set -x

exit_process()
{
	kill -9 "$$"
}

check_ret()
{
	if [ $? -gt 0 ] ;then
		echo "ret = $?"
		exit_process
	fi
}

replace_kv()
{
	FILE=$1
	LINE=$2
	key=`echo $LINE | awk -F '=' '{print $1}'`;
	value=`echo $LINE | awk -F '=' '{print $2}'`
	sed -i "s/$"{"${key}"}"/$value/g" $FILE
	check_ret
}

cover_kv()
{
	KEY=`echo $2 | awk -F '=' '{print $1}'`
	echo $KEY
	sed -i "s/"$KEY=.*"/"$2"/g" "$1"
	check_ret
}

render_file()
{
	FILE=$1
	PARAM=$2

	grep -v "#" $PARAM | while read -r LINE
	do
		if [  -z $LINE ];then
			continue
		fi
		replace_kv $FILE $LINE
	done 
}

cover_file()
{
	FILE=$1
	PARAM=$2
	grep -v "#" $PARAM | while read -r LINE
	do
		if [  -z $LINE ];then
			continue
		fi
		cover_kv $FILE $LINE
	done
}

create_dir()
{
	LINE=$1
	DIR=`echo $LINE | awk -F '/' '{print $1}'`
	mkdir -p $DIR
}

check_param()
{
	BASE_FILE=$1
	TPL_FILE=$2
	TYPE=$3

	if [ -z $BASE_FILE ];then
		echo "@base is not set"
		exit
	fi

	if [ -z $TPL_FILE ];then
		echo "@tpl is not set"
		exit
	fi

	if [ -z $TYPE ];then
		echo "@type is not set"
		exit
	fi
}

render_by_type()
{
	if [ -z "$1" -o -z "$2" -o -z "$3" ];then
		echo "input value is null, type=$1,file=$2,param=$3"
	 	exit_process	
	fi
	
	if [ "$1" == "replace_file" ];then
		render_file $2 $3
	elif [ "$1" == "cover_file" ];then
		cover_file $2 $3
	elif [ "$1" == "replace_line" ];then
		replace_kv $2 $3
	elif [ "$1" == "cover_line" ];then
		cover_kv $2 $3
	else
		echo "@type is not set correctly in params"
	fi

}

render()
{
	FILE="$1"
	DIR_PREFIX=`dirname $FILE`
	grep -v "#" $FILE | while read LINE;
	do
		if [ -z $LINE ];then
			continue
		fi

		if [[ "$LINE" =~ \[([0-9a-zA-Z./-]*)\] ]] ; then
			LHS=${BASH_REMATCH[1]}
			OUTPUT_FILE=$DIR_PREFIX/$LHS
			mkdir -p `dirname $OUTPUT_FILE`
			unset BASE_FILE
			unset TYPE
			unset TPL_FILE
		elif [[  "$LINE" =~ @base=(.*) ]] ;	then
			LHS=${BASH_REMATCH[1]}
			BASE_FILE=$LHS
			dos2unix $BASE_FILE
		elif [[ $LINE =~ @type=(.*) ]]; then
			LHS=${BASH_REMATCH[1]}
			TYPE=$LHS
			render_by_type $TYPE"_file" $OUTPUT_FILE $BASE_FILE
		elif [[ "$LINE" =~ @tpl=(.*) ]] ;then
			LHS=${BASH_REMATCH[1]}
			TPL_FILE=$LHS
			dos2unix $TPL_FILE
			cp $TPL_FILE $OUTPUT_FILE
		else
			render_by_type $TYPE"_line" $OUTPUT_FILE "$LINE"
		fi
	done 
}

loop_dir()
{
	for NAME in `ls -d $1/[a-z]*`
	do
		if [ -d $NAME ];then 
			NAME=`basename $NAME`;
			loop_dir "$1/$NAME"
		elif [[ "$NAME" =~ .*/params ]];
		then
			render $NAME
		fi
	done
}

loop_dir .

#cover_file $1 $2 

set +x

