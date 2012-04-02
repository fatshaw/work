<?
/**
 * 本文件用于与MIS服务器交互
 * 
 * @author    徐鹏程 <xupengcheng@snda.com>
 * @version   0.0.1		2010/04/21
 * @note	1、	可以添加多台服务器，采用随机轮询
 *
 * 系统对于用户提交的信息需要调用check方法做一次检查，结果如下
 *     UC_MIS_RESULT_BAD  命中强过滤词，禁止入库
 *     UC_MIS_RESULT_PASS  允许入库
 *     UC_MIS_RESULT_QUEUE 允许入库，但是如果检查时候没有传入record_id，需要调用一次add2Q方法入队列
 *     UC_MIS_RESULT_FREQ_LIMIT 频率限制，禁止入库
 *
 *  理论上来讲，图片在返回UC_MIS_RESULT_PASS的时候也需要调用add2Q入队列
 *
 * 代码Sample:
 *
 * 对于在入库之前可以获取id的数据
 * <code>
 *  $ret = UCLibMISApi::check($user_id, "twitter", $content, $record_id);
 *  switch ($ret) {
 *      case UC_MIS_RESULT_BAD:
 *      ... ... ... ...
 *  }
 * </code>
 *
 * 对于在入库之前不能获取id的数据，操作的流程是 检查 --> 入库 --> 入队列(如果返回UC_MIS_RESULT_QUEUE的话)
 * <code>
 *  $ret = UCLibMISApi::check($user_id, "twitter", $content);
 *  $reason = UCLibMISApi::getReason();
 *  switch ($ret) {
 *      case UC_MIS_RESULT_BAD:
 *      ... ... ... ...
 *      case UC_MIS_RESULT_QUEUE:
 *          $record_id = DO_INSERT_DATABASE();
 *          UCLibMISApi::add2Q($user_id, "twitter", $content, $record_id, $reason);
 *          break;
 *  }
 * </code>
 * 
 * 对于直接入审核队列的操作，可以直接调用add2Q方法
 * <code>
 *      UCLibMISApi::add2Q($user_id, "twitter", $content, $record_id, $reason);
 * </code>
 */

define("UC_MIS_RESULT_BAD", 1);
define("UC_MIS_RESULT_PASS", 2);
define("UC_MIS_RESULT_QUEUE", 3);
define("UC_MIS_RESULT_FREQ_LIMIT", 4);
define("UC_MIS_RESULT_BAD_REQUEST", 99);

define("UC_MIS_CHANNEL_TWITTER", "twitter");
define("UC_MIS_CHANNEL_TWITTER_IMAGE", "twitter_image");
define("UC_MIS_CHANNEL_MESSAGE", "message");
define("UC_MIS_CHANNEL_COMMENT", "comment");
define("UC_MIS_CHANNEL_NICKNAME", "nickname");
define("UC_MIS_CHANNEL_AVATAR", "avatar");
define("UC_MIS_CHANNEL_PHOTO_TEXT", "photo_text");
define("UC_MIS_CHANNEL_PHOTO_IMAGE", "photo_image");
define("UC_MIS_CHANNEL_BBS", "bbs");
define("UC_MIS_CHANNEL_DEFAULT", "default");

require_once  dirname(__FILE__).'/UCFuncUtils.php';
require_once(dirname(__FILE__).'/UCLibLogger.php');
require_once(dirname(__FILE__).'/../config/UCConfMIS.php');

if (!isset($GLOBALS['THRIFT_ROOT'])) $GLOBALS['THRIFT_ROOT'] =  dirname(__FILE__)."/thrift";
require_once $GLOBALS['THRIFT_ROOT'].'/Thrift.php';
require_once $GLOBALS['THRIFT_ROOT'].'/protocol/TBinaryProtocol.php';
require_once $GLOBALS['THRIFT_ROOT'].'/transport/TSocket.php';
require_once $GLOBALS['THRIFT_ROOT'].'/transport/THttpClient.php';
require_once $GLOBALS['THRIFT_ROOT'].'/transport/TFramedTransport.php';
require_once $GLOBALS['THRIFT_ROOT'].'/packages/mis/MISInterface.php';
require_once $GLOBALS['THRIFT_ROOT'].'/packages/mis/mis_types.php';

/**
 * UCLibMISApi 接口定义
 */
class UCLibMISApi {
	private $_servers;

	private static $_last_reason;

	private static $_send_timeout = 100;
	private static $_rev_timeout = 400;

	/**
	 * 构造函数
	 */
	function __construct() {
		$this->_servers = UCConfMIS::getServers();
	}
	
	
	/**
	 * 添加一个服务器
	 * @param ip: 服务器地址
	 * @param port: 服务器端口
	 */
	public function addServer($host, $port) {
		$this->_servers []= array($host, $port);
	}

	/**
	 * 向MIS服务器添加一个检查请求
	 * @param user_id: 用户id
	 * @param channel: 频道号
	 * @param content: 检查内容
	 * @param record_id: 纪录id
	 * @param reason: 原因
	 * @param ip: IP
	 * @return 如果出错，返回false, 否则 true
	 */
	protected function _add2Q($user_id, $channel, $content, $record_id, $reason, $ip = null) {
		$_time_start = microtime(true);
		$input = new MISRequest();
		$input->record_id = $record_id ? strval($record_id) : "";
		$input->user_id = strval($user_id);
		$input->ip = $ip ? $ip : get_client_ip();
		$input->channel = $channel;
		$input->content = strval($content);
		if (!$reason) $reason = "";

		$cnt = count($this->_servers);
		$index = mt_rand(0, $cnt - 1);
		$i = 0;
		$result = false;
		$host = null;
		$port = null;
		while ($i < $cnt) {
			try {
				$host = $this->_servers[$index][0];
				$port = $this->_servers[$index][1];
				$socket = new TSocket($host, $port);
				$socket->setSendTimeout(self::$_send_timeout);
				$socket->setRecvTimeout(self::$_rev_timeout);
				$transport = new TFramedTransport($socket);
				if (function_exists('thrift_protocol_write_binary')) {
					$protocol = new TBinaryProtocolAccelerated($transport);
				} else {
					$protocol = new TBinaryProtocol($transport);
				}
				$client = new MISInterfaceClient($protocol);
				$transport->open();
				$result = $client->add2Q($input, $reason);
				$transport->close();
				UCLibLogger::rpc("add2Q:".$channel.':'.$user_id.":".$record_id, microtime(true) - $_time_start, $host, $port, UCLIB_LOG_RPC_TYPE_MIS);
				return true;
			} catch (Exception $e) {
			}
			$i += 1;
			$index = ($index + 1) % $cnt;
		}
		UCLibLogger::rpc("add2Q:".$channel.':'.$user_id.":".$record_id, microtime(true) - $_time_start, '', '', UCLIB_LOG_RPC_TYPE_MIS);
		return false;
	}

	/**
	 * 进行一次检查
	 * @param user_id: 用户id
	 * @param channel: 频道号
	 * @param content: 检查内容
	 * @param record_id: 纪录id
	 * @param ip: IP
	 * @return 结果是一个数字,如果出错，返回false, 否则，结果为下面的一种
	 *     UC_MIS_RESULT_BAD
	 *     UC_MIS_RESULT_PASS
	 *     UC_MIS_RESULT_QUEUE
	 *     UC_MIS_RESULT_FREQ_LIMIT
	 */
	protected function _check($user_id, $channel, $content, $record_id, $ip) {
		$_time_start = microtime(true);
		$result = false;
		$input = new MISRequest();
		$input->record_id = $record_id ? strval($record_id) : "";
		$input->user_id = strval($user_id);
		$input->ip = $ip ? $ip : get_client_ip();
		$input->channel = $channel;
		$input->content = strval($content);

		$cnt = count($this->_servers);
		$index = mt_rand(0, $cnt - 1);
		$i = 0;
		$result = false;
		$host = null;
		$port = null;
		while ($i < $cnt) {
			try {
				$host = $this->_servers[$index][0];
				$port = $this->_servers[$index][1];
				$socket = new TSocket($host, $port);
				$socket->setSendTimeout(self::$_send_timeout);
				$socket->setRecvTimeout(self::$_rev_timeout);
				$transport = new TFramedTransport($socket);
				if (function_exists('thrift_protocol_write_binary')) {
					$protocol = new TBinaryProtocolAccelerated($transport);
				} else {
					$protocol = new TBinaryProtocol($transport);
				}
				$client = new MISInterfaceClient($protocol);
				$transport->open();

				$result = $client->check($input);
				$transport->close();
				self::$_last_reason = $result->reason;
				UCLibLogger::rpc("check:".$channel.':'.$user_id.":".$record_id, microtime(true) - $_time_start, $host, $port, UCLIB_LOG_RPC_TYPE_MIS);
				return $result->result;
			} catch (Exception $e) {
			}
			$i += 1;
			$index = ($index + 1) % $cnt;
		}
		UCLibLogger::rpc("check:".$channel.':'.$user_id.":".$record_id, microtime(true) - $_time_start, '', '', UCLIB_LOG_RPC_TYPE_MIS);
		return false;
	}

	protected function _reload() {	
		foreach ($this->_servers as $server) {
			try {
				$host = $server[0];
				$port = $server[1];
				$socket = new TSocket($host, $port);
				$transport = new TFramedTransport($socket);
				if (function_exists('thrift_protocol_write_binary')) {
					$protocol = new TBinaryProtocolAccelerated($transport);
				} else {
					$protocol = new TBinaryProtocol($transport);
				}
				$client = new MISInterfaceClient($protocol);
				$transport->open();

				$client->reload();
				$transport->close();
			} catch (Exception $e) {
			}
		}
	}

	protected function _ping() {
		$ret = array();
		foreach ($this->_servers as $server) {
			$host = $server[0];
			$port = $server[1];
			try {
				$socket = new TSocket($host, $port);
				$transport = new TFramedTransport($socket);
				if (function_exists('thrift_protocol_write_binary')) {
					$protocol = new TBinaryProtocolAccelerated($transport);
				} else {
					$protocol = new TBinaryProtocol($transport);
				}
				$client = new MISInterfaceClient($protocol);
				$transport->open();
				$status = $client->ping();
				$transport->close();
				$ret[$host.":".$port] = $status;
			} catch (Exception $e) {
				$ret[$host.":".$port] = 0;
			}
		}
		return $ret;
	}
	/**
	 * 进行一次检查
	 * @param user_id: 用户id
	 * @param channel: 频道号
	 * @param content: 检查内容
	 * @param record_id: 纪录id
	 * @param ip: IP
	 * @return 结果是一个数字,如果出错，返回false, 否则，结果为下面的一种
	 *     UC_MIS_RESULT_BAD
	 *     UC_MIS_RESULT_PASS
	 *     UC_MIS_RESULT_QUEUE
	 *     UC_MIS_RESULT_FREQ_LIMIT
	 * 注意如果没有传入rec_id,并且返回结果为UC_MIS_RESULT_QUEUE
	 * 一定要在纪录创建之后重新调用一次这个函数把rec_id传进来
	 */	
	public static function check($user_id, $channel, $content, $record_id = null, $ip = null) {
		$service = new UCLibMISApi();
		return $service->_check($user_id, $channel, $content, $record_id, $ip);
	}

	/**
	 * 获取最后一次的检查原因
	 */	
	public static function getReason() {
		return self::$_last_reason;
	}

	/**
	 * 向MIS服务器添加一个检查请求
	 * @param user_id: 用户id
	 * @param channel: 频道号
	 * @param content: 检查内容
	 * @param record_id: 纪录id
	 * @param reason: 原因
	 * @param ip: IP
	 * @return 如果出错，返回false, 否则 true
	 */
	public static function add2Q($user_id, $channel, $content, $record_id, $reason, $ip = null) {
		$service = new UCLibMISApi();
		return $service->_add2Q($user_id, $channel, $content, $record_id, $reason, $ip);
	}

	/**
	 * 向MIS服务器发送reload请求
	 */
	public static function reload() {
		$service = new UCLibMISApi();
		$service->_reload();
	}

	/**
	 * Ping MIS服务器
	 */
	public static function ping() {
		$service = new UCLibMISApi();
		return $service->_ping();
	}
}
