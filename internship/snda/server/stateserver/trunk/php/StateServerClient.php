<?php
require_once(dirname(__FILE__).'/TcpClient.php');
require_once(dirname(__FILE__).'/protocolbuf/message/pb_message.php');
require_once(dirname(__FILE__).'/proto/tokenMessage.php');
class StateServerClient implements PackageSplitter
{
	private $tcp;
	private $host = "127.0.0.1";
	private $port = 8110;
	private $_callTimeout = 0.1;	//second
	private $_connTimeout = 0.01;	//usecond
	function __construct()
	{
		$this->tcp = new TcpClientLong($this);
	}

	function setServer($host,$port)
	{
		$this->tcp->setServer($host,$port,false);
		$this->tcp->setTimeout($this->_connTimeout,$this->_callTimeout);
	}

	function setTimeout($conn_timeout = 0.01, $call_timeout = 0.1) {
		if($call_timeout < $conn_timeout) {
			$call_timeout = $conn_timeout;
		}
		$this->_connTimeout = $conn_timeout;
		$this->_callTimeout = $call_timeout;   
	}

	function request($apiname,$params)
	{
			$totalLength = 0;
			$strlen = 0;
			$pbufLength = 0;
			$typename = "StateServer::StateServerRequest";
			$strlen = strlen($typename);
			$base = new TokenMessage();
			$base->set_seq(1);
			$base->set_apiname($apiname);
			$base->set_cmd(1);
			foreach($params as $key=> $value)
			{
				$params =$base->add_params();
				$params->set_key($key);	
				$params->set_value($value);
			}
			
			$pbuf = $base->SerializeToString();
			$pbufLength = strlen($pbuf);
			$totalLength = 12 + $strlen + $pbufLength;
			$sendbuf = pack("N2",$totalLength,$strlen).$typename.pack("N",$pbufLength).$pbuf;
			$response = $this->tcp->sendReceive($sendbuf);
			if($response === false)
			{
				var_dump("false");
				return;
			}
			$totallength = unpack("N",$response);
			$response = substr($response,4);
			$strlen = unpack("N",$response);
			$response = substr($response,4+$strlen[1]);
			$response = substr($response,4);
			$base = new TokenMessage();
			$base->parseFromString($response);
			return $base->params(0)->value();
	}
	
	public function getHeaderSize(){return 4;}
	public function parsePackageSize($header){
		if (strlen($header) != 4)
			return false;
		$unpack_header = unpack("L", $header);
		$len = NetUtil::ntohl($unpack_header[1]);
		return $len;
	}
}
?>