#ifndef _STATESERVER_HANDLER_H
#define _STATESERVER_HANDLER_H

#include <sys/time.h>

#include "List.h"
#include "net_manager.h"
#include "binary_splitter.h"
#include "MemoryPool.h"

#include "ApiPoolRequest.h"
#include "ApiPoolResponse.h"

#include "common.h"
#include <string>
#include <vector>
#include "options.h"
#include "log.h"
#include <string>
#include "tokenMessage.pb.h"
using namespace std;
using namespace ApiPool;



typedef map<string,string> ArgsMap;

#ifdef DEBUG
#define PROTOBUF_REQUEST_HASH_SIZE		(1 << 5)
#else
#define PROTOBUF_REQUEST_HASH_SIZE		(1 << 10)
#endif
#define PROTOBUF_REQUEST_HASH_MASK		(PROTOBUF_REQUEST_HASH_SIZE - 1)


#define PROTOBUF_CONNECT_TIMEOUT 5

#define PROTOBUF_RECONNECT_INTERVAL 5

#define PROTOBUF_KEEPALIVE_INTERVAL 5

#define MAX_SERVICE_WORKER_COUNT 16

#define PROTOBUF_REQUEST_TIMEOUT 			5


//! 中间层服务器信息
struct ProtobufServerInfo
{
	uint32_t net_id;
	string serviceName;
	string ip;
	int port;
	bool connected;
	time_t reconnect_time;		//!< 下次尝试重新连接的时间
	time_t keepalive_time;		//!< 下次发keepalive的时间
	time_t last_active_time;    //!< 最后收到对端的数据包时间

	// 每个serverlist对象需要保存处理中的数据包
	uint32_t req_count;
	list_head req_hash[PROTOBUF_REQUEST_HASH_SIZE];
	list_head req_list;
	uint32_t curSeq;
public:
	ProtobufServerInfo()
	{
		net_id = 0;
		serviceName = "";
		ip = "";
		port = 0;
		connected = false;
		reconnect_time = 0;		//!< 下次尝试重新连接的时间
		keepalive_time = 0;		//!< 下次发keepalive的时间
		last_active_time = 0;	//!< 最后收到对端的数据包时间
	}
};

class ProtobufServerServiceInfo
{
public:
	ProtobufServerServiceInfo()
	{
		serviceName = "";
		worker_count = 0;
		LOG_DEBUG("MEM TEST new ProtobufServerServiceInfo\n");
	}

	~ProtobufServerServiceInfo()
	{
		LOG_DEBUG("MEM TEST delete ProtobufServerServiceInfo\n");
	}
		
	string serviceName;
	uint8_t worker_count;
	ProtobufServerInfo protobufServerInfos[MAX_SERVICE_WORKER_COUNT];
};

//map   servicename --- serviceinfo
typedef map<string, ProtobufServerServiceInfo*> ProtobufServerServiceInfos;


#pragma pack(1)

struct PROTOBUF_SERVER_HDR
{
	uint32_t seq;
	uint32_t cmd;
};

enum PROTOBUF_SERVER_CMD
{
	STATE_KEEP_ALIVE= 0,
	STATE_REQUEST = 1,
	STATE_RESPONSE = 2,
};
/*
typedef struct 
{
	char version;
	char type;
	unsigned short requestID;
	unsigned short contentLength;
	char paddingLength;
	char reserved;
	
}FCGI_HDR;

#define FCGI_VERSION_1  1
#define FCGI_BEGIN_REQUEST 1
#define FCGI_RESPONSE 1
#define FCGI_ABORT_REQUEST 2
#define FCGI_END_REQUEST 3
#define FCGI_PARAMS 4
#define FCGI_STDIN 5
#define FCGI_STDOUT 6
#define FCGI_STDERR 7
#define FCGI_DATA 8

*/
#pragma pack()


#define MAX_PROTOBUF_REQ_LEN 40960



class ProtobufServerRequest
{
public:
	list_head hash;
	list_head lst;

	PROTOBUF_SERVER_HDR header;
	time_t timeout;
	struct timeval tv_begin;
	uint8_t status;
	char databuf[MAX_PROTOBUF_REQ_LEN];
	uint32_t databuf_len;
	Request* request;
	string targetIp;
	uint16_t targetPort;
public:
	ProtobufServerRequest(ProtobufServerInfo* protobufServerInfo)
		: timeout(time(NULL)+PROTOBUF_REQUEST_TIMEOUT)
	{
		LOG_DEBUG("MEM TEST new ProtobufServerRequest this:%p",
					this);
		request = NULL;
		databuf_len = 0;
		status = REQ_INIT;
		targetIp = protobufServerInfo->ip;
		targetPort = protobufServerInfo->port;
		header.cmd = 0;
		header.seq = 0;
		gettimeofday(&tv_begin,NULL);
	};

	~ProtobufServerRequest()
	{
		LOG_DEBUG("MEM TEST delete ~ProtobufServerRequest this:%p",
					this);
		if(request != NULL)
		{			
			struct timeval tv_end;
			gettimeofday(&tv_end,NULL);
			uint64_t timespan = getTimeSpan(tv_begin,tv_end);
			LOG_INFO("[STATMSG ProtobufServerRequest reqsrc:%s, orgip:%s, port:%d, net_id:%d, orgreqid:%d, serviceName:%s,method:%s,targetip:%s,targetPort:%d,sendreqid:%d,time:%lu(us), status:%d]\n",
						request->getRequestSrc().c_str(),
						request->getIp().c_str(),
						(int)request->getPort(),
						(int)request->getNetId(),
						(int)request->getId().asInt(),
						request->getServiceName().c_str(),
						request->getApiName().c_str(),
						targetIp.c_str(),targetPort,header.seq,
						timespan,
						status);
			delete request;
			request = NULL;
		}
	};

public:
	static string typeName;

#ifndef DISABLE_MEMPOOL
	DECLARE_SLAB(ProtobufServerRequest)
#endif
};

typedef map<uint32_t, ProtobufServerInfo*> Net_ProtobufServer_Map;

class ProtobufServerHandler
{


public:
	ProtobufServerHandler(){}

	~ProtobufServerHandler(){}

public:
		
	static void NetEventHandler(Net_Event& net_event);

	static void stop();
	static void TimerTickHandler(time_t current_time);

	static int initializeService(ServcieInfo& s_info);
	static int initialize();

	static void allocate_new_service(string & serviceName);
	static int updateService(string & servicename ,int index, string ip, int port);
	static int reconfigure();

	static int connect();
	static int connect(string& serviceName);
	
	static int connect(ProtobufServerInfo& protobufServerInfo);

	static int disconnect();
	static int disconnect(string& serviceName);

	static int disconnect(ProtobufServerInfo& protobufServerInfo);

	
	static int updateStateServer(string & servicename, uint16_t index, string ip, uint16_t port);
	

	static void addToReqList(ProtobufServerRequest* pReq, ProtobufServerInfo & protobufServerInfo);
	static ProtobufServerRequest* findReqList(uint32_t seq, ProtobufServerInfo & protobufServerInfo);
	static void deleteReqList(ProtobufServerRequest* pReq, ProtobufServerInfo & protobufServerInfo);

	
	static void checkTimeout(time_t current_time, ProtobufServerInfo & protobufServerInfo);

	static void sendKeepAlive(ProtobufServerInfo& protobufServerInfo);
	static void onRecvKeepAlive(ProtobufServerInfo *protobufServerInfo);

	static int handleRequest(Request* req);

	static void handleResponse(ProtobufServerRequest* pReq, StateServer::TokenMessage & tokenMessage);

	static void getProtobufServerHandlerInformation(char* buf, int len);

private:
	static ProtobufServerInfo* getProtobufServerByReq(Request& req);
	static ProtobufServerInfo* getProtobufServerByNetId(uint32_t net_id);

	//! 连接成功事件
	static void onConnectOk(ProtobufServerInfo & protobufServerInfo, Net_Event& net_event);

	
	static void onConnectError(ProtobufServerInfo& protobufServerInfo, Net_Event& net_event);

	
	static void onNetData(ProtobufServerInfo & protobufServerInfo, Net_Event& net_event);
	static void clearProtobufServerStateinfo(ProtobufServerInfo& protobufServerInfo);
	
	 static int encodeProtobuf(Request& req, ProtobufServerRequest& protobufServerRequest);
	
	static int encodeKeepAlivePack(char * buf);

	static int makeMessagePacket(string  typeName, uint32_t protobufLength,  char * protobuf,Net_Packet *  net_packet);

private:
	//! tcp拆包器
	static Binary_Splitter m_packet_splitter;

	static ProtobufServerServiceInfos protobufServerServiceInfos;

	static Net_ProtobufServer_Map net_ProtobufServer_Map;

};

#endif  /* _PHPSERVICE_HANDLER_H */
