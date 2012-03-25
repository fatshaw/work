<?php
class TokenMessage_Paramlist extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBString";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "key";
    $this->fields["2"] = "PBBytes";
    $this->values["2"] = "";
    $this->fieldNames["2"] = "value";
  }
  function key()
  {
    return $this->_get_value("1");
  }
  function set_key($value)
  {
    return $this->_set_value("1", $value);
  }
  function value()
  {
    return $this->_get_value("2");
  }
  function set_value($value)
  {
    return $this->_set_value("2", $value);
  }
}
class TokenMessage extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "seq";
    $this->fields["2"] = "PBString";
    $this->values["2"] = "";
    $this->fieldNames["2"] = "apiname";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
    $this->fieldNames["3"] = "cmd";
    $this->fields["4"] = "TokenMessage_Paramlist";
    $this->values["4"] = array();
    $this->fieldNames["4"] = "params";
  }
  function seq()
  {
    return $this->_get_value("1");
  }
  function set_seq($value)
  {
    return $this->_set_value("1", $value);
  }
  function apiname()
  {
    return $this->_get_value("2");
  }
  function set_apiname($value)
  {
    return $this->_set_value("2", $value);
  }
  function cmd()
  {
    return $this->_get_value("3");
  }
  function set_cmd($value)
  {
    return $this->_set_value("3", $value);
  }
  function params($offset)
  {
    return $this->_get_arr_value("4", $offset);
  }
  function add_params()
  {
    return $this->_add_arr_value("4");
  }
  function set_params($index, $value)
  {
    $this->_set_arr_value("4", $index, $value);
  }
  function set_all_paramss($values)
  {
    return $this->_set_arr_values("4", $values);
  }
  function remove_last_params()
  {
    $this->_remove_last_arr_value("4");
  }
  function paramss_size()
  {
    return $this->_get_arr_size("4");
  }
  function get_paramss()
  {
    return $this->_get_value("4");
  }
}
?>