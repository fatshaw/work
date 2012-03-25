<?php

require_once('./proto/PMSGForPHP.php');

class GinxResultRowIterator implements Iterator {

	private $obj = null;
	private $index = 0;

	function __construct($obj) {
		$this->obj = $obj;
		$this->index = 0;
	}

	function rewind() {
		$this->index = 0;
	}

	function valid() {
		if ($this->obj == null) {
			return false;
		}

		return $this->index >= 0 && $this->index < $this->obj->getRowsCount();
	}

	function key() {
		return $this->index;
	}

	function current() {
		if ($this->obj == null) {
			return null;
		}

		return $this->obj->getRow($this->index);
	}

	function next() {
		return $this->index ++;
	}
}

class GinxResult implements IteratorAggregate
{
	protected $code = null;
	protected $msg = null;
	protected $insert_id = null;
	protected $affected_rows = null;
	protected $field_names = array();
	protected $rows = array();

	public function __construct() {
		
	}

	function getIterator() {
		return new GinxResultRowIterator($this);
	}

	public function getCode() {
		return $this->code;
	}

	public function getMsg() {
		return $this->msg;
	}

	public function getInsertId() {
		return $this->insert_id;
	}

	public function getAffectedRows() {
		return $this->affected_rows;
	}

	public function getFieldNamesCount() {
		return count($this->field_names);
	}

	public function getFieldNames() {
		return $this->field_names;
	}

	public function getFieldName($index) {
		if ($this->field_names == null) {
			return null;
		}

		return $this->field_names[$index];
	}

	public function getRowsCount() {
		return count($this->rows);
	}

	public function getRows() {
		return $this->rows;
	}

	public function getRow($index) {
		if ($this->rows == null) {
			return null;
		}

		return $this->rows[$index];
	}

	public function getFieldRow($index) {
		$result = array();
		if ($this->rows == null) {
			return $result;
		}
		
		$row = $this->rows[$index];
		foreach ($this->$field_names as $i => $name) {
			$result[$name] = $row[$i];
		}
		
		return $result;
	}

	public function parse($qr) {

		try {
			$this->code = $qr->retnum();
			$this->msg = $qr->retmsg();
			$this->insert_id = $qr->insertid();
			$this->affected_rows = $qr->affectedrows();
			$parse_result = self::parseQueryResult($qr);
			if ($parse_result !== false) {
				$this->field_names = $parse_result[0];
				$this->rows = $parse_result[1];
			} else {
				$this->field_names = array();
				$this->rows = array();
			}
		} catch (Exception $e) {
			print $e->getMessage();
			return false;
		}

		return true;
	}

	public static function getDBVariant($dBVariant){
		if($dBVariant->type() == DBVariant_datatypes::BOOL) {
			return $dBVariant->bool_value();
		} else if ($dBVariant->type() == DBVariant_datatypes::INT) {
			return $dBVariant->int_value();
		} else if ($dBVariant->type() == DBVariant_datatypes::STRING) {
			return $dBVariant->string_value();
		} else if ($dBVariant->type() == DBVariant_datatypes::DOUBLE) {
			return $dBVariant->double_value();
		} else if ($dBVariant->type() == DBVariant_datatypes::NIL) {
			return null;
		} else {
			return null;
		}
	}

	public static function parseQueryResult($qr) {

		$fieldArray = array();
		$fields = $qr->get_fieldNamess();
		if (is_array($fields)) {
			foreach ($fields as $i => $name) {
				array_push($fieldArray, $name->get_value());
			}
		}

		$rowArray = array();
		$rows = $qr->get_rowss();
		if (is_array($rows)) {
			foreach ($rows as $i => $row) {
				$a_row = array();
				$units = $row->get_unitss();
				if (is_array($units)) {
					foreach($units as $j => $unit) {
						array_push($a_row, self::getDBVariant($unit));
					} 
				} else {
					array_push($a_row, $unit->get_value());
				}
				array_push($rowArray, $a_row);
			}
		}
		return array($fieldArray, $rowArray);
	}

}

?>