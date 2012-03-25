#ifndef  _ADMIN_HANDLER_H
#define _ADMIN_HANDLER_H
#include "net_manager/src/net_event.h"
#include "net_manager/src/line_splitter.h"
#include "dispatcher.h"
#include <string>
#include "log.h"
#include "definition.h"
#include "common.h"

extern NetInfo admin_connected;
extern string adminIp;
extern int adminPort;
extern int reconnectInterval;

extern Net_Manager * net_manager;

typedef void (*pfnUserCmdHandler)(const char *cmd, int cmd_len, char *result_buf, int result_buf_len, int& result_len);

struct AdminTcpInfo
{
	uint32_t net_id;
	std::string adminIp;
	int adminPort;
	time_t reconnect_time;
};

class AdminTcpHandler
{
public:
	static void NetEventHandler(Net_Event & net_event);
private:
	static void onNetError(Net_Event& net_event);
	static void onNetData(Net_Event& net_event);
};

class AdminTcpListener
{
public:
	static void NetEventHandler(Net_Event& net_event);

	static int onNetAccept(Net_Event & net_event);

	static void TimerTickHandler(time_t currentTime);

	static int initialize();
	static int start();
	static void setUserCmdHandler(pfnUserCmdHandler handler);

	static int stop();
	static void onNetError(Net_Event & net_event);
public:
	static pfnUserCmdHandler user_cmd_handler;
	static AdminTcpInfo adminTcpInfo;
	static int RECONNECT_TIME_INTERVAL;
	static Line_Splitter m_line_splitter;	
};

#endif  /* _WEB_WORKER_H */
