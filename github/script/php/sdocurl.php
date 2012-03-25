<?php
define("COOKIE","/tmp/cookie");
$str = login();
$pos = strpos($str,'href');
$pos = strpos($str,"ticket",$pos);
$pos += strlen("ticket") + 1;
$start = $pos;
while(ord($str[$pos]) != ord("&"))
{
	$pos++;
}
$ticket = substr($str,$start,$pos- $start);
$str = get("http://t.sdo.com/login?refer=http://t.sdo.com/home&ticket=".$ticket);
$str = get("http://t.sdo.com/home");
var_dump($str);

function login()
{
	$user = "fatshaw";
	$pass = "1987714";
	$url ='https://cas.sdo.com/CAS/Login.Validate.Account?service=http%3A%2F%2Ft.sdo.com%2Flogin%3Frefer%3D%2Fhome';
	$ch = curl_init($url);//创建一个用于存放cookie信息的临时文件
	$cookie  = "/tmp/cookie";//tempnam('.','~');
	$referer_login  = 'http://t.sdo.com';
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);//返回结果存放在变量中,而不是默认的直接输出
	curl_setopt($ch, CURLOPT_HEADER, true);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
	curl_setopt($ch, CURLOPT_POST, true); 
	curl_setopt($ch, CURLOPT_REFERER, $referer_login); 
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2); 
	$fields_post = array('username'=> $user,'password'=> $pass,'service'=>'http://t.sdo.com','warn'=>'false','It'=>'test','_eventId'=>'submit','idtype'=>'0','gametype'=>'0','appId'=>'0','code'=>2);
	$headers_login = array('User-Agent'=> 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)',    'Referer'=> 'http://t.sdo.com');
	$fields_string = '';
	foreach($fields_post as $key => $value)
	{
		$fields_string .= $key . '=' . $value . '&';
	} 
	$fields_string = rtrim($fields_string , '&'); 
	curl_setopt($ch, CURLOPT_COOKIESESSION, true);//关闭连接时，将服务器端返回的cookie保存在以下文件中
	curl_setopt($ch, CURLOPT_COOKIEJAR, COOKIE);
	curl_setopt($ch, CURLOPT_COOKIEFILE, COOKIE);
	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers_login);
	//curl_setopt($ch, CURLOPT_POST, count($fields));
	curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string); 
	$result= curl_exec($ch);
	curl_close($ch);  
	return $result;
}

function get($url){
	
	$ch = curl_init($url); 
	$headers = array('User-Agent' => 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)'); 
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
	curl_setopt($ch, CURLOPT_COOKIEFILE, COOKIE);
	curl_setopt($ch, CURLOPT_COOKIEJAR, COOKIE);
	$result = curl_exec($ch);
	if(curl_error($ch)){
		return false;
	}
	curl_close($ch);
	return $result;
}

function getUrl($str,$pos){
	$start = $pos;
	if(ord($str[$pos]) == ord("\"") or ord($str[$pos]) == ord("'")){
			$pos+=1;
	}
	while(ord($str[$pos]) != ord("\"") and ord($str[$pos]) != ord("'") and ord($str[$pos]) != ord("&")){
			$pos+=1;
	}
	return substr($str,$start+1,$pos-$start-1);
}

?>
