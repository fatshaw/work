#include <string>
#include <iostream>
#include "configure.h"
#include "log.h"
#include "admin_handler.h"
#include "user_cmd_handler.h"
#include "dispatcher.h"
#include "common.h"
#include "protocol.h"
using namespace std;

char * configfile = "phpfcgiagentd.cfg";
utils::Configure configure;

Dispatcher* p_dispather = NULL;

int m_sMaxLogSize = 100000000;
char m_szLogPath[100] = {0};
int loglevel = 5;

string adminIp;
int adminPort;
int reconnectInterval;

extern Net_Manager *net_manager;

static void sigterm_handler(int signo) 
{
	if(p_dispather != NULL)
	{
		p_dispather->local_stop = 1;		
		LOG_INFO("[main] Got Term sigle, stop the worker p_dispather->local_stop =%d!", p_dispather->local_stop);
	}
}

#if 1
//精灵进程，后台启动
void DaemonServer()
{
	signal(SIGINT, sigterm_handler);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, sigterm_handler);
}
#endif

int init_log(utils::Configure &configure, bool is_reload)
{
	std::string val_string;

	int rc = configure.get_single("Log", "MaxLogSize", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get Log MaxLogSize conf error\n");
		return -1;
	}
	m_sMaxLogSize = atoi(val_string.c_str());
	LOG_INFO("MaxLogSize:%d\n",m_sMaxLogSize);
	
	rc = configure.get_single("Log", "LogPath", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get Log LogPath conf error\n");
		return -1;
	}
	strcpy(m_szLogPath, val_string.c_str());
	LOG_INFO("LogPath:%s\n",m_szLogPath);
	
	rc = configure.get_single("Log", "LOGLEVEL", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get Log LOGLEVEL conf error\n");
		return -1;
	}
	loglevel = atoi(val_string.c_str());
	LOG_INFO("LOGLEVEL:%d\n",loglevel);

	//日志
	long val_long = 0;
	bool log_enable_pack_print = true;
	rc = configure.get_single("Log", "log_enable_pack_print", val_long);
	if (0 != rc) 
	{
		log_enable_pack_print = true;
	}
	else
	{
		log_enable_pack_print = (bool)val_long;
	}
	LOG_INFO("log_enable_pack_print:%d\n", log_enable_pack_print);
	
	if(is_reload ==false){
		LOG_INIT((std::string(m_szLogPath) + "phpfcgiagentd").c_str(),
			 m_sMaxLogSize,
			 loglevel);
		rc = LOG_OPEN();
		LOG_INFO("[Init Log]");
	}
	else if(is_reload == true)
	{
		LOG_SET_LEVEL(loglevel);
		LOG_INFO("[Set Log leve = %d]",loglevel);
	}
	LOG_SET_USEC(true);
	LOG_SET_PACK_PRINT(log_enable_pack_print);
	if(rc != 0)
	{
		LOG_FATAL("[init_log] failed ret=%d\n", rc);
		return -1;
	}
	return 0;
}

int init_Admin_Info()
{
	std::string val_string;

	int rc = configure.get_single("admin_server", "admin_ip", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get admin_server admin_ip conf error\n");
		return -1;
	}
	adminIp = val_string;
	rc = configure.get_single("admin_server", "admin_port", val_string);
	if (0 != rc) {
		LOG_FATAL("Configure error, get admin_server admin_port conf error\n");
		return -1;
	}
	adminPort = atoi(val_string.c_str());
	LOG_INFO("configure get admin port = %d\n",adminPort);

	rc = configure.get_single("admin_server", "reconnect_interval", val_string);
	if (0 != rc) {
		LOG_FATAL( "Configure error, get admin_server reconnect_interval conf error\n");
		return -1;
	}
	reconnectInterval = atoi(val_string.c_str());
	return 0;
}

#ifndef TEST
int main(int argc, char ** argv)
{
	//int workerNum = 0;
	//int i = 0;
	int iRet = 0;
	
	if (argc == 2) {
		configfile = argv[1];
	}
	
	//读取配置文件
	int iTempRet = configure.load(configfile);

	if( iTempRet != 0 )
	{
		LOG_FATAL("Read Config File Fail ret=%d!\n", iTempRet);
		return -1;
	}
	
	DaemonServer();

	iRet = init_log(configure);	
	if(iRet !=0){
		LOG_FATAL("[Main]Init log error\n");
	}
	LOG_SET_USEC(true);
	iRet = init_Admin_Info();
	if(iRet != 0){
		LOG_FATAL("[Main]Init  Admin Info error \n");
	}

	net_manager = new Net_Manager();
	iRet = net_manager->start();
	if(0 != iRet)
	{
		LOG_ERROR("[Dispatcher][init_net_manager] net_manager_app start error ret=%d", iRet);
		return RET_NET_MANAGER_START_FAIL;
	}
	sleep(1);	//Wait net_manager to complete init work.
	LOG_INFO("[Dispatcher][init_net_manager] net_manager_app start success");
	
	p_dispather = new Dispatcher(); 
	if(0 != p_dispather->activate())
	{
		LOG_ERROR("Dispatcher activate failed:%s",
				 strerror(errno));
		return -1;
	}

	LOG_DEBUG("ACTIVE SERVER\n");
	AdminTcpListener::initialize();
	AdminTcpListener::setUserCmdHandler(UserCmdHandler);
	AdminTcpListener::start();

	LOG_INFO("[All Service start] Success");

	p_dispather->wait();
	delete p_dispather;
	p_dispather	= NULL;
	LOG_INFO("[Dispatcher]stoped, all service stop.");
	LOG_INFO("Service shutdown");

	LOG_CLOSE();

	return 0;
}

#endif


