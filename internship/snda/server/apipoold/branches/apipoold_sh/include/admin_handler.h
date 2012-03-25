
#ifndef _ADMIN_HANDLER_H_
#define _ADMIN_HANDLER_H_


//
#include "types.h"
#include "net_event.h"
#include "line_splitter.h"

#include <string>

using namespace std;


//! 命令处理函数
//!
//! 每个连接服务器必须实现一个此函数签名的函数, 注册到Admin处理器上, 处理用户发送的管理命令
typedef void (*pfnUserCmdHandler)(const char *cmd, int cmd_len, char *result_buf, int result_buf_len, int& result_len);


//! udp管理服务信息
struct AdminUdpInfo
{
	uint32_t net_id;
	string admin_ip;			//!< 服务ip
	int admin_port;		//!< 服务port
	time_t reconnect_time;		//!< 下次尝试重新建立通道的时间
};

//! udp管理服务处理器
class AdminUdpHandler
{
public:
	//! 网络事件处理器
	static void NetEventHandler(Net_Event& net_event);

	//! 时间事件处理器
	static void TimerTickHandler(time_t current_time);

public:
	//! 初始化
	static int initialize();

	//! 设置命令处理函数
	static void setUserCmdHandler(pfnUserCmdHandler handler);

	//! 启动
	static int start();

	//! 停止
	static int stop();

private:
	//! 网络错误事件
	static void onNetError(Net_Event& net_event);

	//! 网络数据事件
	static void onNetData(Net_Event& net_event);

private:
	//! 命令处理函数
	static pfnUserCmdHandler user_cmd_handler;

	//! udp管理服务信息
	static AdminUdpInfo admin_udp_info;

	//! 重建立通道时间间隔
	static int ADMIN_RECONNECT_INTERVAL;
};




//! tcp管理服务信息
struct AdminTcpInfo
{
	uint32_t net_id;
	string admin_ip;			//!< 服务ip
	int admin_port;		//!< 服务port
	time_t reconnect_time;		//!< 下次尝试重新建立通道的时间
};

//! tcp管理服务监听器
class AdminTcpListener
{
public:
	//! 网络事件处理器
	static void NetEventHandler(Net_Event& net_event);

	//! 时间事件处理器
	static void TimerTickHandler(time_t current_time);

public:
	//! 初始化
	static int initialize();

	//! 设置命令处理函数
	static void setUserCmdHandler(pfnUserCmdHandler handler);

	//! 启动
	static int start();

	//! 停止
	static int stop();

private:
	//! 网络错误事件
	static void onNetError(Net_Event& net_event);

	//! 网络数据事件
	static void onNetAccept(Net_Event& net_event);

public:
	//! 命令处理函数
	static pfnUserCmdHandler user_cmd_handler;

private:
	//! tcp管理服务信息
	static AdminTcpInfo admin_tcp_info;

	//! 重建立通道时间间隔
	static int ADMIN_RECONNECT_INTERVAL;

	//! tcp拆包器
	static Line_Splitter m_packet_splitter;
};


//! tcp管理服务处理器
class AdminTcpHandler
{
public:
	//! 网络事件处理器
	static void NetEventHandler(Net_Event& net_event);

private:
	//! 网络错误事件
	static void onNetError(Net_Event& net_event);

	//! 网络数据事件
	static void onNetData(Net_Event& net_event);
};


#endif // _ADMIN_HANDLER_H_
