<?php
require_once( "pb_proto_.php" );
class DBVariant_datatypes extends PBEnum
{
  const BOOL  = 1;
  const INT  = 2;
  const STRING  = 3;
  const DOUBLE  = 4;
  const NIL  = 5;

  public function __construct($reader=null)
  {
   	parent::__construct($reader);
 	$this->names = array(
			1 => "BOOL",
			2 => "INT",
			3 => "STRING",
			4 => "DOUBLE",
			5 => "NIL");
   }
}
class DBVariant extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "DBVariant_datatypes";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "type";
    $this->fields["2"] = "PBBool";
    $this->values["2"] = "";
    $this->fieldNames["2"] = "bool_value";
    $this->fields["3"] = "PBSignedInt";
    $this->values["3"] = "";
    $this->fieldNames["3"] = "int_value";
    $this->fields["4"] = "PBBytes";
    $this->values["4"] = "";
    $this->fieldNames["4"] = "string_value";
    $this->fields["5"] = "PB64Bit";
    $this->values["5"] = "";
    $this->fieldNames["5"] = "double_value";
  }
  function type()
  {
    return $this->_get_value("1");
  }
  function set_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function type_string()
  {
    return $this->values["1"]->get_description();
  }
  function bool_value()
  {
    return $this->_get_value("2");
  }
  function set_bool_value($value)
  {
    return $this->_set_value("2", $value);
  }
  function int_value()
  {
    return $this->_get_value("3");
  }
  function set_int_value($value)
  {
    return $this->_set_value("3", $value);
  }
  function string_value()
  {
    return $this->_get_value("4");
  }
  function set_string_value($value)
  {
    return $this->_set_value("4", $value);
  }
  function double_value()
  {
    return $this->_get_value("5");
  }
  function set_double_value($value)
  {
    return $this->_set_value("5", $value);
  }
}
class DBRow extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "DBVariant";
    $this->values["1"] = array();
    $this->fieldNames["1"] = "units";
  }
  function units($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_units()
  {
    return $this->_add_arr_value("1");
  }
  function set_units($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_unitss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_units()
  {
    $this->_remove_last_arr_value("1");
  }
  function unitss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_unitss()
  {
    return $this->_get_value("1");
  }
}
class QueryResult2 extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PMSGCoro";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "base";
    $this->fields["2"] = "PBSignedInt";
    $this->values["2"] = "";
    $this->fieldNames["2"] = "retnum";
    $this->fields["3"] = "PBBytes";
    $this->values["3"] = "";
    $this->fieldNames["3"] = "retmsg";
    $this->fields["4"] = "PBInt";
    $this->values["4"] = "";
    $this->fieldNames["4"] = "insertid";
    $this->fields["5"] = "PBInt";
    $this->values["5"] = "";
    $this->fieldNames["5"] = "affectedrows";
    $this->fields["6"] = "PBBytes";
    $this->values["6"] = array();
    $this->fieldNames["6"] = "fieldNames";
    $this->fields["7"] = "DBRow";
    $this->values["7"] = array();
    $this->fieldNames["7"] = "rows";
  }
  function base()
  {
    return $this->_get_value("1");
  }
  function set_base($value)
  {
    return $this->_set_value("1", $value);
  }
  function retnum()
  {
    return $this->_get_value("2");
  }
  function set_retnum($value)
  {
    return $this->_set_value("2", $value);
  }
  function retmsg()
  {
    return $this->_get_value("3");
  }
  function set_retmsg($value)
  {
    return $this->_set_value("3", $value);
  }
  function insertid()
  {
    return $this->_get_value("4");
  }
  function set_insertid($value)
  {
    return $this->_set_value("4", $value);
  }
  function affectedrows()
  {
    return $this->_get_value("5");
  }
  function set_affectedrows($value)
  {
    return $this->_set_value("5", $value);
  }
  function fieldNames($offset)
  {
    $v = $this->_get_arr_value("6", $offset);
    return $v->get_value();
  }
  function append_fieldNames($value)
  {
    $v = $this->_add_arr_value("6");
    $v->set_value($value);
  }
  function set_fieldNames($index, $value)
  {
    $v = new $this->fields["6"]();
    $v->set_value($value);
    $this->_set_arr_value("6", $index, $v);
  }
  function remove_last_fieldNames()
  {
    $this->_remove_last_arr_value("6");
  }
  function fieldNamess_size()
  {
    return $this->_get_arr_size("6");
  }
  function get_fieldNamess()
  {
    return $this->_get_value("6");
  }
  function rows($offset)
  {
    return $this->_get_arr_value("7", $offset);
  }
  function add_rows()
  {
    return $this->_add_arr_value("7");
  }
  function set_rows($index, $value)
  {
    $this->_set_arr_value("7", $index, $value);
  }
  function set_all_rowss($values)
  {
    return $this->_set_arr_values("7", $values);
  }
  function remove_last_rows()
  {
    $this->_remove_last_arr_value("7");
  }
  function rowss_size()
  {
    return $this->_get_arr_size("7");
  }
  function get_rowss()
  {
    return $this->_get_value("7");
  }
}
class QueryRequest2 extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PMSGCoro";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "base";
    $this->fields["2"] = "PBBytes";
    $this->values["2"] = "";
    $this->fieldNames["2"] = "sql";
    $this->fields["3"] = "DBVariant";
    $this->values["3"] = array();
    $this->fieldNames["3"] = "params";
    $this->fields["4"] = "PBInt";
    $this->values["4"] = "";
    $this->fieldNames["4"] = "timeout";
  }
  function base()
  {
    return $this->_get_value("1");
  }
  function set_base($value)
  {
    return $this->_set_value("1", $value);
  }
  function sql()
  {
    return $this->_get_value("2");
  }
  function set_sql($value)
  {
    return $this->_set_value("2", $value);
  }
  function params($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_params()
  {
    return $this->_add_arr_value("3");
  }
  function set_params($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function set_all_paramss($values)
  {
    return $this->_set_arr_values("3", $values);
  }
  function remove_last_params()
  {
    $this->_remove_last_arr_value("3");
  }
  function paramss_size()
  {
    return $this->_get_arr_size("3");
  }
  function get_paramss()
  {
    return $this->_get_value("3");
  }
  function timeout()
  {
    return $this->_get_value("4");
  }
  function set_timeout($value)
  {
    return $this->_set_value("4", $value);
  }
}
?>