
#ifndef _APIPOOLD_LISTENER_H
#define _APIPOOLD_LISTENER_H

#include <string>

#include "net_manager.h"

#include "string_splitter.h"
#include "common.h"

using namespace std;


//! apipoold监听器
class ApipooldListener
{
public:
	//! 网络事件处理器
	static void NetEventHandler(Net_Event& net_event);

	//! 时间事件处理器
	static void TimerTickHandler(time_t current_time);

public:
	//! 初始化
	static int initialize();

	//! 启动监听
	static int start();

	//! 停止监听
	static int stop();

private:
	//! 网络错误事件
	static void onNetError(Net_Event& net_event);

	//! 网络数据事件
	static void onNetAccept(Net_Event& net_event);

public:
	//! apipoold监听器信息
	static ListenInfo apipoold_longtcp_info;
	static ListenInfo apipoold_shorttcp_info;

private:
	//! tcp拆包器
	static String_Splitter m_packet_splitter;
};




#endif  /* _APIPOOLD_LISTENER_H */

