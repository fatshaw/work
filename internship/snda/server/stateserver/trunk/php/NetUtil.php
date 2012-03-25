<?php

class NetUtil
{
	private static $big_endian = null;

	public static function init()
	{
		$p = pack("L", 0xFF);
		$c = unpack("C", $p);
		self::$big_endian = ($c[1] == 0xFF) ? (true) : (false);
	}

	protected static function revertl($l)
	{
		$p = pack("L", $l);
		$p = strrev($p);
		$a = unpack("L", $p);
		$n = $a[1];
		return $n;
	}

	public static function htonl($l)
	{
		if (self::$big_endian == null) {
			self::init();
		}
		
		if (self::$big_endian) {
			return self::revertl($l);
		} else {
			return $l;
		}
	}

	public static function ntohl($l)
	{
		return self::htonl($l);
	}
}

NetUtil::init();

?>