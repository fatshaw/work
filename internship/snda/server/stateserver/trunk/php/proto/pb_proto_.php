<?php
class PMSGCoro extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fieldNames["1"] = "iSequenceID";
  }
  function iSequenceID()
  {
    return $this->_get_value("1");
  }
  function set_iSequenceID($value)
  {
    return $this->_set_value("1", $value);
  }
}
?>