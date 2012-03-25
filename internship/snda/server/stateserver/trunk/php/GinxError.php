<?php

class GinxError
{
	const OK = 0;
	const ERR_MAKE_REQ = 1;
	const ERR_PACK_REQ = 2;
	const ERR_NET = 3;
	const ERR_UNPACK_RES = 4;
	const ERR_PARSE_RES = 5;
	const ERR_NO_PREPARE = 6;

	private static $error_map = array(
		self::OK => "Ok",
		self::ERR_MAKE_REQ => "Make Request Error",
		self::ERR_PACK_REQ => "Pack Request Error",
		self::ERR_NET => "Net Error",
		self::ERR_UNPACK_RES => "Unpack Response Error",
		self::ERR_PARSE_RES => "Parse Resonse Error",
		self::ERR_NO_PREPARE => "No Prepare before",
	);

    public static function translateError($code) {
    	$error = self::$error_map[$code];
    	if ($error != null)
    		return $error;
    	return "Unknown";
    }

}

?>