<?php

require_once('./UCLibTcpSocket.php');
require_once('./NetUtil.php');

interface PackageSplitter
{
	public function getHeaderSize();
	public function parsePackageSize($header);
}

class TcpClientShort
{
	private $host = "127.0.0.1";
	private $port = 9000;
	private $unix = false;
	private $_callTimeout = 0.1;	//second
	private $_connTimeout = 0.01;	//usecond
	private $_error_code = 0;
	private $_error;
	private $_splitter;

	function __construct($splitter) {
		$this->_splitter = $splitter;
	}

	public function getLastErrorCode() {
		return $this->_error_code;
	}

	public function getLastError() {
		return $this->_error;
	}

	function setServer($host, $port, $unix) {
		$this->host = $host;
		$this->port = $port;
		$this->unix = $unix;
	}

	function setTimeout($conn_timeout = 0.01, $call_timeout = 0.1) {
		if($call_timeout < $conn_timeout) {
			$call_timeout = $conn_timeout;
		}
		$this->_connTimeout = $conn_timeout;
		$this->_callTimeout = $call_timeout;   
	}

	function sendReceive($packet) {
		$domain = ($this->unix) ? (AF_UNIX) : (AF_INET);
		$protocol = ($this->unix) ? (0) : (SOL_TCP);
		$sock = new UCLibTcpSocket($domain, $protocol);

		$timeout = new UCLibTimeout($this->_callTimeout);
		$conn_ret = $sock->connect($this->host, $this->port, $this->_connTimeout);
		if( false === $conn_ret){
			$this->_error_code = $sock->getLastError();
			$this->_error = 'connect error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		$len = $sock->write($packet, strlen($packet), $timeout);
		if(false === $len ) {
			$this->_error_code = $sock->getLastError();
			$this->_error = 'write error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		$header_len = $this->_splitter->getHeaderSize();
		$header_pack = $sock->read($header_len, $timeout);
		if($header_pack === false){
			$this->_error_code = $sock->getLastError();
			$this->_error = 'read header error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		if ($sock->getLastError() != 0) {
			$this->_error_code = $sock->getLastError();
			$this->_error = 'read header error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		$len = $this->_splitter->parsePackageSize($header_pack);
		if ($len === false) {
			$this->_error_code = $sock->getLastError();
			$this->_error = 'parse head error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		$body = $this->sock->read($len - $header_len, $timeout);
		if($body === false){
			$this->_error_code = $sock->getLastError();
			$this->_error = 'read content error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		if ($sock->getLastError() != 0) {
			$this->_error_code = $sock->getLastError();
			$this->_error = 'read content error#'.$sock->getLastErrorMsg();
			$sock->close();
			$sock = null;
			return false;
		}

		$this->_error_code = 0;
		$this->_error = "";
		return $header_pack.$body;
	}
}

class TcpClientLong
{
	private $sock = null;
	private $host = "127.0.0.1";
	private $port = 9000;
	private $unix = false;
	private $_callTimeout = 0.1;	//second
	private $_connTimeout = 0.01;	//usecond
	private $_error_code = 0;
    private $_error;
    private $_splitter;

	function __construct($splitter) {
		$this->_splitter = $splitter;
	}

	public function getLastErrorCode() {
		return $this->_error_code;
	}

	public function getLastError() {
		return $this->_error;
	}

	function setServer($host, $port, $unix = false) {
		$this->host = $host;
		$this->port = $port;
		$this->unix = $unix;
		$this->maintainConnection();
	}

	function setTimeout($conn_timeout = 0.01, $call_timeout = 0.1) {
		if($call_timeout < $conn_timeout) {
			$call_timeout = $conn_timeout;
		}
		$this->_connTimeout = $conn_timeout;
		$this->_callTimeout = $call_timeout;
	}

	private function maintainConnection() {
		if ($this->sock != null) {
			$this->_error_code = 0;
			return true;
		}

		$domain = ($this->unix) ? (AF_UNIX) : (AF_INET);
		$protocol = ($this->unix) ? (0) : (SOL_TCP);
		$this->sock = new UCLibTcpSocket($domain, $protocol);
		$this->sock->clearError();
		$con = $this->sock->connect($this->host, $this->port, $this->_connTimeout);
		if ($con === false) {
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'connect timeout#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		$this->_error_code = 0;
		return true;
	}

	function sendReceive($packet) {
		if (false === $this->maintainConnection())
			return false;

		$timeout = new UCLibTimeout($this->_callTimeout);
		$len = $this->sock->write($packet, strlen($packet), $timeout);
		if(false === $len ) {
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'write error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		$header_len = $this->_splitter->getHeaderSize();
		$header_pack = $this->sock->read($header_len, $timeout);
		if($header_pack === false){
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'read header error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		if ($this->sock->getLastError() != 0) {
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'read header error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		$len = $this->_splitter->parsePackageSize($header_pack);
		if ($len === false) {
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'parse head error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		$body = $this->sock->read($len - $header_len, $timeout);
		if($body === false){
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'read content error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		if ($this->sock->getLastError() != 0) {
			$this->_error_code = $this->sock->getLastError();
			$this->_error = 'read content error#'.$this->sock->getLastErrorMsg();
			$this->sock->close();
			$this->sock = null;
			return false;
		}

		$this->_error_code = 0;
		$this->_error = "";
		return $header_pack.$body;
	}

}

?>
