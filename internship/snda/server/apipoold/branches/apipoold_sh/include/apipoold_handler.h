#ifndef _APIPOOLD_HANDLER_H
#define _APIPOOLD_HANDLER_H

#include <string>

#include "net_manager.h"
#include "ApiPoolRequest.h"
#include "ApiPoolResponse.h"
#include "list_map.h"
#include "common.h"

using namespace std;
using namespace ApiPool;

//! apipoold处理器
class ApipooldHandler
{
public:
	static void NetEventHandler(Net_Event& net_event);
	static void TimerTickHandler(time_t current_time);
	static void onNetConnect(Net_Event& net_event);
	static void getApipooldHandlerInformation(char* buf, int len);
	static void sendResponse(Request& request, Response& response);	
	static void sendCustomResponse(int http_err_code, Request& request,const char * errorInfo);

private:
	static void checkTimeoutUser(time_t current_time);
	static void onNetError(Net_Event& net_event);
	static void onNetData(Net_Event& net_event);
	static void tryCloseClient(Request& request);
	static void remove_net(uint32_t net_id);

private:
	static ListMap<NetInfo> net_map_longtcp;	//---这里只保存需要超时检查的长连接 
	static ListMap<NetInfo> net_map_shorttcp;	//---这里只保存需要超时检查的短连接

};

#endif  /* _APIPOOLD_HANDLER_H */
