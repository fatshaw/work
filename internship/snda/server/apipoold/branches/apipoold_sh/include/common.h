#ifndef _COMMON_H
#define _COMMON_H
#include <sys/time.h>
using namespace std;
#include <map>
#include "log.h"

#include "ApiPoolError.h"

//! Socket信息
struct NetInfo
{
	uint32_t net_id;					//对应的Net Manager通道id
	uint64_t updateTime;				//最后一次应用该连接的time
	char ip[24];					//!< 监听ip
	uint16_t port;					//!< 监听port
	NetInfo()
	{
		LOG_DEBUG("MEM TEST new NetInfo\n");
	}

	~NetInfo()
	{
		LOG_DEBUG("MEM TEST delete NetInfo\n");
	}
};

class ListenInfo :public NetInfo
{
public:
	time_t reconnect_time;		//!< 下次尝试重新建立通道的时间		
};

//记录netid 和其Net详细信息的对应
typedef map<uint32_t, NetInfo> NetMap;


typedef enum
{
	RET_OK = 0,

	//1-5
	RET_ILLEGAL_RELATION,
	RET_NET_SEND_ERROR,
	RET_NO_MATCH_SESSION,
	RET_FAIL,
	RET_PARSE_HEAD_ERROR,

	//6-10
	RET_PARSE_PARAM_ERROR,
	RET_MID_FAIL,
	RET_INTERNAL_ERROR,
	RET_ILLEGAL_CLIENT,
	RET_DATA_NOT_EXIST,	
	
	RET_AMOUT_EXCEED,
};

typedef enum
{
	REQ_INIT = 0,
	REQ_SUCC = 1,
	REQ_TIMEOUT = 2,
};



//#define	HTTP_BAD_REQUEST  400
//#define HTTP_STATUS_UNAUTHORIZED 401
//#define HTTP_STATUS_BAD_PACKET_DATA 403
//#define HTTP_STATUS_BAD_SERVICE_NAME 406
//#define HTTP_STATUS_SERVICE_UNAVAILABLE 407
//#define	HTTP_REQUEST_TIMEOUT  408
//#define	HTTP_INTERNAL_ERROR 500
//#define	HTTP_PROTOCOL_ERROR  505

//char * getErrorInfo(int errorCode);

#define MAX_PHP_PACK_LEN 102400

unsigned long getTimeSpan(struct timeval tv_begin, struct timeval tv_end);
#endif  /* _COMMON_H */
