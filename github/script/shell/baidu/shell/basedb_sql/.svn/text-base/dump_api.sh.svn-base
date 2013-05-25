#!/bin/sh
debug=false;
if [ "$1" == "-d" ];then
	debug=true;	
fi
if [ "$debug" == "true" ];then
	set -x
fi

while read line
do
	name=`echo $line | awk -F= '{print $1}'`;
	value=`echo $line | awk -F= '{print $2}'`;
	case $name in
		"user")user=$value;;
		"pic-web-port")pic_web_port=$value;;
		"pic-core-port")pic_core_port=$value;;
		"host")host=$value;;
		"passwd")passwd=$value;;
		"version")version=$value;;
	esac	
done

echo "user=$user;host=$host;pic-web-port=${pic_web_port};pic-core-port=${pic_core_port};version=$version";
mysqldump=`whereis mysqldump|awk '{print $2}'`;
pic_web_dumpop="$mysqldump --skip-opt --add-drop-table --add-drop-database -h $host --port ${pic_web_port} -u$user -p$passwd"
pic_core_dumpop="$mysqldump --skip-opt --add-drop-table --add-drop-database -a -h $host --port ${pic_core_port} -u$user -p$passwd"

echo "=========start to export prodb_product_info apilog ===============================";
eval $(echo "${pic_core_dumpop} -t prodb_product_info api_log --where=\"token!='fbef4dcf-812c-43dc-90f1-0d704dc7fa40'\" >  prodb_product_info.sql");
echo "=========succeed to export prodb_product_info apilog===============================";

if [ "$debug" == "true" ];then
	set +x
fi

