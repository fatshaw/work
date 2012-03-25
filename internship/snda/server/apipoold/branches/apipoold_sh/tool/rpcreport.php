<?php
include_once("Statistic.inc.php");
function getValue($name,$content)
{
    $pos = strpos($content,$name);
    if($pos === false)
    {
        var_dump("can not find".$name);
        return "";
    }
    $pos  = strpos($content,":",$pos);
    if($pos === false)
    {
        var_dump("can not find:");
            return "";
    }
    $pos+=1;
    $end = strpos($content,",",$pos);
    if($end == false){	
        $end = strpos($content,"\n");
    }
    return trim(substr($content,$pos,$end-$pos));
    
}
function getIp()
{
	$str=trim(exec('/sbin/ifconfig  | grep Bcast | grep 10. | grep inet' ));
	$str = explode(' ',$str);
	$ip=explode(":",$str[1]);
	return $ip[1];
}

function getTime($date,$time)
{
  $date = explode("-",$date);
  $time = explode(":",$time);
//  $tmp=$date[0].":".$date[1].":".$date[2].":".$time[0].":".$time[1].":".$time[2];
  $tmp =  mktime(intval($time[0]),intval($time[1]),intval($time[2]));//,intval($date[1]),intval($date[2]),intval($time[0]));
    return $tmp;
}

$path = "/opt/app/server/apipoold/log/";
chdir($path);
#$ret = exec("ls -t *.log | head -n 1 > a");
#$filelist = file("a");
#foreach($filelist as $file){
#$file = trim($file,"\n");
#exec("grep -E 'STATMSG' $file | tail -n 10000 > a");
$tmp = file("rpcreport_temp");
$now = date("Y-m-d G:i:s");
$statArray = array();

$time =getDate();
$date = $time['year'].$time['mon'].$time['mday'];
$file = "rpc_".$date.".log";
$file = fopen($file,'a+');
foreach($tmp as $tmp)
{
		$nameArray = array();
		$tmp1 = explode(" ",$tmp);
		$timestamp  = $tmp1[1]." ".$tmp1[2];
#		if(time() - $timestamp >=1*60)continue;
		$requestSrc = getValue("reqsrc",$tmp1[7]);
		$orgip = getValue("orgip",$tmp1[8]);
		//$serviceName = getValue("serivceName",$tmp1[11]);
		$serviceName = getValue("serviceName",$tmp1[12]);
		$method = getValue("method",$tmp1[12]);
		$time = getValue("time",$tmp1[12]);
		$time = floatval($time) / 1000;
		$targetip = getValue("targetip",$tmp1[12]);
		$reqid = getValue("sendreqid",$tmp1[12]);
		$tmp1[13] = str_replace("]",'',$tmp1[13]);
		$status = getValue("status",$tmp1[13]);
		$msg = "apipool reqid = ".$reqid;
#var_dump("requestsrc = $requestSrc");
#var_dump("orgip = $orgip");
#var_dump("servicename = $serviceName");
#var_dump("method = $method");
#var_dump("time = $time");
#var_dump("targetip = ".$targetip);
#var_dump("reqid = ".$reqid);
		if($status == "1"){
		    $status = "OK";
		}
		else if($status == "2")
		{
		    $status = "TIMEOUT";
		}
		#var_dump("status = $status");
			
		$nameArray[]="sn_".$serviceName;
		$nameArray[]="rs_".$requestSrc;
		$nameArray[]="md_".$method;
		$nameArray[]="orgip_".$orgip;
		$nameArray[]="targetip_".$targetip;
		$nameArray[]="rs_orgip_".$requestSrc."_".$orgip;
		$nameArray[]="rs_sn_".$requestSrc."_".$serviceName;
		$nameArray[]="rs_md_".$requestSrc."_".$method;
		$nameArray[]="rs_tarip_".$requestSrc."_".$targetip;
		$nameArray[]="orgip_sn_".$orgip."_".$serviceName;
		$nameArray[]="orgip_method_".$orgip."_".$method;
		$nameArray[]="orgip_tarip_".$orgip."_".$targetip;
		$nameArray[]="sn_tarip_".$serviceName.'_'.$targetip;
		$nameArray[]="sn_md_".$serviceName.'_'.$method;
		$nameArray[]="md_tarip_".$method.'_'.$targetip;
		$nameArray[]="rs_sn_md_".$requestSrc."_".$serviceName."_".$method;
		$nameArray[]="rs_orgip_sn_md_".$requestSrc."_".$orgip."_".$serviceName."_".$method;
		foreach($nameArray as $name){
			if(array_key_exists($name,$statArray) == false){
				$statArray[$name] = array('OK'=>array('time'=>0,'exectimes'=>0),'TIMEOUT'=>array('time'=>0,'exectimes'=>0));
				if( array_key_exists($name,$statArray) == false)
				{
					var_dump("no in array");
					return;
				}
			}
			else
			{
				$statArray[$name][$status]['time']+=$time;
				$statArray[$name][$status]['exectimes']+=1;
			}
		}
#var_dump("end");
}
$serip = getIp();
fwrite($file,"time=".$now);
fwrite($file,"serverip=".$serip);
$msg = "ApiPooldIp = ".$serip;
foreach($statArray as $key=> $value)
{
	foreach($value as $statKey=> $statValue)
	{
		if($statValue['exectimes'] != 0)
		{
			StatLogOfInterface($key,$statKey,$msg,$statValue['time']/$statValue['exectimes'],$now,$serip,$statValue['exectimes'],"uc_api");
			$log = "servicename = ".$key.",status = ".$statKey.",average time  = ".($statValue['time']/$statValue['exectimes']).",execute times = ".$statValue['exectimes'].",channel = uc_api\r\n";
			fwrite($file,$log);
		}
	}
}
fwrite($file,"\r\n");
fclose($file);
?>
