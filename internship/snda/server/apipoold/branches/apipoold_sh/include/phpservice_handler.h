#ifndef _PHPSERVICE_HANDLER_H
#define _PHPSERVICE_HANDLER_H

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
using namespace std;
using namespace ApiPool;



typedef map<string,string> ArgsMap;

#ifdef DEBUG
#define FCGIAGENT_REQUEST_HASH_SIZE		(1 << 5)
#else
#define FCGIAGENT_REQUEST_HASH_SIZE		(1 << 10)
#endif
#define FCGIAGENT_REQUEST_HASH_MASK		(FCGIAGENT_REQUEST_HASH_SIZE - 1)


#define FCGIAGENT_CONNECT_TIMEOUT 5

#define FCGIAGENT_RECONNECT_INTERVAL 5

#define FCGIAGENT_KEEPALIVE_INTERVAL 5

#define MAX_SERVICE_WORKER_COUNT 16

#define FCGIAGENT_REQUEST_TIMEOUT 			5


//! 中间层服务器信息
struct FcgiAgentInfo
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
	list_head req_hash[FCGIAGENT_REQUEST_HASH_SIZE];
	list_head req_list;
	uint32_t curSeq;
public:
	FcgiAgentInfo()
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

class FcgiServiceInfo
{
public:
	FcgiServiceInfo()
	{
		serviceName = "";
		worker_count = 0;
		LOG_DEBUG("MEM TEST new FcgiServiceInfo\n");
	}

	~FcgiServiceInfo()
	{
		LOG_DEBUG("MEM TEST delete FcgiServiceInfo\n");
	}
		
	string serviceName;
	uint8_t worker_count;
	FcgiAgentInfo agentinfos[MAX_SERVICE_WORKER_COUNT];
};

//map   servicename --- serviceinfo
typedef map<string, FcgiServiceInfo*> FcgiServiceInfos;

#pragma pack(1)
struct FCGI_AGENT_HDR
{
	uint32_t len;
	uint16_t  cmd;
	uint16_t  ver;
	uint32_t seq;
};

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


#pragma pack()


#define MAX_FCGI_REQ_LEN 40960



class FcgiAgentRequest
{
public:

	list_head hash;
	list_head lst;

	FCGI_AGENT_HDR header;
	time_t timeout;
	struct timeval tv_begin;
	uint8_t status;
	char databuf[MAX_FCGI_REQ_LEN];
	uint32_t databuf_len;
	Request* request;
	string targetIp;
	uint16_t targetPort;

public:
	FcgiAgentRequest(FcgiAgentInfo* agentinfo)
		: timeout(time(NULL)+FCGIAGENT_REQUEST_TIMEOUT)
	{
		LOG_DEBUG("MEM TEST new FcgiAgentRequest this:%p",
					this);
		request = NULL;
		databuf_len = 0;
		status = REQ_INIT;
		targetIp = agentinfo->ip;
		targetPort = agentinfo->port;
		header.cmd = 0;
		header.len = 0;
		header.ver = 0;
		header.seq = 0;
		gettimeofday(&tv_begin,NULL);
	};

	~FcgiAgentRequest()
	{
		LOG_DEBUG("MEM TEST delete ~FcgiAgentRequest this:%p",
					this);
		if(request != NULL)
		{			
			struct timeval tv_end;
			gettimeofday(&tv_end,NULL);
			uint64_t timespan = getTimeSpan(tv_begin,tv_end);
			LOG_INFO("[STATMSG FcgiAgentRequest reqsrc:%s, orgip:%s, port:%d, net_id:%d, orgreqid:%d, serviceName:%s,method:%s,targetip:%s,targetPort:%d,sendreqid:%d,time:%lu(us), status:%d]\n",
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

#ifndef DISABLE_MEMPOOL
	DECLARE_SLAB(FcgiAgentRequest)
#endif
};


typedef map<uint32_t, FcgiAgentInfo*> Net_FcgiAgent_Map;

class FcgiAgentHandler
{


public:
	FcgiAgentHandler(){}

	~FcgiAgentHandler(){}

public:
		
	static void NetEventHandler(Net_Event& net_event);

	static void stop();
	static void TimerTickHandler(time_t current_time);

	static int initializeService(ServcieInfo& s_info);
	static int initialize();

	static void allocate_new_service(string serviceName);
	
	static int reconfigure();

	static int updateService(string servicename ,int  index, string ip,int  port);
	static int connect();
	static int connect(string& serviceName);
	
	static int connect(FcgiAgentInfo& fcgiagent_info);

	static int disconnect();
	static int disconnect(string& serviceName);

	static int disconnect(FcgiAgentInfo& fcgiagent_info);

	
	static int updateFcgiAgent(string servicename, uint16_t index, string ip, uint16_t port);

//		static int updateBakFcgiAgent(string ip, uint16_t port);

	

	static void addToReqList(FcgiAgentRequest* pReq, FcgiAgentInfo & fcgiagent_info);
	static FcgiAgentRequest* findReqList(uint32_t seq, FcgiAgentInfo & fcgiagent_info);
	static void deleteReqList(FcgiAgentRequest* pReq, FcgiAgentInfo & fcgiagent_info);

	
	static void checkTimeout(time_t current_time, FcgiAgentInfo & fcgiagent_info);

	static void sendKeepAlive(FcgiAgentInfo& fcgiagent_info);
	static void onRecvKeepAlive(FcgiAgentInfo *fcgiagent_info);


	static void makeBeginRequestBody(unsigned short  role,unsigned char  keepConnection,unsigned char * bin );
	
	static void makeHeader(unsigned char type , unsigned short requestID, unsigned short contentLength, unsigned char paddingLength,unsigned char * header);
		
	static unsigned short makeNameValueHeader(const string & name, const string & value ,unsigned char * tmpBuf);
	
	static int  makeFgiPack(const string & method, char  * buf , int buf_len, const map<string,string> & args,uint16_t recordID, uint16_t& lengh );

/******************************************************************************
 * FcgiAgentHandler.transfer_2_fcgi_proto - 将Json的请求转换成Fcgi的请求报文，不负责组织包头, 
 	并将封装好的报文放置在FcgiAgentRequest的databuf中
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 23aug2010, Davidfan written
 * --------------------
 ******************************************************************************/
	static int transfer_2_fcgi_proto(Request& req, FcgiAgentRequest& fcgiReq);

/******************************************************************************
 * FcgiAgentHandler.transfer_from_fcgi_proto - 解析返回的Fcgi报文，重组为json，并放置在response中
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 23aug2010, Davidfan written
 * --------------------
 ******************************************************************************/
	static int transfer_from_fcgi_proto(Request& request, const char* fcgi_resp_buf, int fcgi_buf_len, Response& resq);
	static void recvAgentHeader(FCGI_AGENT_HDR * hdr,const char * buf, int  offset);

	static void recvFcgiHeader(FCGI_HDR * hdr,const char * buf, int offset);
	static int analyzeResult(string & result,int & errorCode, string & errorInfo);
	static int getresult(const string & content,string & result, int & errorCode, string & errorInfo);

	static int  parseData(const char * recvData, uint32_t len, char* content, int& content_len);


	static int handleRequest(Request* req);

	static void handleResponse(FcgiAgentRequest* pReq, Net_Event& net_event, FcgiAgentInfo & fcgiagent_info);

	static void getFcgiAgentHandlerInformation(char* buf, int len);

private:
	static FcgiAgentInfo* getFcgiAgentByReq(Request& req);
	static FcgiAgentInfo* getFcgiAgentByNetId(uint32_t net_id);

	//! 连接成功事件
	static void onConnectOk(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event);

	
	static void onConnectError(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event);

	
	static void onNetData(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event);
	static void clearFcgiAgentStateinfo(FcgiAgentInfo& fcgiagent_info);

private:
	//! tcp拆包器
	static Binary_Splitter m_packet_splitter;

	static FcgiServiceInfos fcgiServiceInfos;

	static Net_FcgiAgent_Map net_fcgiagent_map;

};

#endif  /* _PHPSERVICE_HANDLER_H */
