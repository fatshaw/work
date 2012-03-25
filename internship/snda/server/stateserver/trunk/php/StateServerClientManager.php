<?php
require_once("./StateServerClient.php");
$client = new StateServerClient();
$client->setServer("localhost",8111);
define(MANAGER,"Manager");

while(true)
{
	$cmd = fread(STDIN,1024);
	$cmd = trim($cmd,"\r\n");
	if($cmd == "quit"){
		var_dump("thank you, good bye\n");
		exit;
	}
	$response = $client->request(MANAGER,array("cmd"=>$cmd));
	$result = $response->appresdata;
	var_dump($result);	
}


?>
