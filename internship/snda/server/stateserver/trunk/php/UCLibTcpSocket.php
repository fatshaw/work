<?php

require_once(dirname(__FILE__) . "/UCLibSocket.php");

class UCLibTcpSocket extends UCLibSocket
{
	private $_connected;
	function __construct($domain = AF_INET, $protocol = SOL_TCP) 
	{
		parent::__construct($domain, SOCK_STREAM, $protocol);
		$this->_connected = false;
	}

	function __destruct()
	{
		$this->disconnect();
		parent::__destruct();
	}

	/** 
	* @brief 判断是否已经建立连接
	* @return true or false
	*/
	function isConnected()
	{
		return $this->_connected;
	}

	/** 
	* @brief 连接指定地址
	* @param[in] address 地址
	* @param[in] port 端口
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @note 如果TcpConn没有Open()，则Connect()会先调用Open()。 如果有如绑定本端IP、设置接收缓冲区等需求，可以先Open()、Bind()后再调用Connect()
	* @return true or false
	*/
	function connect($remote_addr, $remote_port, $timeout)
	{	
		if ( $this->isConnected() ) {
			return false;
		}

		$auto_open = false;
		if ( !$this->isOpen() ) {
			if ( !$this->open() ) {
				return false;
			}
			$auto_open = true;
		}

		if ( !$this->setBlock(false) ) {
			if ( $auto_open ) {
				$this->close();
			}
			return false;
		}
	
		if ( @socket_connect($this->_getSocket(), $remote_addr, $remote_port) === false && $this->getLastError() != SOCKET_EINPROGRESS &&  $this->getLastError() !=  SOCKET_EWOULDBLOCK) {
			return false;
		}
		
		$this->clearError();
		$timeout = new UCLibTimeout($timeout);

		while ( 1 ) {

			if ( $timeout->isTimeout() ) {
				$this->_setError(SOCKET_ETIME);			
				break;
			}

			$sockets = array($this->_getSocket());                

			$ret = @socket_select($sockets, $sockets, $sockets, $timeout->getSec(), $timeout->getUSec());
			if ( $ret === false ) {
				$this->_setError(socket_last_error());
				if ( $this->getLastError() != SOCKET_EINTR ) {
					break;
				}
			}
			elseif ( $ret == 0 ) {
				$this->_setError(SOCKET_ETIME);
				break;
			}
			elseif ( $ret == 1 ) { // succ
				$this->_connected = true;
				break;
			}
			else {
				$this->_setError($this->getSockOption(SOL_SOCKET , SO_ERROR));
				break;
			}
		}

		if ( !$this->_connected ) {
			if ( $auto_open ) {
				$this->close();
			}
			return false;		
		}
		else {
			return true;
		}
	}

	/** 
	* @brief 关闭当前连接
	* @return void
	*/
	function disconnect()
	{
		if ( $this->isConnected() ) {
			$this->close();
			$this->_connected = false;
		}
	}

	/** 
	* @brief 获取对端地址端口。连接时有效
	* @param[out] addr 地址
	* @param[out] port 端口
	* @return true or false
	*/
	function getPeerName(&$addr, &$port)
	{
		return @socket_getpeername($this->_getSocket(), $addr, $port);
	}

	/** 
	* @brief 发送数据。退出条件 ( 调用了一次socket_write | 超时 | 出错 )
	* @param[in] buffer 待发送数据
	* @param[in] len 数据长度
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @return false:失败 >=0:发送的字节数
	*/
	function writeOnce($buffer, $len, $timeout)
	{
		if ( !$this->isConnected() ) {
			return false;
		}

		if( !is_a($timeout, 'UCLibTimeout') ) {
            $timeout = new UCLibTimeout($timeout);         
        }

		while ( 1 ) {

			if ( $timeout->isTimeout() ) {   
				$this->_setError(SOCKET_ETIME);			
				break;
			}

			$read   = NULL;
			$write  = array($this->_getSocket());
			$except = NULL;                                     
            #var_dump($timeout->getSec(), $timeout->getUSec());
			$ret = @socket_select($read, $write, $except, $timeout->getSec(), $timeout->getUSec());
			if ( $ret === false ) {
				$this->_setError(socket_last_error());
				if ( $this->getLastError() != SOCKET_EINTR ) {
					return false;
				}
			}
			elseif ( $ret === 0 ) {	// timeout
				$this->_setError(SOCKET_ETIME);
				return 0;
			}
			else {	// could write
				$write_result = @socket_write($this->_getSocket(), $buffer, $len);
				if ( $write_result === false ) {
					if ( $this->getLastError() != SOCKET_EWOULDBLOCK ) {
						return false;
					}
				}
				else {
					return $write_result;
				}
			}
		}
	}	

	/** 
	* @brief 接收数据。退出条件 ( 调用了一次socket_read | 超时 | 出错 )
	* @param[in] max_len 最大数据长度
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @return false:失败 string:接收到的数据
	*/
	function readOnce($max_len, $timeout)
	{
		if ( !$this->isConnected() ) {
			return false;
		}

		if( !is_a($timeout, 'UCLibTimeout') ) {
            $timeout = new UCLibTimeout($timeout);         
        }

		while (1) {		

			if ( $timeout->isTimeout() ) {
				$this->_setError(SOCKET_ETIME);			
				return false;
			}

			$read   = array($this->_getSocket());
			$write  = NULL;
			$except = NULL;                                     
			$ret = @socket_select($read, $write, $except, $timeout->getSec(), $timeout->getUSec());
			if ( $ret === false ) {
				$this->_setError(socket_last_error());			
				if ( $this->getLastError() != SOCKET_EINTR ) {
					return false;
				}
			}
			elseif ( $ret === 0 ) {	// timeout
				$this->_setError(SOCKET_ETIME);
				return false;
				//return '';
			}
			else {	// could read

				$read_result = @socket_read($this->_getSocket(), $max_len);
				if ( $read_result === false ) {
					if ( $this->getLastError() != SOCKET_EWOULDBLOCK ) {
						return false;
					}
				}
				else {
					return $read_result;
				}
			}                                                                     
		}
	}


	/** 
	* @brief 发送指定长度的数据。退出条件 ( 数据全部发送完毕 | 超时 | 出错 )
	* @param[in] buffer 待发送数据
	* @param[in] len 数据长度
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @return false:失败 >=0:发送的字节数
	*/
	function write($buffer, $len, $timeout)
	{	
		if ( !$this->isConnected() ) {
			return false;
		}

		$left_bytes = strlen($buffer);
		
		if ( $left_bytes > $len ) {
			$left_bytes = $len;
		}
		else {
			$len = $left_bytes;
		}

		if( !is_a($timeout, 'UCLibTimeout') ) {     
            $timeout = new UCLibTimeout($timeout);         
        } 

		while ( $left_bytes > 0 ) {

			if ( $timeout->isTimeout() ) {
				$this->_setError(SOCKET_ETIME);			
				return false;
			}
			$once_result = $this->writeOnce($buffer, $left_bytes, $timeout);
			if ( $once_result === false ) {  // Be sure to use the === operator to check for FALSE in case of an error. 
				return false;
			}

			if ( $once_result === 0 ) { // connection closed by peer or timeout
				break;
			}

			$buffer = substr($buffer, $once_result);
			$left_bytes -= $once_result;
		}

		return $len - $left_bytes;
	}
	
	/** 
	* @brief 接收指定长度的数据。退出条件 ( 接收到指定长度的数据 | 超时 | 出错 )
	* @param[in] recv_len 接收数据长度
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @param[in] total_timeout 设置为true表示timeout是本次发送n个数据的总超时时间; 否则表示每次调用WriteOnce()所设置的单次超时时间
	* @return false:失败 string:接收到的数据
	*/
	function read($recv_len, $timeout)
	{
		if ( !$this->isConnected() ) {
			return false;
		}

		$read_result = '';
		$left_bytes = $recv_len;

        if( !is_a($timeout, 'UCLibTimeout') ) {
		    $timeout = new UCLibTimeout($timeout); 		
        }

		while ( $left_bytes > 0 ) {

			if ( $timeout->isTimeout() ) {
				$this->_setError(SOCKET_ETIME);			
				return false;
			}

			$once_result = $this->readOnce($left_bytes, $timeout);
			if ( $once_result === false ) {
				return false;
			}

			$read_len = strlen($once_result);
			if ( $read_len == 0 ) { 
				break;	// connection closed by peer or timeout
			}
			
			$read_result = $read_result.$once_result;
			$left_bytes -= $read_len;
		}

		return $read_result;
	}
}
/*
for($i=0; $i<1; $i++){
    $conn = new UCLibTcpSocket();
    $r = $conn->connect('127.0.0.1', 9000, 0.001);
    $r = $conn->write('abc', 3, 0.0001); 
    var_dump($r, $conn->getLastErrorMsg());
}   
*/          
?>
