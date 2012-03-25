<?php
require_once("./StateServerClient.php");

$client = new StateServerClient();
$client->setServer("localhost",8110);
$response = $client->request("MakeToken",array("sdid"=>"abc","expired_time"=>"0"));
var_dump("MakeToken result = ",$response);
$response = $client->request("GetToken",array("sdid"=>"abc"));
var_dump("GetToken result = ",$response);
$token = $response->appresdata;
$response = $client->request("CheckToken",array("sdid"=>"abc","token"=>$token));
var_dump("CheckToken result = ",$response);
?>
