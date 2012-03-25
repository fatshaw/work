<?php

require_once("ApiPoolAccessCtrl.php");

$modify_type = $_POST["type"];

$request_src = $_POST["request_src"];
$ip = $_POST["ip"];
$service = $_POST["service"];
$amount = $_POST["amount"];
$freq = $_POST["freq"];
$api = $_POST["api"];

$request_src = trim($request_src);
$ip = trim($ip);
$service = trim($service);
$amount = trim($amount);
$freq = trim($freq);
$api = trim($api);

//var_dump($_POST);
//
//$modify_type = "addRequestSrcApi";
//$request_src = "chuanqi";
//$service = "Shorten";
//$api = "create2";

$conf = parse_ini_file("./install.conf");
$acc = new ApiPoolAccessCtrl($conf["ACCESS_DB_SERVER"], $conf["ACCESS_DB_PORT"], $conf["ACCESS_DB_DATABASE"], $conf["ACCESS_DB_USER"], $conf["ACCESS_DB_PWD"]);

$success = false;

if ($modify_type == "addRequestSrcIp") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
					&& ApiPoolAccessCtrl::checkIp($ip);
	if ($check_param) {
		$value = "'$request_src'".", "."'$ip'";
		$success = $acc->addTableRow(ApiPoolAccessCtrl::$tb_req_src_ip_, $value);
	}
}

if ($modify_type == "removeRequestSrcIp") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
					&& ApiPoolAccessCtrl::checkIp($ip);
	$check_param = true;
	if ($check_param) {
		$condition = "request_src='".$request_src."' and ip='".$ip."'";
		$success = $acc->removeTableRow(ApiPoolAccessCtrl::$tb_req_src_ip_, $condition);
	}
}

if ($modify_type == "addRequestSrcRate") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
	&& ApiPoolAccessCtrl::checkName($service)
	&& ApiPoolAccessCtrl::checkDigit($amount)
	&& ApiPoolAccessCtrl::checkDigit($freq);
	if ($check_param) {
		$value = "'$request_src'".", "."'$service'".", "."'$amount'".", "."'$freq'";
		$success = $acc->addTableRow(ApiPoolAccessCtrl::$tb_req_src_service_rate_, $value);
	}
}

if ($modify_type == "removeRequestSrcRate") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
	&& ApiPoolAccessCtrl::checkName($service);
	$check_param = true;
	if ($check_param) {
		$condition = "request_src='".$request_src."' and service='".$service."'";
		$success = $acc->removeTableRow(ApiPoolAccessCtrl::$tb_req_src_service_rate_, $condition);
	}
}

if ($modify_type == "addRequestSrcApi") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
	&& ApiPoolAccessCtrl::checkName($service)
	&& ApiPoolAccessCtrl::checkApi($api);
	if ($check_param) {
		$value = "'$request_src'".", "."'$service'".", "."'$api'";
		$success = $acc->addTableRow(ApiPoolAccessCtrl::$tb_req_src_service_api_, $value);
	}
}

if ($modify_type == "removeRequestSrcApi") {
	$check_param = ApiPoolAccessCtrl::checkName($request_src) 
	&& ApiPoolAccessCtrl::checkName($service)
	&& ApiPoolAccessCtrl::checkApi($api);
	$check_param = true;
	if ($check_param) {
		$condition = "request_src='".$request_src."' and service='".$service."' and api='".$api."'";
		$success = $acc->removeTableRow(ApiPoolAccessCtrl::$tb_req_src_service_api_, $condition);
	}
}

if ($success) {
	header("Location:index.php");
} else {
	header("Location:error.php");
}
?>