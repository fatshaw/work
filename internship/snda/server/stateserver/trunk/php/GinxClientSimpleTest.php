<?php

require_once('./GinxClient.php');

//test client
$ginx = new GinxClient();

$result = $ginx->query("select * from test.users where sid=2", 1000);
if ($result) {
	print $result->getCode()."\n";
	print $result->getMsg()."\n";
	print "rows:\n";
	foreach ($result as $index => $row) {
		print $row[0]." ".$row[1]."\n";
	}
}

$result = $ginx->query("insert into test.users(sid, name) values(3, 'yinbo')");
if ($result) {
	print $result->getCode()."\n";
	print $result->getMsg()."\n";
}

$result = $ginx->query("delete from test.users where sid=1223123");
if ($result) {
	print $result->getCode()."\n";
	print $result->getMsg()."\n";
}

$result = $ginx->query("update test.users set name='yinbo2' where sid=3");
if ($result) {
	print $result->getCode()."\n";
	print $result->getMsg()."\n";
}

?>