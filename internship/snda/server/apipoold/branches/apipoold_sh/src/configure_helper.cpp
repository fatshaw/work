#include <arpa/inet.h>


#include "configure_helper.h"
#include "options.h"
#include "log.h"
#include "phpservice_handler.h"
#include "protobuf_handler.h"

int LoadConfigure_AdminServer(utils::Configure& configure)
{
	string val_string;
	long val_long;
	int rc;

	// admin server
	rc = configure.get_single("admin_server", "admin_ip", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get admin server conf error");
		return -1;
	}
	Options::admin_server.admin_ip = val_string;

	rc = configure.get_single("admin_server", "admin_port", val_long);
	if (0 != rc) {
		LOG_FATAL("Configure error, get admin server conf error");
		return -1;
	}
	Options::admin_server.admin_port = val_long;

	rc = configure.get_single("admin_server", "reconnect_interval", val_long);
	if (0 != rc) {
		LOG_FATAL("Configure error, get admin server conf error");
		return -1;
	}
	Options::admin_server.reconnect_interval = val_long;

	LOG_INFO("Configure, admin_server, admin_ip : %s", Options::admin_server.admin_ip.c_str());
	LOG_INFO("Configure, admin_server, admin_port : %d", Options::admin_server.admin_port);
	LOG_INFO("Configure, admin_server, reconnect_interval : %d", Options::admin_server.reconnect_interval);

	return 0;
}


int LoadConfigure_Apipoold(utils::Configure& configure, bool is_reload)
{
	string val_string;
	long val_long;
	int rc;

	// apipoold
	if(!is_reload)
	{
		rc = configure.get_single("apipoold", "apipoold_longtcp_ip", val_string);
		if (0 != rc) {
			LOG_FATAL("Configure error, get apipoold conf apipoold_longtcp_ip error");
			return -1;
		}
		Options::apipoold.apipoold_longtcp_ip = val_string;

		rc = configure.get_single("apipoold", "apipoold_longtcp_port", val_long);
		if (0 != rc) {
			LOG_FATAL("Configure error, get apipoold conf apipoold_longtcp_port error");
			return -1;
		}
		Options::apipoold.apipoold_longtcp_port = val_long;

		rc = configure.get_single("apipoold", "apipoold_shorttcp_ip", val_string);
		if (0 != rc) {
			LOG_FATAL("Configure error, get apipoold conf apipoold_shorttcp_ip error");
			return -1;
		}
		Options::apipoold.apipoold_shorttcp_ip = val_string;

		rc = configure.get_single("apipoold", "apipoold_shorttcp_port", val_long);
		if (0 != rc) {
			LOG_FATAL("Configure error, get apipoold conf apipoold_shorttcp_port error");
			return -1;
		}
		Options::apipoold.apipoold_shorttcp_port = val_long;


		rc = configure.get_single("apipoold", "shm_key", val_long);
		if (0 != rc) {
			LOG_FATAL("Configure error, get apipoold conf shm_key error");
			return -1;
		}
		Options::apipoold.shm_key = val_long;	
	}
	else
	{
		LOG_WARN("Configure, apipoold, Can't change apipoold_ip|apipoold_port|apipooldcount|index|shm_key on runtime.");		
	}

	rc = configure.get_single("apipoold", "longtcp_timeout", val_long);
	if (0 != rc) {
		LOG_FATAL("Configure error, get apipoold conf longtcp_timeout error");
		return -1;
	}
	Options::apipoold.longtcp_timeout = val_long;

	rc = configure.get_single("apipoold", "shorttcp_timeout", val_long);
	if (0 != rc) {
		LOG_FATAL("Configure error, get apipoold conf shorttcp_timeout error");
		return -1;
	}
	Options::apipoold.shorttcp_timeout = val_long;

	rc = configure.get_single("apipoold", "rpc_root", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get apipoold conf rpcRoot error");
		return -1;
	}
	Options::apipoold.rpcRoot = val_string;
	

	LOG_INFO("Configure, apipoold, apipoold_longtcp_ip : %s", Options::apipoold.apipoold_longtcp_ip.c_str());
	LOG_INFO("Configure, apipoold, apipoold_longtcp_port : %d", Options::apipoold.apipoold_longtcp_port);
	LOG_INFO("Configure, apipoold, longtcp_timeout : %d", Options::apipoold.longtcp_timeout);
	LOG_INFO("Configure, apipoold, apipoold_shorttcp_ip : %s", Options::apipoold.apipoold_shorttcp_ip.c_str());
	LOG_INFO("Configure, apipoold, apipoold_shorttcp_port : %d", Options::apipoold.apipoold_shorttcp_port);
	LOG_INFO("Configure, apipoold, shorttcp_timeout : %d", Options::apipoold.shorttcp_timeout);
	LOG_INFO("Configure, apipoold, shm_key : %d", Options::apipoold.shm_key);
	LOG_INFO("Configure, apipoold, rpcRoot : %s", Options::apipoold.rpcRoot.c_str());

	return 0;
}


int LoadConfigure_phpService(utils::Configure& configure, bool is_reload)
{
	string val_string;
//		long val_long;
	int count;
	int rc;

	// php service
	rc = configure.count1_array2("phpservice", "service_list", count);
	if (0 != rc) {
		LOG_FATAL("Configure error, get phpservice service_list count conf error");
		return -1;
	}
	LOG_INFO("Configure, phpservice, service_list count:%d", count);
	
	Options::phpservice.service_list.clear();

	for (int i = 0; i < count; i++)
	{
		string serviceName;
		string ip;
		long port;
		long index;

		rc = configure.get_array2("phpservice", "service_list", i, 0, serviceName);
		if (0 != rc) {
			LOG_FATAL("Configure error, get php service conf serviceName error");
			return -1;
		}
		rc = configure.get_array2("phpservice", "service_list", i, 1, ip);
		if (0 != rc) {
			LOG_FATAL("Configure error, get php service conf ip error");
			return -1;	
		}
		rc = configure.get_array2("phpservice", "service_list", i, 2, port);
		if (0 != rc) {
			LOG_FATAL("Configure error, get php service conf port error");
			return -1;
		}
		rc = configure.get_array2("phpservice", "service_list", i, 3, index);
		if (0 != rc) {
			LOG_FATAL("Configure error, get php service conf index error");
			return -1;
		}

		ServcieInfo s_info;
		s_info.serviceName = serviceName;
		s_info.ip = ip;
		s_info.port = port;
		s_info.index = index;

		Options::phpservice.service_list.push_back(s_info);
		LOG_INFO("Configure, phpservice, service_list s_name: %s", s_info.serviceName.c_str());
		LOG_INFO("Configure, phpservice, service_list ip: %s", s_info.ip.c_str());
		LOG_INFO("Configure, phpservice, service_list port: %d", s_info.port);
		LOG_INFO("Configure, phpservice, service_list index: %d", s_info.index);
	}
	if(is_reload == true){
		FcgiAgentHandler::reconfigure();
		LOG_DEBUG("[Reconfigure FcgiAgentHandler configure]");
	}
	return 0;
}


int LoadConfigure_stateServer(utils::Configure& configure, bool is_reload)
{
	string val_string;
//		long val_long;
	int count;
	int rc;

	// state server 
	rc = configure.count1_array2("stateServer", "service_list", count);
	if (0 != rc) {
		LOG_FATAL("Configure error, get staterServer service_list count conf error");
		return -1;
	}
	LOG_INFO("Configure, staterServer, service_list count:%d", count);
	
	Options::stateServer.service_list.clear();

	for (int i = 0; i < count; i++)
	{
		string serviceName;
		string ip;
		long port;
		long index;

		rc = configure.get_array2("stateServer", "service_list", i, 0, serviceName);
		if (0 != rc) {
			LOG_FATAL("Configure error, get stateServer conf serviceName error");
			return -1;
		}
		rc = configure.get_array2("stateServer", "service_list", i, 1, ip);
		if (0 != rc) {
			LOG_FATAL("Configure error, get stateServer conf ip error");
			return -1;	
		}
		rc = configure.get_array2("stateServer", "service_list", i, 2, port);
		if (0 != rc) {
			LOG_FATAL("Configure error, get stateServer conf port error");
			return -1;
		}
		rc = configure.get_array2("stateServer", "service_list", i, 3, index);
		if (0 != rc) {
			LOG_FATAL("Configure error, get stateServer conf index error");
			return -1;
		}

		ServcieInfo s_info;
		s_info.serviceName = serviceName;
		s_info.ip = ip;
		s_info.port = port;
		s_info.index = index;

		Options::stateServer.service_list.push_back(s_info);
		LOG_INFO("Configure, stateServer, service_list s_name: %s", s_info.serviceName.c_str());
		LOG_INFO("Configure, stateServer, service_list ip: %s", s_info.ip.c_str());
		LOG_INFO("Configure, stateServer, service_list port: %d", s_info.port);
		LOG_INFO("Configure, stateServer, service_list index: %d", s_info.index);
	}
	if(is_reload == true){
		ProtobufServerHandler::reconfigure();
		LOG_DEBUG("[Reconfigure ProtobufServerHandler configure]");
	}
	return 0;
}


int LoadConfigure_System(utils::Configure& configure)
{
	string val_string;
	int rc;

	// system
	rc = configure.get_single("system", "log_level", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get system conf error");
		return -1;
	}
	Options::system.log_level = val_string;
	
	if (Options::system.log_level == "fatal") LOG_SET_LEVEL(utils::L_FATAL);
	else if (Options::system.log_level == "error") LOG_SET_LEVEL(utils::L_ERROR);
	else if (Options::system.log_level == "warn") LOG_SET_LEVEL(utils::L_WARN);
	else if (Options::system.log_level == "info") LOG_SET_LEVEL(utils::L_INFO);
	else if (Options::system.log_level == "trace") LOG_SET_LEVEL(utils::L_TRACE);
	else if (Options::system.log_level == "debug") LOG_SET_LEVEL(utils::L_DEBUG);
	else LOG_SET_LEVEL(utils::L_INFO);

	rc = configure.get_single("system", "log_level", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get system conf log_level error");
		return -1;
	}
	Options::system.log_level = val_string;

	long val_long;
	rc = configure.get_single("system", "log_enable_pack_print", val_long);
	if (0 != rc) 
	{
		Options::system.log_enable_pack_print = true;
	}
	else
	{
		Options::system.log_enable_pack_print = (bool)val_long;
		LOG_SET_PACK_PRINT(Options::system.log_enable_pack_print);
	}

	LOG_INFO("Configure, system, log_level : %s", Options::system.log_level.c_str());
	LOG_INFO("Configure, system, log_enable_pack_print : %d", 
						Options::system.log_enable_pack_print);

	return 0;
}


