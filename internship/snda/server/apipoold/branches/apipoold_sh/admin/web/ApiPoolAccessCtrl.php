<?php

class ApiPoolAccessCtrl
{
	public static $tb_req_src_ip_ = "request_src_ip_ctrl";
	public static $tb_req_src_service_rate_ = "request_src_service_rate_ctrl";
	public static $tb_req_src_service_api_ = "request_src_service_api_ctrl";
	public static $tb_params_ = "ctrl_params";
	public static $tb_version_ = "version_ctrl";
	public static $tb_loginfo_ = "loginfo";
	
	public static $tbs_struct_ = array(
		"request_src_ip_ctrl"=>"request_src, ip",
		"request_src_service_rate_ctrl"=>"request_src, service, amount, freq",
		"request_src_service_api_ctrl"=>"request_src, service, api",
		"ctrl_params"=>"name, value",
		"version_ctrl"=>"host, version",
		"loginfo"=>"info, rt"
	);

	private $server_ = "10.241.12.117";
	private $port_ = "3306";
	private $db_name_ = "apipool_access_ctrl";
	private $user_ = "root";
	private $pwd_ = "123qwe";
	
	private $db_ = null;
	
	private $errno_ = 0;
	private $error_ = "";
	
	public static function checkName($name) {
		$name_trimmed = trim($name);
		$reg = "^[a-zA-Z][a-zA-Z0-9_]{1,63}$";
		return (false !== ereg($reg, $name_trimmed));
	}
	
	public static function checkIp($ip) {
		$ip_trimmed = trim($ip);
		$reg = "^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$";
		//$reg = "^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$";
		$ret = ereg($reg, $ip_trimmed);
		return (false !== ereg($reg, $ip_trimmed));
	}
	
	public static function checkApi($api) {
		$api_trimmed = trim($api);
		$reg = "^\\*$";
		return self::checkName($api_trimmed) || (false !== ereg($reg, $api_trimmed));
	}
	
	public static function checkDigit($digit) {
		$digit_trimmed = trim($digit);
		$reg = "^[1-9]\\d*|0$";
		return (false !== ereg($reg, $digit_trimmed));
	}
	
	public static function mysqlEscape($string) {
		$result = $string;
		
		$result = str_replace('\\', '\\\\', $result);
		$result = str_replace('\'', '\\\'', $result);
		$result = str_replace('\"', '\\\"', $result);
		
		return $result;
	}
	
	public function __construct($server = null, $port = null, $db = null, $user = null, $pwd = null) {
		$this->db_name_ = $db;
		$this->connect($server, $port, $user, $pwd);
	}
	
	public function __destruct() {
		$this->disconnect();
	}
	
	public function connect($server, $port, $user, $pwd) {
		$this->server_ = $server;
		$this->port_ = $port;
		$this->user_ = $user;
		$this->pwd_ = $pwd;
		
		$this->disconnect();
		
		return $this->__connect();
	}
	
	private function __connect() {
		if ($this->server_ == null)
			return false;
		
		$this->db_ = mysql_connect($this->server_.":".$this->port_, $this->user_, $this->pwd_);
		if ($this->db_ == null)
			return false;
		
		return true;
	}
	
	public function reconnect() {
		if ($this->db_)
			return true;

		return $this->__connect();
	}
	
	public function disconnect() {
		if ($this->db_ != null) {
			mysql_close($this->db_);
			$this->db_ = null;
		}
	}
	
	public function errno() {
		return $this->errno_;
	}
	
	public function error() {
		return $this->error_;
	}
	
	private function updateError() {
		$this->error_ = mysql_error($this->db_);
		$this->errno_ = mysql_errno($this->db_);
	}
	
	public function getTableList($table) {
		$sql = "select * from ".$table;
		$result = mysql_db_query($this->db_name_, $sql, $this->db_);
		$fields = array();
		for ($i=0; $i<mysql_num_fields($result); $i++)
		{
			array_push($fields, mysql_field_name($result, $i));
		}
		
		$array_result = array();
		array_push($array_result, $fields);
		while (($row = mysql_fetch_row($result))) {
			array_push($array_result, $row);
		}
		
		$this->updateError();
		return $array_result;
	}
	
	public function getRawTableList($table) {
		$sql = "select * from ".$table;
		$result = mysql_db_query($this->db_name_, $sql, $this->db_);
		$array_result = array();
		while (($row = mysql_fetch_row($result))) {
			array_push($array_result, $row);
		}
		$this->updateError();
		return $array_result;
	}
	
	public function getTableRow($table, $condition) {
		$sql = "select * from ".$table." where ".$condition;
		$result = mysql_db_query($this->db_name_, $sql, $this->db_);
		$array_result = array();
		while (($row = mysql_fetch_row($result))) {
			array_push($array_result, $row);
		}
		$this->updateError();
		return $array_result;
	}
	
	public function generateRequestRateApiTable() {
		$request_rate_list = $this->getRawTableList(self::$tb_req_src_service_rate_);
		for ($i=0; $i<count($request_rate_list); $i++) {
			$req_row = $request_rate_list[$i];
			$condition = "request_src="."'".$req_row[0]."'"." and service="."'".$req_row[1]."'";
			$api_row = $this->getTableRow(self::$tb_req_src_service_api_, $condition);
			$apis = "";
			if (count($api_row) > 0) {
				$apis = $api_row[0][2];
			}
			for ($j=1; $j<count($api_row); $j++) {
				$apis = $apis.",".$api_row[$j][2];
			}
			array_push($request_rate_list[$i], $apis);
		}
		$this->updateError();
		return $request_rate_list;
	}
	
	public function addTableRow($table, $values) {
		$success = false;
		mysql_db_query($this->db_name_, "BEGIN", $this->db_);
		$sql = "replace into $table(".self::$tbs_struct_[$table].") values($values)";
		$query1 = mysql_db_query($this->db_name_, $sql, $this->db_);
		$affected = mysql_affected_rows($this->db_);
		$query2 = true;
		if ($affected > 0) {
			$new_version = $this->getDbVersion() + 1;
			$sql2 = "replace into ".self::$tb_version_."(".self::$tbs_struct_[self::$tb_version_].") values('local', $new_version)";
			$query2 = mysql_db_query($this->db_name_, $sql2, $this->db_);
		}
		if ($query1 && $query2) {
			mysql_db_query($this->db_name_, "COMMIT", $this->db_);
			$this->log($sql);
			$success = true;
		} else {
			mysql_db_query($this->db_name_, "ROLLBACK", $this->db_);
			$success = false;
		}
		mysql_db_query($this->db_name_, "END", $this->db_);
		$this->updateError();
		return $success;
	}
	
	public function removeTableRow($table, $condition) {
		$success = false;
		mysql_db_query($this->db_name_, "BEGIN", $this->db_);
		$sql = "delete from ".$table." where ".$condition;
		$query1 = mysql_db_query($this->db_name_, $sql, $this->db_);
		$affected = mysql_affected_rows($this->db_);
		$query2 = true;
		if ($affected > 0) {
			$new_version = $this->getDbVersion() + 1;
			$sql2 = "replace into ".self::$tb_version_."(".self::$tbs_struct_[self::$tb_version_].") values('local', $new_version)";
			$query2 = mysql_db_query($this->db_name_, $sql2, $this->db_);
		}
		if ($query1 && $query2) {
			mysql_db_query($this->db_name_, "COMMIT", $this->db_);
			$this->log($sql);
			$success = true;
		} else {
			mysql_db_query($this->db_name_, "ROLLBACK", $this->db_);
			$success = false;
		}
		mysql_db_query($this->db_name_, "END", $this->db_);
		$this->updateError();
		return $success;
	}
	
	public function getDbVersion() {
		$rows = $this->getTableRow(self::$tb_version_, "host='local'");
		if (!$rows)
			return 0;
		if (count($rows) < 1)
			return 0;
		$version_row = $rows[0];
		if (count($version_row) < 2)
			return 0;
		return $version_row[1];
	}
	
	public function updateDbVersion() {
		$row = $this->getTableRow(self::$tb_version_, "host='local'");
		$new_version = $row[0][1] + 1;
		$sql = "replace into ".self::$tb_version_."(".self::$tbs_struct_[self::$tb_version_].") values('local', $new_version)";
		mysql_db_query($this->db_name_, $sql, $this->db_);
		$this->updateError();
	}
	
	public function log($info) {
		$info_escape = self::mysqlEscape($info);
		$sql = "insert into ".self::$tb_loginfo_." value('".$info_escape."', null)";
		$result = mysql_db_query($this->db_name_, $sql, $this->db_);
		$this->updateError();
		return (false !== $result);
	}
}

?>