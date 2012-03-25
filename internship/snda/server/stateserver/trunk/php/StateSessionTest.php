<?php
require_once(dirname(__FILE__)."/StateSession.php");
GlobalSession::start();
$_SESSION["HELLO"] = "123123";
var_dump($_SESSION["HELLO"]);
?>
