<?php

require_once("ApiPoolAccessCtrl.php");

function printTableList($table) {
	for ($i=0; $i<count($table); $i++) {
		
		echo "\n<tr>\n";
		$row = $table[$i];
		for ($j=0; $j<count($row); $j++) {
			echo '<td bgcolor="#FFFFFF"><b>'.$row[$j];
			echo "</b></td>\n";
		}
		echo "</tr>\n";
		
	}
}

$conf = parse_ini_file("./install.conf");
$acc = new ApiPoolAccessCtrl($conf["ACCESS_DB_SERVER"], $conf["ACCESS_DB_PORT"], $conf["ACCESS_DB_DATABASE"], $conf["ACCESS_DB_USER"], $conf["ACCESS_DB_PWD"]);

$action_list = array(
	array("addRequestSrcIp", "update ip", "request_src", "ip"),
	array("removeRequestSrcIp", "del ip", "request_src", "ip"),
	array("addRequestSrcRate", "update rate", "request_src", "service", "amount", "freq"),
	array("removeRequestSrcRate", "del rate", "request_src", "service"),
	array("addRequestSrcApi", "update api", "request_src", "service", "api"),
	array("removeRequestSrcApi", "del api", "request_src", "service", "api")
);

echo '<html><body>';

for ($i=0; $i<count($action_list); $i++) {
	$action = $action_list[$i];
	echo '<form action="modify.php" method="post">'."\n";
	echo '<input type="hidden" name="type" id="type" value="'.$action[0].'"/>'."\n";
	for ($j=2; $j<count($action); $j++) {
		echo $action[$j].': <input type="text" name="'.$action[$j].'" />'."\n";
	}
	echo '<input type="submit" value="'.$action[1].'" />'."\n";
	echo '</form>'."\n";
}

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_req_src_ip_));
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_req_src_service_rate_));
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_req_src_service_api_));
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_params_));
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_version_));
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->generateRequestRateApiTable());
echo "</table>\n";
echo "</font>";

echo '<font face="verdana">';
echo '<table border="1" cellpadding="1" cellspacing="2">';
printTableList($acc->getTableList(ApiPoolAccessCtrl::$tb_loginfo_));
echo "</table>\n";
echo "</font>";

echo '</body></html>';

?>