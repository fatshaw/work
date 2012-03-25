<?php
#************************************************************************
#         Author: zhangke.colin (huangsheng01@snda.com)
#  Last modified: 2010-06-11 15:06:46
#       Filename: Statistic.inc.php
#    Description: 数据采集接口
#          Other: 目前提供5个接口
#          		  1、StatActionName()
#				  	 用户行为如PV、在页面中的各行为次数累加
#				  2、StatUserinfo()
#					以参数形式传递任何用户名、IP等数据数据
#				  3、StatExecutionTime()
#					统计程序或第三方接口响应时间（执行时间）
#				  4、StatOnlineUsers()
#					统计在线用户数
#				  5、StatPageView()
#					PV统计
#				  6、StatNumberSum()
#				  	业务数据统计（5分钟累加数据）
#				  7、StatReportError()
#					上报错误
#				  8、StatLogOfInterface()
#					接口调用状态信息统计
#				  9、StatNumberAvg()
#					统计在线数据（5分钟平均数据）
#
#  统计编号的分配及对接口有任何疑问均可访问 http://monitor.sdo.com/
#  同时亦可联系监控部（张珂.colin）
#************************************************************************


/**
 * 是否加载sysvmsg模块
 * return @param boolean
 **/
function _StatExtensionLoaded() {
	return extension_loaded('sysvmsg');
}

/**
 * 数据采集类型定义
 * todo 在此添加规则
 *
 * return @param string
 **/
function _StatBehaviorDefine($type = 'action') {
	$arr	= array(
		'action'		=> "table_action(300): top_id KEY_STR, parent_id KEY_STR, node_id KEY_STR, id KEY_STR, action_name KEY_STR, execute_times COUNT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'page'      	=> "table_page(300): top_id KEY_STR, parent_id KEY_STR, node_id KEY_STR, script_name KEY_STR, execute_times COUNT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'totalpv'      	=> "table_totalpv(300): top_id KEY_STR, parent_id KEY_STR, node_id KEY_STR, execute_times COUNT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'userinfo'		=> "table_userinfo(300): flag KEY_STR, content KEY_STR, execute_times COUNT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'online'		=> "table_online(300): flag KEY_STR, datanum AVG_INT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'usedtime'		=> "table_usedtime(300): top_id KEY_STR, parent_id KEY_STR, node_id KEY_STR, id KEY_STR, action_name KEY_STR, execute_times COUNT, avg_time AVG_FLOAT, min_time MIN_FLOAT, max_time MAX_FLOAT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'reporterror'	=> "table_error(300): ip LOCAL_IP, error_id KEY_STR, source KEY_STR, happen_time CONST_STR, receive_time TIME_FLOOR, error_cnt SUM_INT, msg CONST_STR",
		'number_sum'	=> "table_numbersum(300): flag KEY_STR, app_id KEY_STR, datanum SUM_INT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'number_avg'	=> "table_numberavg(300): channel_name KEY_STR, app_id KEY_STR, datanum AVG_INT, happen_time TIME_FLOOR, ip LOCAL_IP",
		'interfaceLog'	=> "tbl_interfacelog(300): function_name KEY_STR, status KEY_STR, msg CONST_STR, responseTime AVG_FLOAT, remote_ip KEY_STR, happen_time CONST_STR, receive_time TIME_FLOOR, datanum CONST_STR, channel_name KEY_STR",
		'smsmessage'	=> "table_smsmsg(1): linkman KEY_STR, phone KEY_STR, message KEY_STR, execute_times COUNT, receive_time TIME_FLOOR, ip KEY_STR"
	);

	if (isset($arr[$type]))
		return $arr[$type];
	else
		return NULL;
}

/**
 * PV统计
 *
 **/
function StatPageView($channel_id, $script_name) {
    if (!_StatExtensionLoaded())
        return NULL;
    list($cid, $pid, $nid) = explode('|', $channel_id);
    if (empty($cid) || empty($pid) || empty($nid))
        return false;
    if (empty($script_name))
        return false;
    $record = "$cid,$pid,$nid,$script_name,0,0,0";
    $table  = _StatBehaviorDefine('page');

    $record_totalpv = "$cid,$pid,$nid,0,0,0";
    $table_totalpv  = _StatBehaviorDefine('totalpv');

    _StatSendMsg($table, $record);
    _StatSendMsg($table_totalpv, $record_totalpv);
}

/**
 * 动作数据采集
 * 动作编号的申请请登录monitor.sdo.com申请
 * return @param boolean
 **/
function StatActionName($action_name) {
	if (!_StatExtensionLoaded())
		return NULL;
	list($channel_id, $parent_id, $type_id, $id) = explode("|", $action_name);
	if (empty($channel_id) || empty($parent_id) || empty($type_id) || empty($id))
		return false;
	$record	= "$channel_id,$parent_id,$type_id,$id,$action_name,0,0,0";
	$table	= _StatBehaviorDefine('action');

	return _StatSendMsg($table, $record);
}

/**
 * 用户信息统计
 * 任何新加入的用户信息统计均告之监控部
 * @param string $flag
 * @param string $content
 * $flag 为统计标识位，全站唯一，因此任何新加的$flag均需申请后使用
 * $content 为发送的内容，如当前用户名，当前IP等信息，若使用中文，则用utf-8编码。
 */
function StatUserinfo($flag, $content) {
	if (!_StatExtensionLoaded())
		return NULL;
	$record	= "$flag,$content,0,0,0";
	$table	= _StatBehaviorDefine('userinfo');

	return _StatSendMsg($table, $record);
}

/**
 * 执行时间统计
 *
 * @param string $action_name
 * @param float @execution_time
 *
 * $action_name 为申请到的标识ID，进入http://monitor.sdo.com/ 后在“数据采集”-->“编号添加查询”-->中添加&查询
 * @execution_time 由于系统不支持减法操作，故程序执行时间由各业务处理，将执行时间做为参数传入
 */
function StatExecutionTime($action_name, $execution_time) {
	if (!_StatExtensionLoaded())
		return NULL;
	list($channel_id, $parent_id, $type_id, $id) = explode("|", $action_name);
	if (empty($channel_id) || empty($parent_id) || empty($type_id) || empty($id))
		return false;

	$record	= "$channel_id,$parent_id,$type_id,$id,$action_name,0,$execution_time,$execution_time,$execution_time,0,0";
	$table	= _StatBehaviorDefine('usedtime');

	return _StatSendMsg($table, $record);
}

/**
 * 在线数据发送.对5分钟内取平均值
 * @param string $flag
 * @param interger $datanum
 *
 * $flag 为标识符，表明是何种在线数据
 * $datanum 为要发送的数据，表明当前在线数据。后续会对5分钟内的数据取平均。
 **/
function StatOnlineUsers($flag, $datanum) {
	if (!_StatExtensionLoaded())
		return NULL;
	$record	= "$flag,$datanum,0,0";
	$table	= _StatBehaviorDefine('online');

	return _StatSendMsg($table, $record);
}

/**
 * 在线数据发送.对5分钟内取平均值
 * @param string $channel_name 业务频道名
 * @param string $app_id	   业务内部唯一标识字符串
 * @param interger $datanum
 *
 * $flag 为标识符，表明是何种在线数据
 * $datanum 为要发送的数据，表明当前在线数据。后续会对5分钟内的数据取平均。
 **/
function StatNumberAvg($channel_name, $app_id, $datanum) {
	if (!_StatExtensionLoaded())
		return NULL;
	$record	= "$channel_name,$app_id,$datanum,0,0";
	$table	= _StatBehaviorDefine('number_avg');

	return _StatSendMsg($table, $record);
	// channel_name KEY_STR, app_id KEY_STR, datanum AVG_INT, happen_time TIME_FLOOR, ip LOCAL_IP",
}

/**
 * 在线数据统计发送
 * @param string $flag
 * @param interger $datanum
 *
 * $flag 为标识符，表明是何种在线数据
 * $datanum 为要发送的数据，表明当前在线用户数
 **/
function StatNumberSum($flag, $app_id, $datanum)
{
	if(!_StatExtensionLoaded())
	{
		return NULL;
	}
	$record = "$flag,$app_id,$datanum,0,0";
	$table  =  _StatBehaviorDefine('number_sum');

	return _StatSendMsg($table, $record);
}

/**
 * 错误监控数据发送
 * @param int 	 $error_id：			必填		错误类型，可向监控部赵志敏要类型-ID对照表，填入对应的ID号。
 * @param string $msg：				必填		错误信息，若使用中文，用utf-8编码。
 * @param int	 $error_cnt：		选填		错误个数。
 * @param date 	 $happen_time：		选填		错误的发生时间，填写执行代码时的时间。时间格式为date。
 *
 **/
function StatReportError($error_id, $msg, $happen_time = '', $error_cnt = 1)
{
	if (!_StatExtensionLoaded())
		return NULL;
	if (empty($happen_time))
	{
		$happen_time = date('Y-m-d H:i:s');
	}
	if ($error_cnt < 1)
	{
		$error_cnt = 1;
	}
	$msg = str_encode($msg);
	$record	= "0,$error_id,301,$happen_time,0,$error_cnt,$msg";
	$table	= _StatBehaviorDefine('reporterror');
	//ip LOCAL_IP, error_id KEY_STR, source KEY_STR, happen_time CONST_STR, receive_time TIME_FLOOR, error_cnt SUM_INT, msg CONST_STR",

	return _StatSendMsg($table, $record);
}

/**
 * receive log information from uc & oa、
 * @param string $function_name：	调用接口名。
 * @param string $status：			返回状态，如OK、WARN、ERROR、FATAL
 * @param int 	 $msg：				错误信息。
 * @param int 	 $responseTime：		调用接口的响应时间（FLOAT）
 * @param int 	 $happen_time：		错误的发生时间。
 * @param int 	 $datanum：
 * @param int 	 $remote_ip：		调用的接口连接的服务器IP，如必要，可带端口
 * @param string $channel：			调用的接口连接的部门标识符
 */
function StatLogOfInterface($function_name, $status, $msg, $responseTime, $happen_time, $remote_ip, $Execute_times,$channel='unkown') {
	if (!_StatExtensionLoaded())
		return NULL;

	$table	= _StatBehaviorDefine('interfaceLog');
	$record	= "$function_name,$status,$msg,$responseTime,$remote_ip,$happen_time,0,$Execute_times,$channel";
	//"tbl_interfacelog(60): function_name KEY_STR, status KEY_STR, msg CONST_STR, responseTime AVG_FLOAT, remote_ip KEY_STR, happen_time CONST_STR, receive_time TIME_FLOOR, datanum CONST_STR"

	return _StatSendMsg($table, $record);
}

/**
 * 发送信息至手机号
 * @param array $phone
 * @param string $message
 * 注意：
 * 因网关限制，186手机号可能无法正常接收此短信
 * $phone 为接入人姓名及手机号数组
 * 如：$phone = array( "13800000000" => "张三01", "13500000000" => "李四01);
 */
function StatSendMessage(array $phones, $message) {

    if (!_StatExtensionLoaded())
        return NULL;
    if (empty($phones) || !is_array($phones))
        return NULL;
    if (empty($message))
        return NULL;

    $message    = trim($message);
	if (empty($message))
		return NULL;
	$message	= iconv_substr($message, 0, 81, "GB18030");
    $rule       = _StatBehaviorDefine("smsmessage");

    foreach ($phones as $phone => $username) {
		$username	= iconv_substr($username, 0, 40, "GB18030");
        $record		= "$username,$phone,$message,0,0,0";
        _StatSendMsg($rule, $record);
    }

	return NULL;
}

/**
 * 打包数据
 * return @param string (binary stream)
 **/
function _StatPack($table_des, $record) {
		/// 打包数据内容
	$data	= pack('n', 0x0)                // 消息类型，QT_DATA
            . pack('n', 0)                  // reserved
            . pack('N', strlen($table_des)) // table_des length
            . $table_des                    // table_des content
            . pack('N', 1)                  // record number
            . pack('N', strlen($record))    // record length
            . $record;                      // record content

        // 生成消息包
    $packet = chr(0x02).chr(0x16)           // head sync
            . pack('n', 0x100)              // version
            . pack('N', strlen($data))      // data length
            . $data;                        // data content

    return $packet;
}

/**
 * 数据发送
 * return @param boolean
 **/
function _StatSendMsg($table, $record) {
	if (!_StatExtensionLoaded())
		return NULL;

	$packet		= _StatPack($table, $record);

	$iMsgId 	= @msg_get_queue(7771);

	return is_resource( $iMsgId ) && @msg_send($iMsgId, 100, $packet, false, false);
}
/**
 * 转义函数，把将要进入ebro的字符串中的',', '\0', '\n', '\r', '^'替换为'^.', '^0', '^n', '^r', '^^'
 **/
function str_encode($str)
{
	//return str_replace(array(',', '\0', '\n', '\r', '^'), array('^.', '^0', '^n', '^r', '^^'), $str);
	$ret = '';
    $len = strlen($str);

	if($len == 0)
	{
		return $ret;
	}
    for ($i = 0; $i < $len; $i++)
    {
        if ($str[$i] == ',')
        {
			$ret .= "^.";
			continue;
        }
        else if ($str[$i] == '\0')
        {
			$ret .= "^0";
			continue;
        }
        else if ($str[$i] == '\n')
        {
			$ret .= "^n";
			continue;
        }
        else if ($str[$i] == '\t')
        {
			$ret .= "^t";
			continue;
        }
        else if ($str[$i] == '\r')
        {
			$ret .= "^r";
			continue;
        }
        else if ($str[$i] == '^')
        {
			$ret .= "^^";
			continue;
        }
        else
        {
			$ret .= $str[$i];
			continue;
	 	}
    }
    //$ret .= '?';
    return $ret;
}
?>