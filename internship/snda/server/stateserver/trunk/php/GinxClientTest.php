<?php

require_once('./GinxClient.php');

//test proto
$dBVariant1 = new DBVariant();
$dBVariant1->set_type(DBVariant_datatypes::DOUBLE);
$dBVariant1->set_double_value(0.1234);
$dBVariant2 = new DBVariant();
$dBVariant2->set_type(DBVariant_datatypes::BOOL);
$dBVariant2->set_bool_value(true);

$dBRow1 = new DBRow();
$dBRow1->set_units(0, $dBVariant1);
$dBRow1->set_units(1, $dBVariant2);

$qr = new QueryResult2();
$qr->set_fieldNames(0, 'double');
$qr->set_fieldNames(1, 'bool');
$qr->set_rows(0, $dBRow1);
//var_dump($qr);

//test GinxResult
$qr_parse = new GinxResult();
$qr_parse->parse($qr);
//var_dump($qr_parse);
if ($qr_parse) {
	foreach ($qr_parse as $index => $value) {
		//var_dump($value);
	}
}

//test pack unpack
$sql = "select name from Profile.info where name = 'yinbo'";
$send_msg = GinxClient::makeRequest($sql, 0, 1000);
$send_pack = GinxClient::packMessage($send_msg);
//var_dump($send_pack);
$recv_pack = $send_pack;
$recv_msg = GinxClient::unpackMessage($recv_pack);
//var_dump($recv_msg);
$recv_pack = GinxClient::packMessage($recv_msg);
//var_dump($recv_pack);

if ($send_pack != $recv_pack) {
	print "pack unpack not match!\n";
}

//test client
$ginx = new GinxClient("10.241.12.117", "14000", false);


//test query
print "******************Test Query******************\n";

$test_cases = array(
	array("select name from Profile.info where name='yinbo'", false),
	array("select * from test.users", true),
	array("insert into test.users(sid, name) values(3, 'yinbo')", true),
	array("select * from test.users", true),
	array("update test.users set name='yinbo2' where sid=3", true),
	array("select * from test.users", true),
	array("delete from test.users where sid=3", true),
	array("select * from test.users", true),
	array("select sid from test.users", true),
	array("select * from test.users where unknown=0", false),
	array("update test.users set unknown='yinbo2' where sid=3", false),
	array("delete from test.users where sid=1223123", true),
	array("insert into test.users(sid, name) values(2, 'yinbo')", false),
	array("select sid from test.unknown", false),
	array("select * from test.users where sid=10", true),
	array("select * from test.users where sid=11", true),
);

$failed_cases = array();

foreach ($test_cases as $index => $test_case) {
	$sql = $test_case[0];
	$ret = $test_case[1];
	print "sql:".$sql.": \n";
	$result = $ginx->query($sql);
	if ($result) {
		print "code:".$result->getCode()."\n";
		print "msg:".$result->getMsg()."\n";
		print "insertId:".$result->getInsertId()."\n";
		print "affectedRows:".$result->getAffectedRows()."\n";
		print "fields:\n";
		foreach ($result->getFieldNames() as $i => $field) {
			print $field." ";
		}
		print "\n";
		print "rows:\n";
		foreach ($result as $i => $row) {
			foreach ($row as $j => $unit) {
				print $unit." ";
			}
			print "\n";
		}
	} else {
		print "error:".$ginx->getLastErrorInfo()."\n";
	}
	$suc = $result && $result->getCode() == 0;
	$test_suc = ($suc == $ret);
	if (!$test_suc) {
		array_push($failed_cases, $test_case);
	}
	print "*************Test ".($test_suc ? "OK" : "Failed")."*************\n\n";
}

print "\n";
if (count($failed_cases) == 0) {
	print "******************Test Query All OK******************\n";
} else {
	print "******************Test Query Failed******************\n";
	print "Failed cases:\n";
	foreach ($failed_cases as $index => $testcase) {
		print $testcase[0]." => ".$testcase[1]."\n";
	}
}

//test prepare
print "******************Test Prepare******************\n";
$test_cases = array(
	array("select ? from test.users where sid=?", false, "si", array("sid", 2)),
);

$failed_cases = array();

foreach ($test_cases as $index => $test_case) {
	$sql = $test_case[0];
	$ret = $test_case[1];
	$format = $test_case[2];
	$params = $test_case[3];
	print "sql:".$sql.": \n";
	print "format:".$format.": \n";
	print "params:";
	foreach ($params as $i => $param) {print strval($param).",";};
	print "\n";
	$ginx->prepare($sql);
	$ginx->bindParam($format, $params);
	$result = $ginx->execute();
	if ($result) {
		print "code:".$result->getCode()."\n";
		print "msg:".$result->getMsg()."\n";
		print "insertId:".$result->getInsertId()."\n";
		print "affectedRows:".$result->getAffectedRows()."\n";
		print "fields:\n";
		foreach ($result->getFieldNames() as $i => $field) {
			print $field." ";
		}
		print "\n";
		print "rows:\n";
		foreach ($result as $i => $row) {
			foreach ($row as $j => $unit) {
				print $unit." ";
			}
			print "\n";
		}
	} else {
		print "error:".$ginx->getLastErrorInfo()."\n";
	}
	$suc = $result && $result->getCode() == 0;
	$test_suc = ($suc == $ret);
	if (!$test_suc) {
		array_push($failed_cases, $test_case);
	}
	print "*************Test ".($test_suc ? "OK" : "Failed")."*************\n\n";
}

print "\n";
if (count($failed_cases) == 0) {
	print "******************Test Prepare All OK******************\n";
} else {
	print "******************Test Prepare Failed******************\n";
	print "Failed cases:\n";
	foreach ($failed_cases as $index => $testcase) {
		print $testcase[0]." => ".$testcase[1]." => ".$testcase[2]."\n";
	}
}

?>