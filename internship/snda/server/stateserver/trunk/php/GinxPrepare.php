<?php

require_once('./protocolbuf/message/pb_message.php');
require_once('./proto/PMSGForPHP.php');

class GinxPrepare {
	private $prepare = "";
	private $params_format = "";
	private $params_type = array();
	private $params = array();
	
	static public function coerce($type, $value) {
		if       ($type == "i") {
			return array(DBVariant_datatypes::INT, intval($value));
		} elseif ($type == "d") {
			return array(DBVariant_datatypes::DOUBLE, doubleval($value));
		} elseif ($type == "s") {
			return array(DBVariant_datatypes::STRING, strval($value));
		} elseif ($type == "b") {
			return array(DBVariant_datatypes::STRING, strval($value));
		} 
		
		return array(DBVariant_datatypes::NIL, "");
	}
	
	static public function toDBVariant($type, $value) {
		$dbV = new DBVariant();
		$dbV->set_type($type);
		if       ($type == DBVariant_datatypes::INT) {
			$dbV->set_int_value($value);
		} elseif ($type == DBVariant_datatypes::DOUBLE) {
			$dbV->set_double_value($value);
		} elseif ($type == DBVariant_datatypes::STRING) {
			$dbV->set_string_value($value);
		} 
		
		return $dbV;
	}
	
	public function prepare($prepare) {
		$this->prepare = "";
		$this->params_type = array();
		$this->params = array();
		if (! is_string($prepare) || strlen($prepare) == 0)
			return false;
		
		$this->prepare = $prepare;
		return true;
	}
	
	public function bindParam($format, $args){
		$this->params_type = array();
		$this->params = array();
		if (! is_string($format))
			return false;
		
	    if (! is_array($args))
	    	return false;
		
		$format_num = strlen($format);
		$arg_num = count($args);
		$min_num = min($format_num, ($arg_num - 1));
		for ($i=0; $i<$min_num; $i++) {
			$coerce = self::coerce(substr($format, $i, 1), $args[$i + 1]);
			array_push($this->params_type, $coerce[0]);
			array_push($this->params, $coerce[1]);
		}
		
		return true;
	}
	
	public function getPrepare() {
		return $this->prepare;
	}
	
	public function getParams() {
		return $this->params;
	}
	
	public function getDBVariantParams() {
		$result = array();
		foreach ($this->params as $i => $param) {
			array_push($result, self::toDBVariant($this->params_type[$i], $this->params[$i]));
		}
		return $result;
	}
}

?>