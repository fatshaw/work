
#ifndef _OPTIONS_H_
#define _OPTIONS_H_


#include <string>
#include <vector>

using namespace std;



struct IpPort
{
	string ip;
	int port;
};


// 管理服务配置
struct AdminServer
{
	string admin_ip;
	int admin_port;
	int reconnect_interval;
};




// 通知服务配置
struct NotifyServer
{
	vector<IpPort> notify_list;
	int connect_timeout;
	int reconnect_interval;
	int keepalive_interval;
};



// apipoold服务配置
struct Apipoold
{
	string apipoold_longtcp_ip;
	int apipoold_longtcp_port;
	int longtcp_timeout;

	string apipoold_shorttcp_ip;
	int apipoold_shorttcp_port;
	int shorttcp_timeout;
	string rpcRoot;


	uint32_t shm_key;

};

// Webagent服务配置
struct Webagent
{
	string webagent_ip;
	int webagent_port;
};



// 系统配置
struct System
{
	string log_level;
	bool log_enable_pack_print;
};

struct ServcieInfo
{
	string serviceName;
	string ip;
	int port;
	int index;		
};

struct PhpService
{
	vector<ServcieInfo>	service_list;	
};

struct ProtobufService
{
	vector<ServcieInfo>	service_list;	
};



//! 连接服务器配置数据
struct Options
{
	// 管理服务配置
	static AdminServer admin_server;

	// apipoold服务配置
	static Apipoold apipoold;
	
	// 系统配置
	static System system;

	static PhpService phpservice;

	static ProtobufService stateServer;
};


#endif // _OPTIONS_H_
