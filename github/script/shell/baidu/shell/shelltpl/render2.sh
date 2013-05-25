#!/bin/sh
if [ ! $# -eq 3 ];
then
	echo "usage sh render2.sh tplfile outputfile paramfile"
	exit -1
fi

set -x

tplfile="$1"
outputfile="$2"
param="$3"

prefix="$""{"
suffix="}"

cp $tplfile $outputfile

while read line
do
	key=`echo $line | awk -F '=' '{print $1}'`;
	value=`echo $line | awk -F '=' '{print $2}'`
	sed -i "s/${prefix}${key}${suffix}/$value/g" $outputfile
done < $param

diff $tplfile $outputfile

set +x


