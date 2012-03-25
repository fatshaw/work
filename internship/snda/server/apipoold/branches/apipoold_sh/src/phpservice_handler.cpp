#include <sstream>


#include "phpservice_handler.h"
#include "service_router.h"
#include "dispatcher.h"
#include "common.h"
#include "apipoold_protocol.h"

#include "options.h"

#include <json/json.h>



#include "binary_stream.h"

#include <string>


FcgiServiceInfos FcgiAgentHandler::fcgiServiceInfos;
Net_FcgiAgent_Map FcgiAgentHandler::net_fcgiagent_map;


#ifndef DISABLE_MEMPOOL
#ifdef DEBUG
	IMPLEMENT_SLAB("FcgiAgentRequest",FcgiAgentRequest, 2)
#else
	IMPLEMENT_SLAB("FcgiAgentRequest",FcgiAgentRequest, 256)
#endif
#endif


string default_svc = "Foo";
string defaultip = "127.0.0.1";
int defaultport = 9869;


static inline int hashfn(uint32_t seq)
{
	return (seq & FCGIAGENT_REQUEST_HASH_MASK);
}

static inline uint32_t hash_pjw(const char *str, size_t len)
{
	uint32_t hash = 0;

	for (size_t i = 0; i < len; i++)
	{
		const char temp = str[i];
		hash = (hash << 4) + (temp * 13);

		uint32_t g = hash & 0xf0000000;

		if (g)
		{
			hash ^= (g >> 24);
			hash ^= g;
		}
	}

	return hash;
}

int FcgiAgentHandler::reconfigure()
{
	int service_count = (uint32_t)Options::phpservice.service_list.size();

	LOG_INFO("FcgiAgentHandler::reconfigure, Phpfcgi server number:%lu", (unsigned long)Options::phpservice.service_list.size());

	for (int i = 0; i <service_count; i++)
	{
		string servicename = Options::phpservice.service_list[i].serviceName;
		string ip = Options::phpservice.service_list[i].ip;
		int port = Options::phpservice.service_list[i].port;
		int index =Options::phpservice.service_list[i].index;
		ServcieInfo& s_info = Options::phpservice.service_list[i];
		FcgiServiceInfos::iterator it = fcgiServiceInfos.find(s_info.serviceName);
		if(it == fcgiServiceInfos.end()){
			LOG_DEBUG("UPDATE  Service: No Service Name : %s\n",servicename.c_str());
			initializeService(s_info);
			connect(servicename);
		}
		else
		{
			updateService(servicename,index, ip, port);
		}
	}
	return 0;
}

int FcgiAgentHandler::updateService(string servicename ,int index, string ip, int port)
{
	LOG_INFO("FcgiAgentHandler::update phpservice handler for Sname:%s, server:[%s:%d]", 
						servicename.c_str(), ip.c_str(), port);

	FcgiServiceInfos::iterator it  = fcgiServiceInfos.find(servicename);
	if(it == fcgiServiceInfos.end()){
		LOG_DEBUG("UPDATE  Service: No Service Name : %s\n",servicename.c_str());
	}
	FcgiServiceInfo * serviceInfo = it->second;
	if ((serviceInfo->agentinfos[index].ip!= ip) || (serviceInfo->agentinfos[index].port != port))
	{
		//服务器发生变化
		LOG_INFO("FcgiAgentHandler::update phpservice handler, server addr changed, oldip:%s, oldport:%d, newip:%s, newport:%d, reconnect",
				serviceInfo->agentinfos[index].ip.c_str(), serviceInfo->agentinfos[index].port, ip.c_str(), port);
		int ret = 0;
		if(serviceInfo->agentinfos[index].port  != 0)
		{
			ret = disconnect(serviceInfo->agentinfos[index]);
		}
		serviceInfo->agentinfos[index].ip = ip;
		serviceInfo->agentinfos[index].port = port;
		if(ret != 0){
			LOG_DEBUG("[update Service]disconnect error\n");
		}
		else{
			connect(serviceInfo->agentinfos[index]);
		}
		return 0;
	}

	// 服务器没有发生变化
	LOG_INFO("phpService handler::updateservice, server addr not changed, ignore");
	return 0;
}

void FcgiAgentHandler::allocate_new_service(string serviceName)
{
	FcgiServiceInfo* fcgiServiceInfo = new FcgiServiceInfo();
	fcgiServiceInfo->serviceName = serviceName;	

	ServiceManager::registerServiceHandler(fcgiServiceInfo->serviceName.c_str(),
						handleRequest);
	
	for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
	{	
		fcgiServiceInfo->agentinfos[i].net_id = 0;
		fcgiServiceInfo->agentinfos[i].serviceName = fcgiServiceInfo->serviceName;;		
		fcgiServiceInfo->agentinfos[i].reconnect_time = 0;
		fcgiServiceInfo->agentinfos[i].keepalive_time = 0;
		fcgiServiceInfo->agentinfos[i].last_active_time = 0;

		fcgiServiceInfo->agentinfos[i].curSeq = 0;
		fcgiServiceInfo->agentinfos[i].req_count = 0;		

		INIT_LIST_HEAD(&fcgiServiceInfo->agentinfos[i].req_list);
		for(int j = 0; j < FCGIAGENT_REQUEST_HASH_SIZE; j++)
		{
			INIT_LIST_HEAD(&fcgiServiceInfo->agentinfos[i].req_hash[j]);
		}		
	}
	
	fcgiServiceInfos.insert(make_pair(fcgiServiceInfo->serviceName,fcgiServiceInfo));
	
	LOG_INFO("FcgiAgentHandler::allocate_new_service, servicename:%s, cur service size:%d", 
							serviceName.c_str(), (int)fcgiServiceInfos.size());

}

int FcgiAgentHandler::initializeService(ServcieInfo& s_info)
{
	FcgiServiceInfos::iterator it = fcgiServiceInfos.find(s_info.serviceName);

	if(it == fcgiServiceInfos.end())
	{
		allocate_new_service(s_info.serviceName);
	}

	if(s_info.index >= MAX_SERVICE_WORKER_COUNT)
	{
		LOG_ERROR("FcgiAgentHandler::initialize fail index num illegal, s_name:%s ip:%s port:%d index:%d",
							s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index);		
		return RET_FAIL;
	}

	FcgiServiceInfo* fcgiServiceInfo = fcgiServiceInfos[s_info.serviceName];	
	if(fcgiServiceInfo->agentinfos[s_info.index].ip
			!= "")
	{
		LOG_ERROR("FcgiAgentHandler::initialize fail index duplicated, s_name:%s ip:%s port:%d index:%d",
							s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index);	
		return RET_FAIL;
	}
	fcgiServiceInfo->agentinfos[s_info.index].ip = s_info.ip;
	fcgiServiceInfo->agentinfos[s_info.index].port = s_info.port;
	fcgiServiceInfo->worker_count++;

	LOG_INFO("FcgiAgentHandler::initialize succ index, s_name:%s ip:%s port:%d index:%d count:%d",
					s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index, fcgiServiceInfo->worker_count);	

	return RET_OK;

}

	

int FcgiAgentHandler::initialize()
{
	for(int i = 0; i<(int)Options::phpservice.service_list.size(); i++)
	{
		ServcieInfo& s_info = Options::phpservice.service_list[i];
		initializeService(s_info);
	}

	return 0;
}

int FcgiAgentHandler::disconnect()
{
	LOG_INFO("FcgiAgentHandler::disconnect, disconnect all fcgiagent begin");

	FcgiServiceInfos::iterator it = fcgiServiceInfos.begin();

	for(; it!= fcgiServiceInfos.end(); it++)
	{
		string serviceName = it->first;
		int ret = disconnect(serviceName);
		if (ret != RET_OK) 
		{
			LOG_WARN("FcgiAgentHandler::disconnect fail for sname:%s",
					it->first.c_str());
		}
	}

	LOG_INFO("FcgiAgentHandler::disconnect, disconnect all fcgiagent end");
	return RET_OK;
}

int FcgiAgentHandler::disconnect(string& serviceName)
{
	LOG_INFO("FcgiAgentHandler::disconnect, disconnect all fcgiagent begin");

	FcgiServiceInfos::iterator it = fcgiServiceInfos.find(serviceName);
	if(it == fcgiServiceInfos.end())
	{
		LOG_ERROR("FcgiAgentHandler::disconnect unknown service:%s",
					serviceName.c_str());
		return RET_FAIL;
	}

	FcgiServiceInfo& fcgiServiceInfo = *it->second;

	int rc = 0;

	for (size_t i = 0; i < fcgiServiceInfo.worker_count; i++)
	{
		int r = disconnect(fcgiServiceInfo.agentinfos[i]);
		if (0 != r) {
			rc = -1;
		}
	}

	LOG_INFO("FcgiAgentHandler::disconnect, disconnect all fcgiagent end");
	return rc;
}

int FcgiAgentHandler::disconnect(FcgiAgentInfo& fcgiagent_info)
{
	LOG_INFO("FcgiAgentHandler::disconnect begin, net_id:%d, server:[%s:%d]", fcgiagent_info.net_id, fcgiagent_info.ip.c_str(), fcgiagent_info.port);

	if (0 == fcgiagent_info.net_id) {		// 没有连接
		return 0;
	}
	clearFcgiAgentStateinfo(fcgiagent_info);

	Dispatcher::getNetManager()->delete_net(fcgiagent_info.net_id);
	net_fcgiagent_map.erase(fcgiagent_info.net_id);

	fcgiagent_info.net_id = 0;
	fcgiagent_info.connected = false;
	fcgiagent_info.reconnect_time = 0;
	fcgiagent_info.keepalive_time = 0;
	fcgiagent_info.last_active_time = 0;

	LOG_INFO("FcgiAgentHandler::disconnect ok");
	
	return 0;
}

void FcgiAgentHandler::sendKeepAlive(FcgiAgentInfo& fcgiagent_info)
{
	LOG_DEBUG("FcgiAgentHandler::sendKeepAlive, net_id:%d", fcgiagent_info.net_id);

	if (!fcgiagent_info.connected) 
	{
		LOG_FATAL("FcgiAgentHandler::sendKeepAlive error, not connect");
		return;
	}

	Net_Packet *net_packet = new Net_Packet;

	FCGI_AGENT_HDR* pHdr = (FCGI_AGENT_HDR*)net_packet->ptr();

	pHdr->len = htonl((uint32_t)sizeof(FCGI_AGENT_HDR));
	pHdr->cmd = htons(CMD_FCGIAGENT_KEEPALIVE);
	pHdr->ver = 0;
	pHdr->seq = 0;

	net_packet->length((uint16_t)sizeof(FCGI_AGENT_HDR));

	Net_Manager *net_manager = Dispatcher::getNetManager();
	int rc = net_manager->send_packet(fcgiagent_info.net_id, net_packet);
	if (0 != rc) 
	{
		delete net_packet;
		LOG_FATAL("FcgiAgentHandler::sendKeepAlive error, send_packet error, net_id:%d", fcgiagent_info.net_id);
		return;
	}

	fcgiagent_info.keepalive_time = time(NULL) + FCGIAGENT_KEEPALIVE_INTERVAL;
	return;
	
}

void FcgiAgentHandler::onRecvKeepAlive(FcgiAgentInfo *fcgiagent_info)
{
	LOG_DEBUG("FcgiAgentHandler::onRecvKeepAlive, ip:%s, port:%d", 
		fcgiagent_info->ip.c_str(), fcgiagent_info->port);
}

void FcgiAgentHandler::onConnectOk(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event)
{
	LOG_INFO("FcgiAgentHandler::onConnectOk, net_id:%d, ip:%s, port:%d", fcgiagent_info.net_id,
		fcgiagent_info.ip.c_str(), fcgiagent_info.port);

	fcgiagent_info.connected = true;
	fcgiagent_info.reconnect_time = 0;
	fcgiagent_info.keepalive_time = time(NULL) + FCGIAGENT_KEEPALIVE_INTERVAL;
}

void FcgiAgentHandler::addToReqList(FcgiAgentRequest* pReq, FcgiAgentInfo & fcgiagent_info)
{
	int i = hashfn(pReq->header.seq);
	list_add_tail(&pReq->hash, &fcgiagent_info.req_hash[i]);
	list_add_tail(&pReq->lst, &fcgiagent_info.req_list);
	++fcgiagent_info.req_count;

#ifdef LIST_TEST
		list_head* pos = NULL;
		int realcount = 0;
		list_for_each(pos, &fcgiagent_info.req_list)
		{
			realcount++;
		}	
		assert(realcount == (int)fcgiagent_info.req_count);
#endif	
	LOG_DEBUG("FcgiAgentHandler::addToReqList seq=%u,cmd=%u,count=%u\n", 
		pReq->header.seq, pReq->header.cmd, fcgiagent_info.req_count);
}


FcgiAgentRequest* FcgiAgentHandler::findReqList(uint32_t seq, FcgiAgentInfo & fcgiagent_info)
{
	int i = hashfn(seq);
	list_head* pos;
	FcgiAgentRequest* fcgiReq;
	list_for_each(pos, &fcgiagent_info.req_hash[i])
	{
		fcgiReq = list_entry(pos, FcgiAgentRequest, hash);
//	#ifdef LIST_TEST
//			assert(pos->next == &fcgiagent_info.req_hash[i]
//					&& pos->prev == &fcgiagent_info.req_hash[i]);
	//hash complict
//	#endif
		if(fcgiReq->header.seq == seq)	
			return fcgiReq;
	}
	return NULL;
}

void FcgiAgentHandler::deleteReqList(FcgiAgentRequest* pReq, FcgiAgentInfo & fcgiagent_info)
{
	list_del(&pReq->hash);
	list_del(&pReq->lst);
	--fcgiagent_info.req_count;
	
#ifdef LIST_TEST
		list_head* pos = NULL;
		int realcount = 0;
		list_for_each(pos, &fcgiagent_info.req_list)
		{
			realcount++;
		}	
		assert(realcount == (int)fcgiagent_info.req_count);
#endif	
	LOG_DEBUG("FcgiAgentHandler::deleteReqList seq=%u,cmd=%u,count=%u\n", 
		pReq->header.seq, pReq->header.cmd, fcgiagent_info.req_count);
}

FcgiAgentInfo* FcgiAgentHandler::getFcgiAgentByReq(ApiPool::Request& req)
{
	static uint32_t local_seq = 0;
	FcgiServiceInfos::iterator it = fcgiServiceInfos.find(req.getServiceName());
	if(it == fcgiServiceInfos.end())
	{
		LOG_ERROR("FcgiAgentHandler::getFcgiAgentByReq unknown service:%s",
					req.getServiceName().c_str());
		return NULL;
	}

	FcgiServiceInfo& fcgiServiceInfo = *it->second;

	for (size_t loopcount = 0; loopcount < MAX_SERVICE_WORKER_COUNT; loopcount++)
	{
		int index = (local_seq + loopcount)%MAX_SERVICE_WORKER_COUNT;
		if(fcgiServiceInfo.agentinfos[index].connected)
		{
			local_seq = index + 1;
			return &(fcgiServiceInfo.agentinfos[index]);
		}
	}
	LOG_ERROR("FcgiAgentHandler::getFcgiAgentByReq can't find connect for service:%s",
				req.getServiceName().c_str());
	return NULL;
}


FcgiAgentInfo* FcgiAgentHandler::getFcgiAgentByNetId(uint32_t net_id)
{
	Net_FcgiAgent_Map::iterator it = net_fcgiagent_map.find(net_id);
	if(it != net_fcgiagent_map.end())
	{
		return it->second;
	}

	LOG_ERROR("FcgiAgentHandler::getFcgiAgentByNetId can't find connect for net_id:%d",
				net_id);
	return NULL;	
}




int FcgiAgentHandler::handleRequest(Request* req)
{
	FcgiAgentInfo* agentinfo = getFcgiAgentByReq(*req);

	if(agentinfo == NULL)
	{
		LOG_ERROR("FcgiAgentHandler::handleRequest getFcgiAgentByReq fail for:%s",
					req->getServiceName().c_str());
		return RET_FAIL;
	}
	
	FcgiAgentRequest* fcgiReq = new FcgiAgentRequest(agentinfo);
	int ret = transfer_2_fcgi_proto(*req, *fcgiReq);
	if(ret != RET_OK)
	{
		LOG_ERROR("FcgiAgentHandler::handleRequest transfer_2_fcgi_proto fail ret:%d for:%s",
					ret, req->getServiceName().c_str());
		delete fcgiReq;
		return RET_FAIL;
	}

	fcgiReq->header.seq = ++agentinfo->curSeq;
	fcgiReq->header.cmd = CMD_FCGIAGENT_RPC;
	addToReqList(fcgiReq, *agentinfo);

    Net_Packet *net_packet = new Net_Packet;

	Binary_Write_Stream<FCGI_AGENT_HDR> wstream(net_packet->ptr(), net_packet->max_packet_length());
    memcpy(net_packet->ptr() + sizeof(FCGI_AGENT_HDR),
				fcgiReq->databuf, fcgiReq->databuf_len);

    wstream.head().len = htonl((uint32_t)wstream.length() + fcgiReq->databuf_len);
	wstream.head().cmd = htons(fcgiReq->header.cmd);
	wstream.head().ver = htons(fcgiReq->header.ver);
	wstream.head().seq = htonl(fcgiReq->header.seq);

    net_packet->length(wstream.length()+ fcgiReq->databuf_len);
	LOG_HEX_PREFIX("FcgiAgentHandler::handleRequest send req to agent", 
				net_packet->ptr(), net_packet->length(), utils::L_DEBUG);

	Net_Manager *net_manager = Dispatcher::getNetManager();
	int rc = net_manager->send_packet(agentinfo->net_id, net_packet);
	if (0 != rc) 
	{			
		//此处原本依赖checktimeout对pReq进行清理，但考虑到现网环境下,
		//netmanager繁忙时ERROR并不能及时被上层处理，故此处可能造成大量的
		//无效pReq存在队列中，因此将此处修改为send_packet出错立即删除，不依赖
		//checktimeout完成		
		deleteReqList(fcgiReq, *agentinfo);
		delete fcgiReq;
		delete net_packet;
		LOG_FATAL("FcgiAgentHandler::handleRequest error, send_packet error, net_id:%d", 
						agentinfo->net_id);
		return RET_NET_SEND_ERROR;
	}

	//记录req,依赖fcgiReq对req进行释放
	fcgiReq->request = req;
   	return RET_OK;	
}



void FcgiAgentHandler::makeBeginRequestBody(unsigned short  role,unsigned char  keepConnection,unsigned char * bin )
{
	role = htons(role);
	memcpy(bin,(void*)&role,sizeof(unsigned short));
	bin[2] = keepConnection;
	for(int i = 3;i < 8;++i)
	{
		bin[i] = 0;	
	}		
}

void FcgiAgentHandler::makeHeader(unsigned char type , unsigned short requestID, unsigned short contentLength, unsigned char paddingLength,unsigned char * header)
{
	header[0] = FCGI_VERSION_1;
	header[1] = type;
	requestID = htons(requestID);
	contentLength = htons(contentLength);
	memcpy(header+2,(void*)&requestID,sizeof(unsigned short));
	memcpy(header+4,(void*)&contentLength,sizeof(unsigned short));
	header[6] = paddingLength;
	header[7] = 0;
}
	
unsigned short FcgiAgentHandler::makeNameValueHeader(const string & name, const string & value ,unsigned char * tmpBuf)
{
	int nameLength = name.length();
	int valueLength = value.length();
	
	int index = 0;
	if(nameLength < 0x80)
	{
		tmpBuf[index++] = nameLength;
	}
	else
	{
		tmpBuf[index++] = ((valueLength >> 24) & 0x7f | 0x80);
		tmpBuf[index++] = ((valueLength >> 16) & 0xff);
		tmpBuf[index++] = ((valueLength >> 8) & 0xff);
		tmpBuf[index++] = (valueLength & 0xff);
	}

	if(valueLength < 0x80)
	{
		tmpBuf[index++] = valueLength; 
	}
	else
	{
		tmpBuf[index++] = ((valueLength >> 24) & 0x7f | 0x80);
		tmpBuf[index++] = ((valueLength >> 16) & 0xff);
		tmpBuf[index++] = ((valueLength >> 8) & 0xff);
		tmpBuf[index++] = (valueLength & 0xff);
	}

	memcpy(tmpBuf + index, name.c_str(),name.length());
	index += name.length();
	memcpy(tmpBuf + index, value.c_str(),value.length());
	index += value.length();
	return (unsigned short)index;
}	

int FcgiAgentHandler::makeFgiPack(const string & method, char  * buf , int buf_len, const map<string,string> & args,uint16_t recordID, uint16_t& lengh )
{
	static string rpcRoot = Options::apipoold.rpcRoot;
	int offset = 0;
	unsigned char body[8];
	unsigned short bodyLength =  0 ;
	unsigned char header[8];
	makeBeginRequestBody(FCGI_RESPONSE, 0,body);
	makeHeader(FCGI_BEGIN_REQUEST, recordID, sizeof(body), 0,header);
	if((offset + 8) > buf_len)
	{
		LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 1",
						(offset + 8), buf_len);
		return RET_INTERNAL_ERROR;
	}
	memcpy(buf+offset,header,8);	
	offset+=8;	
	if((offset + 8) > buf_len)
	{
		LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 2",
						(offset + 8), buf_len);
		return RET_INTERNAL_ERROR;
	}
	memcpy(buf+offset,body,8);	
	offset +=8;
	
	ArgsMap ar_pack;
	ar_pack["SCRIPT_FILENAME"] = rpcRoot;
	ar_pack["FRPC_METHOD"] = method;
	unsigned char nameValueBody[MAX_PHP_PACK_LEN];
	for(ArgsMap::iterator it = ar_pack.begin();it!=ar_pack.end();++it)
	{
		bodyLength = makeNameValueHeader(it->first,it->second,nameValueBody);
		makeHeader(FCGI_PARAMS,recordID,bodyLength,0,header);
		if((offset + 8) > buf_len)
		{
			LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 3",
							(offset + 8), buf_len);
			return RET_INTERNAL_ERROR;
		}
		memcpy(buf+offset,header,8);			
		offset +=8;			
		if((offset + bodyLength) > buf_len)
		{
			LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 4",
							(offset + bodyLength), buf_len);
			return RET_INTERNAL_ERROR;
		}
		memcpy(buf+offset,nameValueBody,bodyLength);			
		offset +=bodyLength;
	}
	
 	if(args.size() > 0)
	{
 		for(ArgsMap::const_iterator cit = args.begin();cit!= args.end();++cit)
		{
	    		bodyLength = makeNameValueHeader("FRPC_ARGS_"+cit->first , cit->second,nameValueBody);
	  	 	makeHeader(FCGI_PARAMS,recordID, bodyLength, 0,header);
			if((offset + 8) > buf_len)
			{
				LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 5",
								(offset + 8), buf_len);
				return RET_INTERNAL_ERROR;
			}
			memcpy(buf+offset,header,8);		
			offset +=8;			
			if((offset + bodyLength) > buf_len)
			{
				LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 6",
								(offset + bodyLength), buf_len);
				return RET_INTERNAL_ERROR;
			}
			memcpy(buf+offset,nameValueBody,bodyLength);			
			offset +=bodyLength;
		}
	}

      //结束符   
    makeHeader(FCGI_STDIN, recordID,0, 0,header);
	if((offset + 8) > buf_len)
	{
		LOG_ERROR("FcgiAgentHandler::makeFgiPack buffer need len:%d over exceed len:%d, step 6",
					  (offset + 8), buf_len);
		return RET_INTERNAL_ERROR;
	}
    memcpy(buf+offset,header,8);			
	offset +=8;
	lengh = offset;
	return RET_OK;
}


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
int FcgiAgentHandler::transfer_2_fcgi_proto(Request& req, FcgiAgentRequest& fcgiReq)
{
//		struct timeval begin;
//		struct timeval end;
//		gettimeofday(&begin,NULL);
	string method = req.getServiceName()+"."+req.getApiName();
	string requestSrc = req.getRequestSrc();
	ArgsMap args;
	args.insert(make_pair("requestSrc",requestSrc));
	vector<pair<string,string> >params = req.getParams();
	for(vector<pair<string,string> >::iterator it = params.begin();it!=params.end();++it)
	{
		args.insert(make_pair( (*it).first,(*it).second ) );
	}
	uint16_t recordID = 1;

	uint16_t length  = 0;
	int ret = makeFgiPack(method,fcgiReq.databuf, MAX_FCGI_REQ_LEN,
							args,recordID, 
							length);
	fcgiReq.databuf_len = length;
//		gettimeofday(&end,NULL);
//		unsigned long timespan = getTimeSpan(begin,end);
//		LOG_INFO("[STATMSG:FcgiAgentHandler::transfer_2_fcgi_proto, requestSrc:%s, serviceName:%s]\n",
//			requestSrc.c_str(),method.c_str());
	return ret;
}

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
int FcgiAgentHandler::transfer_from_fcgi_proto(Request& request, const char* fcgi_resp_buf, int fcgi_buf_len, Response& resq)
{
//		struct timeval begin;
//		struct timeval end;
//		gettimeofday(&begin,NULL);
	char content[MAX_PHP_PACK_LEN];
	int content_len = MAX_PHP_PACK_LEN;
	string result;
	int ret  = 0;
	int errorCode = 0;
	string errorInfo;
	
	string method = request.getServiceName()+"."+request.getApiName();
	string requestSrc = request.getRequestSrc();
	
	ret = parseData(fcgi_resp_buf+sizeof(FCGI_AGENT_HDR), 
				fcgi_buf_len - sizeof(FCGI_AGENT_HDR), 
				content, content_len);
	if(ret != RET_OK)
	{
		LOG_WARN("[transfer_from_fcgi_proto]parseData ret:%d, reqsrc:%s, orgin reqid:%s, reqsrv:%s",
				ret, request.getRequestSrc().c_str(), 
				request.getId().asString().c_str(),
				request.getServiceName().c_str());

		resq.setResult(Response::nullValue());
		resq.setError(ApiPool::ERR_STATUS_INTERNAL);
		resq.setErrorInfo(ApiPool::translateError(ApiPool::ERR_STATUS_INTERNAL));
	}
	else
	{
		string contentstr = content;
		ret = getresult(contentstr,result,errorCode, errorInfo);
		if(ret != RET_OK)//error:set error code in error and error info in the info.result  is null value.
		{
			LOG_WARN("[transfer_from_fcgi_proto] get Result error , ret:%d, error code = %d, error = %s, reqsrc:%s, orgin reqid:%s, reqsrv:%s",
							ret, errorCode, errorInfo.c_str(),
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());

			resq.setResult(Response::nullValue());
			resq.setError(errorCode);
			resq.setErrorInfo(errorInfo);
		}
		else//success, set result as the result of request. set error as null value.no extra info is required.
		{
			LOG_INFO("[transfer_from_fcgi_proto]get Result success, reqsrc:%s, orgin reqid:%s, reqsrv:%s\n",
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());
			resq.setResult(result);
			resq.setError(Response::nullValue());
		}
	}
	resq.setId(request.getId());
	return 0;
}

void FcgiAgentHandler::recvAgentHeader(FCGI_AGENT_HDR * hdr,const char * buf, int  offset)
{
	FCGI_AGENT_HDR * phdr = (FCGI_AGENT_HDR*)(buf + offset);
	hdr->len = ntohl(phdr->len);
	hdr->cmd = ntohs(phdr->cmd);
	hdr->ver = ntohs(phdr->ver);
	hdr->seq = ntohl(phdr->seq);
}

void FcgiAgentHandler::recvFcgiHeader(FCGI_HDR * hdr,const char * buf, int offset)
{
	FCGI_HDR * phdr = (FCGI_HDR*)(buf+offset);
	hdr->version = phdr->version;
	hdr->type = phdr->type;
	hdr->version = ntohs(phdr->requestID);
	hdr->contentLength = ntohs(phdr->contentLength);
	hdr->paddingLength = phdr->paddingLength;
	hdr->reserved = phdr->reserved;
}

/*
	@result:rpcserver return value
	@errorCode:error code 
	@errorInfo: error information
*/
int FcgiAgentHandler::analyzeResult(string & result,int & errorCode, string & errorInfo)
{
	/****result sample******
	if succ
	{"state":"200 success","entity":{"apperrcode":0,"errorinfo":"","appresdata":[],"debuginfo":""}}
	if fail
	// TODO ....
	**************************/
	string::size_type state_pos = result.find("state");
	if(state_pos == string::npos){
		errorInfo= result;
		errorCode = ApiPool::ERR_PROTOCOL_ERROR;
		LOG_ERROR("FcgiAgentHandler::analyzeResult parse fail can't find _state_.");
		return RET_PARSE_PARAM_ERROR;
	}
	
	string::size_type succ_pos = result.find("success",0);
	if(succ_pos != string::npos)
	{
		string::size_type entity_pos = result.find("entity");
		if(entity_pos == string::npos)
		{
			result = "success";
		}
		else
		{
			entity_pos+=strlen("entity")+2;
			result = result.substr(entity_pos,result.length()-1 - entity_pos);
		}
		
		return RET_OK;
	}
	else
	{
		string::size_type state_code_pos = state_pos + strlen("state") + 3;
		string::size_type state_code_end_pos = result.find(" ", state_code_pos);
		string errorno = result.substr(state_code_pos, state_code_end_pos - state_code_pos);
		string::size_type error_pos = result.find("error");
		error_pos +=strlen("error")+2;
		errorInfo = result.substr(error_pos,result.length()- error_pos -1);
		errorCode = atoi(errorno.c_str());
		//LOG_ERROR("FcgiAgentHandler::analyzeResult parse fail can't find _state_.");
//			return -1;

		return RET_MID_FAIL;
	}	
}

int FcgiAgentHandler::getresult(const string& content,string & result,int & errorCode, string & errorInfo)
{
	/****content sample******
	X-Powered-By:.PHP/5.2.13..Content-type:.text/html....{"state":"200.success","entity":{"apperrcode":0,"errorinfo":"","appresdata":[],"debuginfo":""}}
	**************************/

	string::size_type pos = content.find("\r\n\r\n",0);
	if(string::npos == pos)
	{
		errorCode = ApiPool::ERR_PROTOCOL_ERROR;
		errorInfo = ApiPool::translateError(ApiPool::ERR_PROTOCOL_ERROR);
		LOG_ERROR("FcgiAgentHandler::getresult parse fail can't find double return.");
		return RET_PARSE_PARAM_ERROR;
	}
	pos += 4;

	if(pos == content.length())
	{
		errorCode = ApiPool::ERR_PROTOCOL_ERROR;
		errorInfo = ApiPool::translateError(ApiPool::ERR_PROTOCOL_ERROR);
		LOG_ERROR("FcgiAgentHandler::getresult parse fail can't find content.");
		return RET_PARSE_PARAM_ERROR;
	}
	else
	{
		result = content.substr(pos);
		return analyzeResult(result,errorCode,errorInfo);
	}	
}

int  FcgiAgentHandler::parseData(const char * recvData, uint32_t len, char* content, int& content_len)
{
	FCGI_HDR fcgi_hdr;
//		char buf[MAX_PHP_PACK_LEN];
	int offset = 0;
	int fcgi_block_cnt = 0;
	int content_buff_len = content_len;
	content_len = 0;
	
	while(offset < len)
	{
		if(offset + sizeof(FCGI_HDR) > len)
		{
			LOG_ERROR("FcgiAgentHandler::parseData parse fail len:%d less than FCGI_HDR:%d, offset:%d, fcgi_block_cnt:%d",
						len, (int)sizeof(FCGI_HDR), offset, fcgi_block_cnt);
			return RET_PARSE_HEAD_ERROR;
		}
		recvFcgiHeader(&fcgi_hdr,recvData,offset);
		offset += sizeof(FCGI_HDR);

		if(offset + fcgi_hdr.contentLength > len)
		{
			LOG_ERROR("FcgiAgentHandler::parseData parse fail len:%d less than fcgi_hdr.contentLength:%d, offset:%d, fcgi_block_cnt:%d",
						len, fcgi_hdr.contentLength, offset, fcgi_block_cnt);
			return RET_PARSE_HEAD_ERROR;
		}

		if(fcgi_hdr.type == FCGI_STDOUT || fcgi_hdr.type == FCGI_STDERR)
		{
			// 仅在type == FCGI_STDOUT, FCGI_STDERR时的输出是对业务有意义的
			// 此处-1的原因是fcgi的content是不包括\0结束符的.
			if((offset + fcgi_hdr.contentLength) > content_buff_len - 1)
			{
				LOG_ERROR("FcgiAgentHandler::parseData len:%d exceed conten_buff_len:%d",
								(offset + fcgi_hdr.contentLength),
								content_buff_len);
				return RET_PARSE_PARAM_ERROR;
			}

			memcpy(content+content_len, recvData+offset,fcgi_hdr.contentLength);
			content_len += fcgi_hdr.contentLength;
			content[content_len] = 0;
		}
		else
		{
			LOG_INFO("FcgiAgentHandler::parseData discard fcgi block type:%d",
						fcgi_hdr.type);
		}
		
		offset += fcgi_hdr.contentLength;	//此处已经解析完毕content字段
		offset += fcgi_hdr.paddingLength;	//不需要解析padding字段

		fcgi_block_cnt++;	
	}
	return 0;
}



void FcgiAgentHandler::getFcgiAgentHandlerInformation(char* buf, int len)
{	
	stringstream logstr;
	logstr<<"***FcgiAgent info***"<<endl;
	FcgiServiceInfos::iterator it = fcgiServiceInfos.begin();

	for(; it != fcgiServiceInfos.end(); it++)
	{
		FcgiServiceInfo& fcgiServiceInfo = *it->second;
		logstr<<"ServiceName:"<<fcgiServiceInfo.serviceName
			  <<",worker_count:"<<fcgiServiceInfo.worker_count
			  <<", detail:::"<<endl;
		
		for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
		{
			if(fcgiServiceInfo.agentinfos[i].ip != "")
			{
			logstr<<"index:"<<i
				  <<",net_id:"<<fcgiServiceInfo.agentinfos[i].net_id
				<<",ip:"<<fcgiServiceInfo.agentinfos[i].ip
				<<",port:"<<fcgiServiceInfo.agentinfos[i].port
				<<",conneted:"<<fcgiServiceInfo.agentinfos[i].connected
				<<",reconnect_time:"<<fcgiServiceInfo.agentinfos[i].reconnect_time
				<<",keepalive_time:"<<fcgiServiceInfo.agentinfos[i].keepalive_time
				<<",last_active_time:"<<fcgiServiceInfo.agentinfos[i].last_active_time
				<<",req_count:"<<fcgiServiceInfo.agentinfos[i].req_count
				<<",curSeq:"<<fcgiServiceInfo.agentinfos[i].curSeq
				<<endl;
			}
		}		
	}
	strncpy(buf, logstr.str().c_str(), len);
	return;
}

 
