#!/bin/sh
##########################################################
# SIMPLE BASH TEMPLATING ENGINE
# USAGE: render
##########################################################

set -x

if [[ ! -z $2 ]]; then
	. $2
else
	echo "usage sh render.sh tpl_file_name param_file_name"
	exit -1;
fi
abc.abc=123
echo "$(abc.abc)"

echo "${indexHelperAdminDBProxy.password}"
exit
render(){
	File="$1"
	while read -r line ; do
		while [[ "$line" =~ (\$\{[a-zA-Z_][.a-zA-Z_0-9]*\}) ]] ; do
			LHS=${BASH_REMATCH[1]}
			RHS="$(eval echo "\"$LHS\"")"
			line=${line//$LHS/$RHS}
		done
		#echo -e "\e[00;34m$line\e[00m"
		echo -e "$line"
	done < $File
}

render $1 $2

set +x
