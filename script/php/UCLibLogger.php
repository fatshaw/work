<?php
/**  
 * @package sdk_uclib.UCLibLogger
 * @target 日志封装
 * @author jiangjie
 * @since  2009.02.10
 * @example
 * <code> 
 *   UCLibLogger::setMask(UCLIB_LOG_PRI_ALL);
 *   UCLibLogger::attach(new StdoutLogOb());
 *   // optional: StdoutLogOb(), JsonpLogOb(),
 *   UCLibLogger::log('i am a log');
 *   UCLibLogger::debug('i am a debug log');
 * </code>
 **/
define('UCLIB_LOG_PRI_EMERG',    1);     /* System is unusable */
define('UCLIB_LOG_PRI_ALERT',    2);     /* Immediate action required */
define('UCLIB_LOG_PRI_CRITICAL',     4);     /* Critical conditions */
define('UCLIB_LOG_PRI_ERROR',      8);     /* Error conditions */
define('UCLIB_LOG_PRI_WARNING',  16);     /* Warning conditions */
define('UCLIB_LOG_PRI_NOTICE',   32);     /* Normal but significant */
define('UCLIB_LOG_PRI_INFO',     64);     /* Informational */
define('UCLIB_LOG_PRI_DEBUG',    128);     /* Debug-level messages */
define('UCLIB_LOG_PRI_RPC_DEBUG',    256);     /* Debug-level messages */

define('UCLIB_LOG_PRI_ALL',      0xffffffff);    /* All messages */
define('UCLIB_LOG_PRI_NONE',     0x00000000);    /* No message */

define('UCLIB_LOG_RPC_TYPE_MYSQL', 'mysql');
define('UCLIB_LOG_RPC_TYPE_TT', 'tt');
define('UCLIB_LOG_RPC_TYPE_MEMCACHE', 'memcache');
define('UCLIB_LOG_RPC_TYPE_UNIFIED', 'unified');
define('UCLIB_LOG_RPC_TYPE_SEARCH', 'search'); 
define('UCLIB_LOG_RPC_TYPE_MIS', 'mis');   
define('UCLIB_LOG_RPC_TYPE_GRAPH', 'graph'); 

function _is_log_pri_masked($priority, $mask)
{
    return $priority & $mask;
}

abstract class UCLibLogObserver
{
    protected $_id = 0;
    protected $_mask = UCLIB_LOG_PRI_ALL;
    
    public function setMask($mask) 
    {
        $this->_mask = $mask;
        return $this->_mask;
    }
    
    public function isMasked($pri)
    {
        return _is_log_pri_masked($pri, $this->_mask);
    }
    
    public function getId()
    {
        if( !$this->_id )
            $this->_id = md5(uniqid(rand(), true));
        return $this->_id;
    }
    
    protected function _priorityToString($priority)
    {
        $levels = array(
            UCLIB_LOG_PRI_EMERG   => 'emergency',
            UCLIB_LOG_PRI_ALERT   => 'alert',
            UCLIB_LOG_PRI_CRITICAL    => 'critical',
            UCLIB_LOG_PRI_ERROR     => 'error',
            UCLIB_LOG_PRI_WARNING => 'warning',
            UCLIB_LOG_PRI_NOTICE  => 'notice',
            UCLIB_LOG_PRI_INFO    => 'info',
            UCLIB_LOG_PRI_DEBUG   => 'debug',
            UCLIB_LOG_PRI_RPC_DEBUG => 'rpc'
        );

        return $levels[$priority];
    }
    
    protected function _stringToPriority($name)
    {
        $levels = array(
            'emergency' => UCLIB_LOG_PRI_EMERG,
            'alert'     => UCLIB_LOG_PRI_ALERT,
            'critical'  => UCLIB_LOG_PRI_CRITICAL,
            'error'     => UCLIB_LOG_PRI_ERROR,
            'warning'   => UCLIB_LOG_PRI_WARNING,
            'notice'    => UCLIB_LOG_PRI_NOTICE,
            'info'      => UCLIB_LOG_PRI_INFO,
            'debug'     => UCLIB_LOG_PRI_DEBUG,
            'rpc'       => UCLIB_LOG_PRI_RPC_DEBUG
        );

        return $levels[strtolower($name)];
    }
    
    /**
    * @params $msg 日志字符串
    */
    abstract function notify($msg, $db, $pri, $params);
}

class UCLibLogger 
{
    static private $_listeners = array();
    static private $_priority = UCLIB_LOG_PRI_INFO;
    static private $_mask = UCLIB_LOG_PRI_ALL;
    
    static public function setMask($mask) 
    {
        self::$_mask = $mask;
        return self::$_mask;
    }
    
    static public function isMasked($pri)
    {
        return _is_log_pri_masked($pri, self::$_mask);
    }
    
    static public function attach(&$observer)
    {
        if (!is_a($observer, 'UCLibLogObserver')) {
            return false;
        }
        self::$_listeners[$observer->getId()] = &$observer;
        return true;
    }
    
    static public function detach($observer)
    {
        if (!is_a($observer, 'UCLibLogObserver') ||
            !isset(self::$_listeners[$observer->getId()])) {
            return false;
        }
        unset(self::$_listeners[$observer->getId()]);
        return true;
    }
    
    static private function _announce($msg, $db, $pri, $params=array())
    {
        foreach (self::$_listeners as $id => $listener) {
            if ( self::isMasked($pri) && self::$_listeners[$id]->isMasked($pri) ) {
                self::$_listeners[$id]->notify($msg, $db, $pri, $params);
            }
        }
    }
    /**
    * @todo 多维数组的支持
    */
    static private function _formatToString($message)
    {
        if (is_object($message)) {
            if (method_exists($message, 'getmessage')) {
                $message = $message->getMessage();
            } else if (method_exists($message, 'tostring')) {
                $message = $message->toString();
            } else if (method_exists($message, '__tostring')) {
                if (version_compare(PHP_VERSION, '5.0.0', 'ge')) {
                    $message = (string)$message;
                } else {
                    $message = $message->__toString();
                }
            } else {
                $message = var_export($message, true);
            }
        } else if (is_array($message)) {
            if (isset($message['message'])) {
                if (is_scalar($message['message'])) {
                    $message = $message['message'];
                } else {
                    $message = var_export($message['message'], true);
                }
            } else {
                $message = var_export($message, true);
            }
        } else if (is_bool($message) || $message === NULL) {
            $message = var_export($message, true);
        }
        return $message;
    }
    
	static public function log($msg, $priority = UCLIB_LOG_PRI_INFO, $params=array())
    {
        $db = debug_backtrace();
        $c_db = array();
        if( is_array($db) ) {
            if( count($db) == 1 ) {
                $c_db['line'] = $db[0]['line'];
                $c_db['file'] = $db[0]['file'];
                $c_db['function'] = null;
                $c_db['args'] = null;
            }
            else {
                if( $priority != UCLIB_LOG_PRI_RPC_DEBUG ) {
                    $c_db['line'] = $db[1]['line'];
                    $c_db['file'] = $db[1]['file'];
                    $c_db['function'] = $db[1]['function'];
                    $c_db['args'] = $db[1]['args'];
                } else {
                    for($i=0; $i<count($db); $i++) {
                        if( stristr($db[$i]['function'], 'action') && stristr($db[$i]['class'], 'controller') ) {
                            $c_db['line'] = ':'.$db[$i]['function'].':'.$db[$i-1]['line'].' @'.session_id();
                            //$c_db['file'] = $db[$i-1]['file'];
                            $c_db['file'] = $db[$i]['class'];
                            $c_db['function'] = $db[$i]['function'];
                            $c_db['args'] = $db[$i]['args'];
                            break;
                        }
                    }
                }
            }
        }
        
        $msg = self::_formatToString($msg);
        
        self::_announce($msg, $c_db, $priority, $params);
    }
    
    static public function emergency($message)
    {
        return self::log($message, UCLIB_LOG_PRI_EMERG);
    }
    static public function alert($message)
    {
        return self::log($message, UCLIB_LOG_PRI_ALERT);
    }
    /**
    * @desc 严重错误，一般需要上报(比如mysql连不上)
    */
    static public function critical($message)
    {
        return self::log($message, UCLIB_LOG_PRI_CRITICAL);
    }
    /**
    * @desc 一般错误，且无法继续操作
    */
    static public function error($message)
    {
        return self::log($message, UCLIB_LOG_PRI_ERROR);
    }
    /**
    * @desc 不错误，但仍可继续操作（比如取exif信息失败）
    */
    static public function warning($message)
    {
        return self::log($message, UCLIB_LOG_PRI_WARNING);
    }
    static public function notice($message)
    {
        return self::log($message, UCLIB_LOG_PRI_NOTICE);
    }
    static public function info($message)
    {
        return self::log($message, UCLIB_LOG_PRI_INFO);
    }
    /**
    * @desc 调试信息，建议在一些关键步骤把一些关键信息打出来，供以后开发调试，上线时关闭
    */
    static public function debug($message)
    {
        return self::log($message, UCLIB_LOG_PRI_DEBUG);
    }

	static public function getSearch($action)
	{
		return $action; 
	}

	static public function getTT($channel,$action)
	{
		return $channel.'.'.$action; 
	}

	static public function getCache($channel,$action)
	{
		return $channel.'.'.$action; 
	}

	static public function getDB($db,$table,$sql)
	{
		$s = $db . '.' . $table;
		if(preg_match('/select/i',$sql))
		{
			$s .= '.select';
		}
		else if(preg_match('/update/i',$sql))
		{
			$s .= '.update';
		} 
		else if(preg_match('/insert/i',$sql))
		{
			$s .= '.insert';
		}

		else if(preg_match('/connect/i',$sql))
		{
			$s .= '.connect';
		}
		return $s;
	}
    
    /**
    * 记录所有远程调用的消耗时间
    * 
    * @param string $info 需要记录的信息（比如SQL、read/write、统一XX的API name等）
    * @param string $consumed_time 远程调用消耗的时间
    * @param string $remote_ip   远程服务器IP
    * @param int $port    远程服务器端口
    * @param macro(string) $type    远程调用的类型，见宏
    */
    static public function rpc($info, $consumed_time, $remote_ip, $port, $type)
    {
        $consumed_time = sprintf('%.4f', $consumed_time);
        if( $consumed_time < 0.01 ) 
            $consumed_str = "\033[0;32;40m<$consumed_time>\033[0m";
        elseif( $consumed_time < 0.1 )
            $consumed_str = "\033[0;36;40m<$consumed_time>\033[0m";
        elseif( $consumed_time < 0.5 )
            $consumed_str = "\033[0;33;40m<$consumed_time>\033[0m";
        else
            $consumed_str = "\033[0;31;40m<$consumed_time>\033[0m";
        $message = ">$remote_ip:$port \33[4m[$type]\033[0m $consumed_str $info";
        $message = str_replace('Error', "\033[0;31;40mError\033[0m", $message);
        return self::log($message, UCLIB_LOG_PRI_RPC_DEBUG, array('consumed_time'=> $consumed_time));
    }
};

class StdoutLogOb extends UCLibLogObserver
{ 
    public function __construct(){
        $this->setMask(UCLIB_LOG_PRI_ALL^UCLIB_LOG_PRI_RPC_DEBUG);
    }
    
    public function notify($msg, $db, $pri, $params)
    {
        $pri_str = $this->_priorityToString($pri);
        $datetime = date("Y-m-d H:i:s");
        echo "[$datetime][$pri_str]$msg ({$db['file']}:{$db['line']})<br/>\n";
    }
}

class JsonpLogOb extends UCLibLogObserver
{  
    public function __construct(){
        $this->setMask(UCLIB_LOG_PRI_ALL^UCLIB_LOG_PRI_RPC_DEBUG);
    }
    public function notify($msg, $db, $pri, $params)
    {
        $pri_str = $this->_priorityToString($pri);
        $datetime = date("Y-m-d H:i:s");
        echo "/*[$datetime][$pri_str]$msg ({$db['file']}:{$db['line']})*/\n";
    }
}

class HtmlCmtLogOb extends UCLibLogObserver
{     
    public function __construct() { 
        $this->setMask(UCLIB_LOG_PRI_ALL^UCLIB_LOG_PRI_RPC_DEBUG);
        echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">'."\n";
    }
    public function notify($msg, $db, $pri, $params)
    {
        $pri_str = $this->_priorityToString($pri);
        $datetime = date("Y-m-d H:i:s");
        echo "<!--[$datetime][$pri_str]$msg ({$db['file']}:{$db['line']})-->\n";
    }
}

class UdpLogOb extends UCLibLogObserver
{ 
    private $_ip = '127.0.0.1';
    private $_port = '30001';
    private $_rate = 1;
    private $_threshold = 0.1;
    
    public function __construct($ip, $port)
    {   
        $this->_ip = $ip;
        $this->_port = $port;
    }
    
    /**
    * 由于UDP日志量会很大，这里可以设置它的采样频率
    * 
    * @param int $rate [0.0001-1]
    * @param float $time_threshold 采样的阈值，即请求时间大于该值的才进行采样
    */
    public function setRate($rate = 1, $time_threshold = 0.1)
    {
        $rate = intval($rate*10000);
        if($rate < 0) $rate = 0;
        if($rate > 10000) $rate = 10000;
        $this->_rate = $rate;
        
        $this->_threshold = $time_threshold;
    }
    
    public function notify($msg, $db, $pri, $params)
    {
        if( mt_rand(0, 10000) > $this->_rate )
            return; 
        $tm = floatval($params['consumed_time']);
        if( $tm < $this->_threshold && !strstr($msg, 'Error') )
            return;
        $pri_str = $this->_priorityToString($pri);
        $content = "[$pri_str]$msg ({$db['file']}:{$db['line']})";
        
        $socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
        if (!$socket) 
        {
            return false;
        }
        socket_set_option($socket, SOL_SOCKET, SO_SNDTIMEO, array("sec" => 0, "usec" => 100000));
        $result = socket_connect($socket, $this->_ip, $this->_port);
        if ($result && is_resource($socket)) 
        {
            socket_write($socket, $content, strlen($content));
        }
        socket_close($socket);
    }
}

class HtmlLogOb extends UCLibLogObserver
{                         
    public function __construct() {
        $this->setMask(UCLIB_LOG_PRI_ALL^UCLIB_LOG_PRI_RPC_DEBUG);
        echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">'."\n";
    }
    private function _getHtmlPriority($pri)
    {
        $background_color = '';
        $pri_str = $this->_priorityToString($pri);
        switch($pri)
        {
        case UCLIB_LOG_PRI_DEBUG: $background_color = '#ADFF2F'; break;
        case UCLIB_LOG_PRI_INFO: $background_color = '#C0FFC0'; break;
        case UCLIB_LOG_PRI_WARNING: $background_color = '#F58516'; break;
        case UCLIB_LOG_PRI_ERROR: $background_color = '#E32F0B'; break;
        }
        return "<font style='background-color:$background_color'>$pri_str</font>";
    }
    
    public function notify($msg, $db, $pri, $params)
    {
        $pri_str = $this->_getHtmlPriority($pri);
        $datetime = date("Y-m-d H:i:s");
        $file_line = "<span style='color:#808080'>({$db['file']}:{$db['line']})</span>";
        $pre_before = '';
        $pre_after = '';
        if( substr_count ($msg, "\n") > 1 ) {
            $pre_before = '<pre style="font-size:12px;margin:0px;padding:0em">';
            $pre_after = '</pre>';
        }
        else {
            $msg = htmlentities($msg, ENT_COMPAT, 'UTF-8');
        }
            
        echo "<div style='font-size:14px;margin:1px;padding:0.1em'><span style='color:blue'>[$datetime]</span>"
            ."$pri_str $pre_before $msg $pre_after $file_line</div>";
    }
}

class FileLogOb extends UCLibLogObserver
{
    private $_filepath = null;
    
    public function __construct($name)
    {    
        $this->setMask(UCLIB_LOG_PRI_ALL^UCLIB_LOG_PRI_RPC_DEBUG);
        $this->_filepath = "/tmp/uclog_$name.log";
    }
    
    public function notify($msg, $db, $pri, $params){
        $pri_str = $this->_priorityToString($pri);
        $datetime = date("Y-m-d H:i:s");
        $str = "[$datetime][$pri_str]$msg ({$db['file']}:{$db['line']})\n";
        error_log($str, 3, $this->_filepath);
    }
}
/*
UCLibLogger::attach(new UdpLogOb('114.80.132.20', 19001));
//$logger->attach(new FileLogOb());
UCLibLogger::log('aaaaaaaaa');

function abc($a)
{
    UCLibLogger::critical('bbb');
}
function def()
{
    abc('1');
}
function sss()
{
    def();
}
sss();
*/
