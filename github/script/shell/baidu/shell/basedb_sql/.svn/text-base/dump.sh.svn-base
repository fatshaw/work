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
mkdir -p "$version";
cd "$version";
mysqldump=`whereis mysqldump|awk '{print $2}'`;
pic_web_dumpop="$mysqldump --skip-opt --add-drop-table --add-drop-database -h $host --port ${pic_web_port} -u$user -p$passwd"
pic_core_dumpop="$mysqldump --skip-opt --add-drop-table --add-drop-database -a -h $host --port ${pic_core_port} -u$user -p$passwd"

echo "=========start to export prodb_feed_testing database structure====================";
eval $(echo "${pic_web_dumpop} -d --databases prodb_feed_testing > prodb_feed_testing.sql");

echo "=========start to export prodb_mgmt database structure====================";
eval $(echo "${pic_web_dumpop} -d --databases prodb_mgmt > prodb_mgmt.sql");

echo "=========start to export prodb_mgmt table : account_product_user==========";
echo "use prodb_mgmt;" > prodb_mgmt_initdata.sql;

eval $(echo "${pic_web_dumpop} -t prodb_mgmt account_product_user --where="role_id=1">>prodb_mgmt_initdata.sql");
echo "=========start to export prodb_mgmt table :product_line, account_type, account_role====";

eval $(echo "${pic_web_dumpop} -t prodb_mgmt product_line account_role account_type>> prodb_mgmt_initdata.sql");
echo "=========start to export prodb_mgmt table :merchant test and admin=========";

eval $(echo "${pic_web_dumpop} -t prodb_mgmt merchant >> prodb_mgmt_initdata.sql");
echo "=========succeed to export prodb_mgmt database structure and init data====";


echo "=========start to export prodb_product_info database structure and init data==";
eval $(echo "${pic_core_dumpop} -d --databases prodb_product_info >  prodb_product_info.sql");
echo "
use prodb_product_info; 

INSERT INTO \`index_helper_attribute\` (\`iftestdb\`, \`latesteffectivetime\`, \`preparedeffectivetime\`) VALUES 
(1, NULL, NULL), 
(0, NULL, NULL); 
 
INSERT INTO \`aw_updatetime_tracking\` (\`latesteffectivetime\`, \`istestdb\`) VALUES  
(NULL, 1),  
(NULL, 0);  
 
INSERT INTO \`uniqueid_creator\` VALUES ('1', 'Product', '1');  
INSERT INTO \`uniqueid_creator\` VALUES ('2', 'Attribute', '1');  
INSERT INTO \`uniqueid_creator\` VALUES ('3', 'Image', '1');  
INSERT INTO \`uniqueid_creator\` VALUES ('4', 'TestProduct', '1');  " > prodb_product_info_initdata.sql
echo "=========succeed to export prodb_product_info database structure and init data==";

if [ "$debug" == "true" ];then
	set +x
fi

