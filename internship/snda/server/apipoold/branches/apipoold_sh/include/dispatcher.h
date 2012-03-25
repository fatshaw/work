
#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_


#include "types.h"
#include "net_manager.h"
#include "net_event.h"
#include "log.h"

#include <map>
#include <set>

using namespace std;


//! 网络事件处理器
typedef void (*pfnNetEventHandler)(Net_Event& net_event);


//! 时间事件处理器
typedef void (*pfnTimerTickHandler)(time_t current_time);


//! 主事件循环处理类
class Dispatcher
{
public:
	//! 注册网络事件处理器
	static void registerNetEventHandler(uint32_t net_id, pfnNetEventHandler handler);

	//! 注册时间事件处理器
	static void removeNetEventHandler(uint32_t net_id);

	//! 注销网络事件处理器
	static void registerTimerTickHandler(pfnTimerTickHandler handler);

	//! 注册时间事件处理器
	static void removeTimerTickHandler(pfnTimerTickHandler handler);

	//! 取网络管理器
	static Net_Manager* getNetManager();

	//! 运行事件循环
	static void run();

	//! 停止事件循环
	static void stop();

private:
	typedef map<uint32_t, pfnNetEventHandler> NetEventHandlerMap;

	typedef set<pfnTimerTickHandler> TimerTickHandlerSet;

	static NetEventHandlerMap m_net_event_handler_map;

	static TimerTickHandlerSet m_timer_tick_handler_set;

	static Net_Manager m_net_manager;

	static bool m_stop;
};


#endif // _DISPATCHER_H_
