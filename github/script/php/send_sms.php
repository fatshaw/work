<?php
if($argc != 3 )
{
	echo "usage php send_sms.php mobilenum message";
	exit(0);
}
$mobiles = $argv[1];
$message = $argv[2];
include_once(dirname(__FILE__)."/Statistic.inc.php");
$message = preg_replace('/,/','',$message);
$mobiles = explode(";",$mobiles);
foreach($mobiles as $mobile){
	StatSendMessage(array($mobile=>'snda'),$message);
}
?>