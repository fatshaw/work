<?php
require_once(dirname(__FILE__)."/StateServerClient.php");

class GlobalSession
{
		private static $CLIENT = "";
		private static $SERVER_ADDRESS = "10.241.12.117";
		private static $PORT= 8110;
		private static $MAX_LIFETIME;
		
		static function start()
		{
			self::$MAX_LIFETIME  = get_cfg_var("session.gc_maxlifetime");
			session_set_save_handler("GlobalSession::open", "GlobalSession::close", "GlobalSession::read", "GlobalSession::write", "GlobalSession::destroy", "GlobalSession::gc");
			register_shutdown_function("session_write_close");
			session_start();
		}
		
		static function setServer($host,$port)
		{
			self::$SERVER_ADDRESS = $host;
			self::$PORT = $port;
		}
		
		static function setMaxLifeTime($lifetime)
		{
			self::$MAX_LIFETIME = $lifetime;
		}
		
		static function request($name,$param)
		{
				$response = self::$CLIENT->request($name,$param);
				return $response;
		}
		
		static function open($save_path, $session_name)
		{
				self::$CLIENT = new StateServerClient();
				self::$CLIENT->setServer(self::$SERVER_ADDRESS,self::$PORT);
				return(true);
		}
			
		static function close()
		{
			return(true);
		}
			
		static function read($id)
		{
			$sess_data = self::request("GetSession",array("session_id"=>$id));
			return $sess_data;
		}
			
		static function write($id, $sess_data)
		{
				if(empty($sess_data))return true;
				$sess_data = self::request("SetSession",array("session_id"=>$id,"session_data"=>$sess_data,"expired_time"=>self::$MAX_LIFETIME));
		    return true;
		}
		
		static function destroy($id)
		{
		  return true;
		}
			
		static function gc($maxlifetime)
		{
		  return true;
		}
}

?>