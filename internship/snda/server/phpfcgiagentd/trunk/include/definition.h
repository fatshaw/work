#ifndef _DEFINITION_H
#define _DEFINITION_H
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <net_manager/src/net_manager.h>
#include "msg_queue.h"
using namespace std;

using namespace utils;

typedef MessageQueue<Net_Event*> Request_queue;


typedef enum
{
	NET_OFFLINE = 0,
	NET_LISTENED,
	NET_CONNECTTING,
	NET_CONNECTED,
	NET_TIMEOUT,
	NET_ERROR,
	NET_CLOSE,
	NET_LOCAL_CLOSE,
	NET_UNAVALIABLE,

	NET_STATUS_BUTT = 255,

}Net_Status;


//! Socket信息
typedef struct _NetInfo
{
	uint32_t net_id;					//对应的Net Manager通道id
	uint64_t updateTime;				//最后一次应用该连接的time
	char ip[24];					//!< 监听ip
	uint16_t port;					//!< 监听port
	Net_Status	 status;			//标识本链接状态
	int32_t processing;			//当前正在处理的请求数
#ifdef MULTI_WORKER
	pthread_mutex_t m_thread_mutex;
#endif
}NetInfo;

//记录netid 和其Net详细信息的对应
typedef map<uint32_t, NetInfo> NetMap;


typedef enum
{
	NET_SOURCE_CONNECTD = 1,
	NET_SOURCE_WEB = 2,
	NET_ADMIN_SOURCE = 3,
	NET_SOURCE_BUTT = 255,
}NET_SOURCE;


#define MAX_PHP_PACK_LEN 204800
#endif  /* _DEFINITION_H */
