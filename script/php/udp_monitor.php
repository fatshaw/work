<?php
	require_once('/opt/wwwroot/i.sdo.com/sdk/uc/libraries/UCFuncUtils.php');

	define('UDP_LOG_PATH','/opt/app/rpc_logsvr/log/');
	define('UDP_LOG_FILE_NEWLINE','/home/script/lht/udp/line.log');
	define('UDP_LOG_FILE_TEMP','/home/script/lht/udp/tmp.log');
	define('UDP_LOG_ROOT','/home/script/lht/udp/log/');
	define('UDP_IP_TEMP_FILE','/home/script/lht/udp/ip.log');
	define('UDP_INTERFACE_TEMP_FILE','/home/script/lht/udp/interface.log');
	define('UDP_STAT_API_URL','http://10.150.9.13/log_report.php');

	$config = array('mysql' => array('timeout'=>1),
					'unified' => array('timeout'=>1),
					'search' => array('timeout'=>1),
					'tt' => array('timeout'=>1),
					'memcache' => array('timeout'=>1),
					);

	$log_file = UDP_LOG_PATH.'rpc.'.date('Y-m-d');
	$s_tmp =  exec('wc -l '.$log_file);
	$ar_tmp = explode(' ',$s_tmp);

	$i_line = 1 ;
	if(!file_exists(UDP_LOG_ROOT.date('Y-m-d')))
	{
		mkdir(UDP_LOG_ROOT.date('Y-m-d'));
	}
	else
	{
		$i_line = intval(file_get_contents(UDP_LOG_FILE_NEWLINE));
	}
	file_put_contents(UDP_LOG_FILE_NEWLINE,$ar_tmp[0]);

	exec('sed -n "' . $i_line . ',' . $ar_tmp[0] . 'p" ' . $log_file . ' > ' . UDP_LOG_FILE_TEMP);
	foreach($config as $key=>$value)
	{
		exec("awk '{if($2~/".$key."/)print $1}' ".UDP_LOG_FILE_TEMP." | awk -F, '{print substr($3,7)|\"sort -u\"}END{close(\"sort\")}' | awk '{print $1 > \"".UDP_IP_TEMP_FILE."\"}'");
		$ar_ip = file(UDP_IP_TEMP_FILE);
		exec("awk '{if($2~/".$key."/)print $4|\"sort -u\"}END{close(\"sort\")}' ".UDP_LOG_FILE_TEMP." | awk '{print $1 > \"".UDP_INTERFACE_TEMP_FILE."\"}'");
		$ar_inf = file(UDP_INTERFACE_TEMP_FILE);

		if(!empty($ar_inf))
		{
			foreach($ar_inf as $inf)
			{
				$inf = rtrim($inf);
				if(!empty($ar_ip))
				{
					foreach($ar_ip as $ip)
					{
						$ip = rtrim($ip);
						$ar_error = array();
						$ar_timeout = array();
						$ar_error['channel'] = 'uc';
						$ar_error['apiname'] = $inf;
						$ar_error['remote_ip'] = $ip;
						$ar_error['response_time'] = 0;
						$ar_timeout = $ar_error;

						$s_timeout = exec("awk 'BEGIN{num=0;totaltime=0}{time=substr($3, index($3,\"<\")+1,index($3,\">\")-index($3,\"<\")-1);if($1~/".addcslashes($ip,'/')."/&&time>=".$value["timeout"]."&&$4==\"".$inf."\"&&$5~/Ok/){num++;totaltime += time;}}END{print num\"|\"totaltime}' ".UDP_LOG_FILE_TEMP);
						$error_num = exec("awk 'BEGIN{num=0}{if($1~/".addcslashes($ip,'/')."/&&$4==\"".$inf."\"&&$5~/Error/)num++}END{print num}' ".UDP_LOG_FILE_TEMP);
						$ar_tmp = explode('|',$s_timeout);
						$ar_error['status'] = 'Error';
						$ar_error['execute_times'] = $error_num;
						$ar_timeout['response_time'] = round($ar_tmp[1]/$ar_tmp[0],3);
						$ar_timeout['execute_times'] = $ar_tmp[0];
						$ar_timeout['status'] = 'Timeout';

						if(!empty($ar_error['execute_times']))
						{
							get_http_data(UDP_STAT_API_URL, array('post'=>$ar_error), '', 0, 5);
						}
						if(!empty($ar_timeout['execute_times']))
						{
							get_http_data(UDP_STAT_API_URL, array('post'=>$ar_timeout), '', 0, 5);
						}
						
					}
				}
			}
		}
		exec('rm -rf '.UDP_INTERFACE_TEMP_FILE);
		exec('rm -rf '.UDP_IP_TEMP_FILE);
	}
	$ar_error = array();
	$ar_timeout = array();
	$ar_error['channel'] = 'uc';
	$ar_error['apiname'] = 'Total_Exception_Data';
	$ar_error['remote_ip'] = '127.0.0.1';
	$ar_error['response_time'] = 0;
	$ar_timeout = $ar_error;

	$s_timeout = exec("awk 'BEGIN{num=0;totaltime=0}{time=substr($3, index($3,\"<\")+1,index($3,\">\")-index($3,\"<\")-1);if(time>=1&&$5~/Ok/){num++;totaltime += time;}}END{print num\"|\"totaltime}' ".UDP_LOG_FILE_TEMP);
	$error_num = exec("awk 'BEGIN{num=0}{if($5~/Error/)num++}END{print num}' ".UDP_LOG_FILE_TEMP);
	$ar_tmp = explode('|',$s_timeout);
	$ar_error['status'] = 'Error';
	$ar_error['execute_times'] = $error_num;
	$ar_timeout['response_time'] = round($ar_tmp[1]/$ar_tmp[0],3);
	$ar_timeout['execute_times'] = $ar_tmp[0];
	$ar_timeout['status'] = 'Timeout';

	if(!empty($ar_error['execute_times']))
	{
			get_http_data(UDP_STAT_API_URL, array('post'=>$ar_error), '', 0, 5);
	}
	if(!empty($ar_timeout['execute_times']))
	{
			get_http_data(UDP_STAT_API_URL, array('post'=>$ar_timeout), '', 0, 5);
	}
?>
