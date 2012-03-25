<?php
//=============================================================================
/**
 *  @file       fiveonelib_udp_dgram.php
 *  @brief      udp类
 *  @version    1.2
 *  @author     Bob
 *  @date       2008/07/29
 */
//=============================================================================

require_once(dirname(__FILE__) . "/UCLibSocket.php");

class UCLibTcpSocket extends UCLibSocket
{
	function __construct() 
	{
		return parent::__construct(AF_INET, SOCK_DGRAM, SOL_UDP);
	}

	function UCLibTcpSocket()
	{
		$this->__construct();
	}

	function __destruct()
	{
		parent::__destruct();
	}

	//function IsOpen();

	//function Open();

	//function Close();


	/** 
	* @brief 发送数据。如果socket没有open()则会自动先调用Open()
	* @param[in] buf 发送buffer
	* @param[in] len 发送len
	* @param[in] addr 对端地址
	* @param[in] port 对端端口
	* @param[in] flag 同socket_sendto()参数flag
	* @return -1:失败 >=0:发送的字节数
	*/
	function sendTo($buf, $len, $addr, $port, $flags=0)
	{
		if ( !$this->isOpen() ) {
			if ( !$this->open() ) {
				return -1;
			}
		}                                     
		return @socket_sendto($this->_getSocket(), $buf, $len, $flag, $addr, $port);
	}

	/** 
	* @brief 接收数据。退出条件 ( 接收到数据 | 超时 | 出错 )
	* @param[out] buf 接收buffer
	* @param[in] len 最大长度
	* @param[out] addr 对端地址
	* @param[out] port 对端端口
	* @param[in] timeout 超时时间。可以为以下4种形式: 1.秒数(int); 2.精确到微秒的浮点数; 3. array('sec','usec'); 4. TimeValue object
	* @param[in] flag 同socket_recvfrom()参数flag
	* @return -1:失败 >=0:接收到的字节数
	*/
	function recvFrom(&$buf, $len, &$addr, &$port, $timeout, $flags=0)
	{
		if ( !$this->isOpen() ) {
			return -1;
		}

		$timeout = new UCLibTimeout($timeout);

		while (1) {

			if ( $timeout->isTimeout() ) {
				$this->_setError(SOCKET_ETIME);			
				return 0;
			}

			$read   = array($this->_getSocket());
			$write  = NULL;
			$except = NULL;                                     
			$ret = @socket_select($read, $write, $except, $timeout->getSec(), $timeout->getUSec());

			if ( $ret === false ) {
				$this->_setError(socket_last_error());
				if ( $this->getLastError() != SOCKET_EINTR ) {
					return -1;
				}
			}
			elseif ( $ret === 0 ) {	// timeout
				$this->_setError(SOCKET_ETIME);
				return 0;
			}
			else {	// could read
				$recv_bytes = @socket_recvfrom($this->_getSocket(), $buf, $len, $flags, $addr, $port);
				if ( $recv_bytes == -1 ) {
					if ( $this->getLastError() != SOCKET_EWOULDBLOCK ) {
						return -1;
					}
				}
				else {
					return $recv_bytes;
				}
			}	
		}
	}
}

?>
