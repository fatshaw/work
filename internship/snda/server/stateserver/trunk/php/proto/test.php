<?php
// first include pb_message
require_once('../protocolbuf/message/pb_message.php');

// now read it with the old file
// include the generated file
// require_once('./PMSGCoro.php');
require_once('./PMSGSqlSigRqt.php');
require_once('./PMSGSqlSigRslt.php');

$base = new PMSGCoro();
$base->set_iSequenceID(111);
$base->set_callgraph('222');

$query = new PMSGSqlSigRqt();
$query->set_base($base);
$query->set_m_strSql("select * from tab");
$string = $query->SerializeToString();

$q = new PMSGSqlSigRqt();
$q->parseFromString($string);
var_dump($q->base()->iSequenceID());
echo "<br>";

var_dump($q->base()->callgraph());
echo "<br>";

var_dump($q->m_strSql());
echo "<br>";

$base->set_iSequenceID(333);
$base->set_callgraph('444');

$result = new PMSGSqlSigRslt();
$result->set_base($base);
$result->set_m_iStatus(555);
$result->set_m_strData("result0:0, result1:1, result2:2");
$string = $result->SerializeToString();

$r = new PMSGSqlSigRslt();
$r->parseFromString($string);
var_dump($r->base()->iSequenceID());
echo "<br>";

var_dump($r->base()->callgraph());
echo "<br>";

var_dump($r->m_iStatus());
echo "<br>";

var_dump($r->m_strData());
echo "<br>";
?>
