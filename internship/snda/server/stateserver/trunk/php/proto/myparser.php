<?php

require_once('../protocolbuf/parser/pb_parser.php');

$proto_list = array(
	'./pmsgforphp.proto' => './PMSGForPHP.php',
);

foreach ($proto_list as $proto => $php) {
	$parser = new PBParser();
	$parser->parse($proto, $php);
}

echo 'parse successfully';

?>
