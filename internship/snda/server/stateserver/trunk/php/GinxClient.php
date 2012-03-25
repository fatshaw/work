<?php

require_once('./TcpClient.php');
require_once('./protocolbuf/message/pb_message.php');
require_once('./proto/PMSGForPHP.php');
require_once('./GinxError.php');
require_once('./GinxResult.php');
require_once('./GinxPrepare.php');

class GinxClient implements PackageSplitter
{
	private $tcp = null;
	private $sequence_id = 0;
	private $error = 0;
	private $error_info = "";
	private $prepare = null;

	public function __construct($host = null, $port = null, $unix = null) {
		$this->setServer($host, $port, $unix);
		$this->prepare = new GinxPrepare();
	}

	public function setServer($host, $port, $unix) {
		if (is_null($unix)) {
			$unix = true;
		}
		if (is_null($host) && $unix) {
			$host = "/tmp/webagent";
		}
		$this->tcp = new TcpClientLong($this);
		$this->tcp->setServer($host, $port, $unix);
	}

	public function getLastError() {
		return $this->error;
	}

	public function getLastErrorInfo() {
		return $this->error_info;
	}

	protected function setErrorHelper($code, $additional_error = "") {
		$this->error = 0;
		$error_info = GinxError::translateError($code);
		if (strlen($additional_error) > 0)
			$error_info = $error_info."#".$additional_error;
		$this->error_info = $error_info;
	}

	public static function makeRequest($sql, $sequence, $timeout) {
		$sql_req_base = new PMSGCoro();
		$sql_req_base->set_iSequenceID($sequence);
		$sql_req = new QueryRequest2();
		$sql_req->set_base($sql_req_base);
		$sql_req->set_sql($sql);
		$sql_req->set_timeout($timeout);
		
		return $sql_req;
	}
	
	public static function makePrepareRequest($pre, $sequence, $timeout) {
		$sql_req_base = new PMSGCoro();
		$sql_req_base->set_iSequenceID($sequence);
		$sql_req = new QueryRequest2();
		$sql_req->set_base($sql_req_base);
		$sql_req->set_sql($pre->getPrepare());
		$sql_req->set_timeout($timeout);
		
		$params = $pre->getDBVariantParams();
		foreach ($params as $i => $param) {
			$sql_req->set_params($i, $param);
		}
		
		return $sql_req;
	}

	public static function packMessage($msg) {
		$msg_class = get_class($msg);
		$msg_class_len = strlen($msg_class);
		$msg_pack = $msg->SerializeToString();
		$msg_pack_len = strlen($msg_pack);
		$total_len = 4 + (4 + $msg_class_len) + (4 + $msg_pack_len);
		$header = pack("L2", NetUtil::htonl($total_len), NetUtil::htonl($msg_class_len));
		$pack_header = pack("L", NetUtil::htonl($msg_pack_len));
		return $header.$msg_class.$pack_header.$msg_pack;
	}

	public static function unpackMessage($pack) {
		$array = unpack("L2", $pack);
		$pack_len = NetUtil::ntohl($array[1]);
		$class_len = NetUtil::ntohl($array[2]);
		if ($pack_len != strlen($pack)) {
			return false;
		}

		$class_start = 4 + 4;
		$class = substr($pack, $class_start, $class_len);
		$array_class_pack_len = unpack("L", substr($pack, $class_start + $class_len, 4));
		$class_pack_len = NetUtil::ntohl($array_class_pack_len[1]);
		$class_pack_start = $class_start + $class_len + 4;
		$class_pack = substr($pack, $class_pack_start, $class_pack_len);
		if (strlen($class_pack) < $class_pack_len) {
			return false;
		}

		if (! class_exists($class, true)) {
			return false;
		}

		try {
			$result = new $class();
			if (! method_exists($result, "ParseFromString"))
				return false;
			$result->ParseFromString($class_pack);
		} catch (Exception $e) {
			print $e->getMessage();
			return false;
		}

		return $result;
	}

	public function query($sql, $timeout = 1000) {

		$sql_req = self::makeRequest($sql, $this->sequence_id, $timeout);
		$this->sequence_id ++;
		if ($sql_req === false) {
			$this->setErrorHelper(GinxError::ERR_MAKE_REQ);
			return false;
		}

		$send_pack = self::packMessage($sql_req);
		if ($send_pack === false) {
			$this->setErrorHelper(GinxError::ERR_PACK_REQ);
			return false;
		}

		$this->tcp->setTimeout($timeout / 1000, $timeout / 1000);
		$receive_pack = $this->tcp->sendReceive($send_pack);
		if ($receive_pack === false) {
			$this->setErrorHelper(GinxError::ERR_NET, $this->tcp->getLastError());
			return false;
		}

		$result = self::unpackMessage($receive_pack);
		if ($result === false) {
			$this->setErrorHelper(GinxError::ERR_UNPACK_RES);
			return false;
		}

		$ginx_result = new GinxResult;
		$ginx_parse = $ginx_result->parse($result);
		if ($ginx_parse === false) {
			$this->setErrorHelper(GinxError::ERR_PARSE_RES);
			return false;
		}

		$this->setErrorHelper(GinxError::OK);
		return $ginx_result;
	}

	public function prepare($prepare) {
		return $this->prepare->prepare($prepare);
	}

	public function bindParam($format) {
		$args = func_get_args();
		$flat_args = array();
		foreach ($args as $i => $arg) {
			if (is_array($arg)) {
				foreach ($arg as $j => $arg_unit) {
					array_push($flat_args, $arg_unit);
				}
			} else {
				array_push($flat_args, $arg);
			}
		}
		return $this->prepare->bindParam($format, $flat_args);
	}

	public function execute($timeout = 1000) {

		$sql_req = self::makePrepareRequest($this->prepare, $this->sequence_id, $timeout);
		$this->sequence_id ++;
		if ($sql_req === false) {
			$this->setErrorHelper(GinxError::ERR_MAKE_REQ);
			return false;
		}

		$send_pack = self::packMessage($sql_req);
		if ($send_pack === false) {
			$this->setErrorHelper(GinxError::ERR_PACK_REQ);
			return false;
		}

		$this->tcp->setTimeout($timeout / 1000, $timeout / 1000);
		$receive_pack = $this->tcp->sendReceive($send_pack);
		if ($receive_pack === false) {
			$this->setErrorHelper(GinxError::ERR_NET, $this->tcp->getLastError());
			return false;
		}

		$result = self::unpackMessage($receive_pack);
		if ($result === false) {
			$this->setErrorHelper(GinxError::ERR_UNPACK_RES);
			return false;
		}

		$ginx_result = new GinxResult;
		$ginx_parse = $ginx_result->parse($result);
		if ($ginx_parse === false) {
			$this->setErrorHelper(GinxError::ERR_PARSE_RES);
			return false;
		}

		$this->setErrorHelper(GinxError::OK);
		return $ginx_result;
	}

	public function getHeaderSize() {
		return 4;
	}

	public function parsePackageSize($header) {
		if (strlen($header) != 4)
			return false;
		$unpack_header = unpack("L", $header);
		$len = NetUtil::ntohl($unpack_header[1]);
		return $len;
	}

}

?>