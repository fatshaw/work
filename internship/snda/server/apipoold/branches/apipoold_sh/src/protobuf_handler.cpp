#include <sstream>
#include "protobuf_handler.h"
#include "service_router.h"
#include "dispatcher.h"
#include "common.h"
#include "apipoold_protocol.h"
#include "options.h"
#include <json/json.h>
#include "binary_stream.h"
#include <string>
#include "tokenMessage.pb.h"

ProtobufServerServiceInfos ProtobufServerHandler::protobufServerServiceInfos;
Net_ProtobufServer_Map ProtobufServerHandler::net_ProtobufServer_Map;

#define STATESERVER_REQUEST_TYPE "StateServer::StateServerRequest" 

#ifndef DISABLE_MEMPOOL
#ifdef DEBUG
	IMPLEMENT_SLAB("ProtobufServerRequest",ProtobufServerRequest, 2)
#else
	IMPLEMENT_SLAB("ProtobufServerRequest",ProtobufServerRequest, 256)
#endif
#endif

/*
string default_svc = "Foo";
string defaultip = "127.0.0.1";
int defaultport = 9869;
*/

static inline int hashfn(uint32_t seq)
{
	return (seq & PROTOBUF_REQUEST_HASH_MASK);
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

int ProtobufServerHandler::reconfigure()
{
	int service_count = (uint32_t)Options::stateServer.service_list.size();

	LOG_INFO("ProtobufServerHandler::reconfigure, stateServer server number:%lu", (unsigned long)Options::stateServer.service_list.size());

	for (int i = 0; i <service_count; i++)
	{
		string servicename = Options::stateServer.service_list[i].serviceName;
		string ip = Options::stateServer.service_list[i].ip;
		int port = Options::stateServer.service_list[i].port;
		int index =Options::stateServer.service_list[i].index;
		ServcieInfo& s_info = Options::stateServer.service_list[i];
		ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.find(s_info.serviceName);
		if(it == protobufServerServiceInfos.end()){
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

int ProtobufServerHandler::updateService(string & servicename ,int index, string ip, int port)
{
	LOG_INFO("ProtobufServerHandler::update phpservice handler for Sname:%s, server:[%s:%d]", 
						servicename.c_str(), ip.c_str(), port);

	ProtobufServerServiceInfos::iterator it  = protobufServerServiceInfos.find(servicename);
	if(it == protobufServerServiceInfos.end()){
		LOG_DEBUG("UPDATE  Service: No Service Name : %s\n",servicename.c_str());
	}
	ProtobufServerServiceInfo * serviceInfo = it->second;
	if ((serviceInfo->protobufServerInfos[index].ip!= ip) || (serviceInfo->protobufServerInfos[index].port != port))
	{
		//服务器发生变化
		LOG_INFO("ProtobufServerHandler::update phpservice handler, server addr changed, oldip:%s, oldport:%d, newip:%s, newport:%d, reconnect",
				serviceInfo->protobufServerInfos[index].ip.c_str(), serviceInfo->protobufServerInfos[index].port, ip.c_str(), port);
		int ret = 0;
		if(serviceInfo->protobufServerInfos[index].port  != 0)
		{
			ret = disconnect(serviceInfo->protobufServerInfos[index]);
		}
		serviceInfo->protobufServerInfos[index].ip = ip;
		serviceInfo->protobufServerInfos[index].port = port;
		if(ret != 0){
			LOG_DEBUG("[update Service]disconnect error\n");
		}
		else{
			connect(serviceInfo->protobufServerInfos[index]);
		}
		return 0;
	}

	// 服务器没有发生变化
	LOG_INFO("ProtobufServerHandler handler::updateservice, server addr not changed, ignore");
	return 0;
}

void ProtobufServerHandler::allocate_new_service(string & serviceName)
{
	ProtobufServerServiceInfo* protobufServerServiceInfo = new ProtobufServerServiceInfo();
	protobufServerServiceInfo->serviceName = serviceName;	

	ServiceManager::registerServiceHandler(protobufServerServiceInfo->serviceName.c_str(),
						handleRequest);
	
	for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
	{	
		protobufServerServiceInfo->protobufServerInfos[i].net_id = 0;
		protobufServerServiceInfo->protobufServerInfos[i].serviceName = protobufServerServiceInfo->serviceName;;		
		protobufServerServiceInfo->protobufServerInfos[i].reconnect_time = 0;
		protobufServerServiceInfo->protobufServerInfos[i].keepalive_time = 0;
		protobufServerServiceInfo->protobufServerInfos[i].last_active_time = 0;

		protobufServerServiceInfo->protobufServerInfos[i].curSeq = 0;
		protobufServerServiceInfo->protobufServerInfos[i].req_count = 0;		

		INIT_LIST_HEAD(&protobufServerServiceInfo->protobufServerInfos[i].req_list);
		for(int j = 0; j < FCGIAGENT_REQUEST_HASH_SIZE; j++)
		{
			INIT_LIST_HEAD(&protobufServerServiceInfo->protobufServerInfos[i].req_hash[j]);
		}		
	}
	
	protobufServerServiceInfos.insert(make_pair(protobufServerServiceInfo->serviceName,protobufServerServiceInfo));
	
	LOG_INFO("ProtobufServerHandler::allocate_new_service, servicename:%s, cur service size:%d", 
							serviceName.c_str(), (int)protobufServerServiceInfos.size());

}

int ProtobufServerHandler::initializeService(ServcieInfo& s_info)
{
	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.find(s_info.serviceName);

	if(it == protobufServerServiceInfos.end())
	{
		allocate_new_service(s_info.serviceName);
	}

	if(s_info.index >= MAX_SERVICE_WORKER_COUNT)
	{
		LOG_ERROR("ProtobufServerHandler::initialize fail index num illegal, s_name:%s ip:%s port:%d index:%d",
							s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index);		
		return RET_FAIL;
	}

	ProtobufServerServiceInfo *protobufServerServiceInfo = protobufServerServiceInfos[s_info.serviceName];	
	if(protobufServerServiceInfo->protobufServerInfos[s_info.index].ip
			!= "")
	{
		LOG_ERROR("ProtobufServerHandler::initialize fail index duplicated, s_name:%s ip:%s port:%d index:%d",
							s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index);	
		return RET_FAIL;
	}
	protobufServerServiceInfo->protobufServerInfos[s_info.index].ip = s_info.ip;
	protobufServerServiceInfo->protobufServerInfos[s_info.index].port = s_info.port;
	protobufServerServiceInfo->worker_count++;

	LOG_INFO("ProtobufServerHandler::initialize succ index, s_name:%s ip:%s port:%d index:%d count:%d",
					s_info.serviceName.c_str(), s_info.ip.c_str(), s_info.port, s_info.index, protobufServerServiceInfo->worker_count);	

	return RET_OK;

}

	

int ProtobufServerHandler::initialize()
{
	for(int i = 0; i<(int)Options::stateServer.service_list.size(); i++)
	{
		ServcieInfo& s_info = Options::stateServer.service_list[i];
		initializeService(s_info);
	}

	return 0;
}

int ProtobufServerHandler::disconnect()
{
	LOG_INFO("ProtobufServerHandler::disconnect, disconnect all fcgiagent begin");

	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.begin();

	for(; it!= protobufServerServiceInfos.end(); it++)
	{
		string serviceName = it->first;
		int ret = disconnect(serviceName);
		if (ret != RET_OK) 
		{
			LOG_WARN("ProtobufServerHandler::disconnect fail for sname:%s",
					it->first.c_str());
		}
	}

	LOG_INFO("ProtobufServerHandler::disconnect, disconnect all fcgiagent end");
	return RET_OK;
}

int ProtobufServerHandler::disconnect(string& serviceName)
{
	LOG_INFO("ProtobufServerHandler::disconnect, disconnect all fcgiagent begin");

	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.find(serviceName);
	if(it == protobufServerServiceInfos.end())
	{
		LOG_ERROR("ProtobufServerHandler::disconnect unknown service:%s",
					serviceName.c_str());
		return RET_FAIL;
	}

	ProtobufServerServiceInfo& protobufServerServiceInfo = *it->second;

	int rc = 0;

	for (size_t i = 0; i < protobufServerServiceInfo.worker_count; i++)
	{
		int r = disconnect(protobufServerServiceInfo.protobufServerInfos[i]);
		if (0 != r) {
			rc = -1;
		}
	}

	LOG_INFO("ProtobufServerHandler::disconnect, disconnect all protobufServer end");
	return rc;
}

int ProtobufServerHandler::disconnect(ProtobufServerInfo& protobufServerInfo)
{
	LOG_INFO("ProtobufServerHandler::disconnect begin, net_id:%d, server:[%s:%d]", protobufServerInfo.net_id, protobufServerInfo.ip.c_str(), protobufServerInfo.port);

	if (0 == protobufServerInfo.net_id) {		// 没有连接
		return 0;
	}
	clearProtobufServerStateinfo(protobufServerInfo);

	Dispatcher::getNetManager()->delete_net(protobufServerInfo.net_id);
	net_ProtobufServer_Map.erase(protobufServerInfo.net_id);

	protobufServerInfo.net_id = 0;
	protobufServerInfo.connected = false;
	protobufServerInfo.reconnect_time = 0;
	protobufServerInfo.keepalive_time = 0;
	protobufServerInfo.last_active_time = 0;

	LOG_INFO("ProtobufServerHandler::disconnect ok");
	
	return 0;
}

void ProtobufServerHandler::sendKeepAlive(ProtobufServerInfo& protobufServerInfo)
{
	LOG_DEBUG("ProtobufServerHandler::sendKeepAlive, net_id:%d", protobufServerInfo.net_id);

	if (!protobufServerInfo.connected) 
	{
		LOG_FATAL("ProtobufServerHandler::sendKeepAlive error, not connect");
		return;
	}

	
	char buf[1024];
	int length = 0;
	length=encodeKeepAlivePack(buf);

	Net_Packet *net_packet = new Net_Packet;
	int totalLength = makeMessagePacket(STATESERVER_REQUEST_TYPE,length,buf,net_packet);

	net_packet->length(totalLength);
	LOG_HEX_PREFIX("ProtobufServerHandler::sendKeepAlive :", 
				net_packet->ptr(), net_packet->length(), utils::L_DEBUG);
	Net_Manager *net_manager = Dispatcher::getNetManager();
	int rc = net_manager->send_packet(protobufServerInfo.net_id, net_packet);
	if (0 != rc) 
	{
		delete net_packet;
		LOG_FATAL("ProtobufServerHandler::sendKeepAlive error, send_packet error, net_id:%d", protobufServerInfo.net_id);
		return;
	}

	protobufServerInfo.keepalive_time = time(NULL) + PROTOBUF_KEEPALIVE_INTERVAL;
	return;
	
}

void ProtobufServerHandler::onRecvKeepAlive(ProtobufServerInfo *protobufServerInfo)
{
	LOG_DEBUG("ProtobufServerHandler::onRecvKeepAlive, ip:%s, port:%d", 
		protobufServerInfo->ip.c_str(), protobufServerInfo->port);
}

void ProtobufServerHandler::onConnectOk(ProtobufServerInfo &protobufServerInfo, Net_Event& net_event)
{
	LOG_INFO("ProtobufServerHandler::onConnectOk, net_id:%d, ip:%s, port:%d", protobufServerInfo.net_id,
		protobufServerInfo.ip.c_str(), protobufServerInfo.port);

	protobufServerInfo.connected = true;
	protobufServerInfo.reconnect_time = 0;
	protobufServerInfo.keepalive_time = time(NULL) + FCGIAGENT_KEEPALIVE_INTERVAL;
}

void ProtobufServerHandler::addToReqList(ProtobufServerRequest* pReq, ProtobufServerInfo & protobufServerInfo)
{
	int i = hashfn(pReq->header.seq);
	list_add_tail(&pReq->hash, &protobufServerInfo.req_hash[i]);
	list_add_tail(&pReq->lst, &protobufServerInfo.req_list);
	++protobufServerInfo.req_count;

#ifdef LIST_TEST
		list_head* pos = NULL;
		int realcount = 0;
		list_for_each(pos, &protobufServerInfo.req_list)
		{
			realcount++;
		}	
		assert(realcount == (int)protobufServerInfo.req_count);
#endif	
	LOG_DEBUG("ProtobufServerHandler::addToReqList seq=%u,cmd=%u,count=%u\n", 
		pReq->header.seq, pReq->header.cmd, protobufServerInfo.req_count);
}


ProtobufServerRequest* ProtobufServerHandler::findReqList(uint32_t seq, ProtobufServerInfo & protobufServerInfo)
{
	int i = hashfn(seq);
	list_head* pos;
	ProtobufServerRequest* protobufServerRequest;
	list_for_each(pos, &protobufServerInfo.req_hash[i])
	{
		protobufServerRequest = list_entry(pos, ProtobufServerRequest, hash);
//	#ifdef LIST_TEST
//			assert(pos->next == &fcgiagent_info.req_hash[i]
//					&& pos->prev == &fcgiagent_info.req_hash[i]);
	//hash complict
//	#endif
		if(protobufServerRequest->header.seq == seq)	
			return protobufServerRequest;
	}
	return NULL;
}

void ProtobufServerHandler::deleteReqList(ProtobufServerRequest* pReq,ProtobufServerInfo & protobufServerInfo)
{
	list_del(&pReq->hash);
	list_del(&pReq->lst);
	--protobufServerInfo.req_count;
	
#ifdef LIST_TEST
		list_head* pos = NULL;
		int realcount = 0;
		list_for_each(pos, &protobufServerInfo.req_list)
		{
			realcount++;
		}	
		assert(realcount == (int)protobufServerInfo.req_count);
#endif	
	LOG_DEBUG("ProtobufServerHandler::deleteReqList seq=%u,count=%u\n", 
		pReq->header.seq, protobufServerInfo.req_count);
}

ProtobufServerInfo* ProtobufServerHandler::getProtobufServerByReq(ApiPool::Request& req)
{
	static uint32_t local_seq = 0;
	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.find(req.getServiceName());
	if(it == protobufServerServiceInfos.end())
	{
		LOG_ERROR("ProtobufServerHandler::getProtobufServerByReq unknown service:%s",
					req.getServiceName().c_str());
		return NULL;
	}

	ProtobufServerServiceInfo& protobufServerServiceInfo = *it->second;

	for (size_t loopcount = 0; loopcount < MAX_SERVICE_WORKER_COUNT; loopcount++)
	{
		int index = (local_seq + loopcount)%MAX_SERVICE_WORKER_COUNT;
		if(protobufServerServiceInfo.protobufServerInfos[index].connected)
		{
			local_seq = index + 1;
			return &(protobufServerServiceInfo.protobufServerInfos[index]);
		}
	}
	LOG_ERROR("ProtobufServerHandler::getProtobufServerByReq can't find connect for service:%s",
				req.getServiceName().c_str());
	return NULL;
}

ProtobufServerInfo* ProtobufServerHandler::getProtobufServerByNetId(uint32_t net_id)
{
	Net_ProtobufServer_Map::iterator it = net_ProtobufServer_Map.find(net_id);
	if(it != net_ProtobufServer_Map.end())
	{
		return it->second;
	}

	LOG_ERROR("ProtobufServerHandler::getProtobufServerByNetId can't find connect for net_id:%d",
				net_id);
	return NULL;	
}

int ProtobufServerHandler::handleRequest(Request* req)
{
	ProtobufServerInfo* protobufServerInfo = getProtobufServerByReq(*req);

	if(protobufServerInfo == NULL)
	{
		LOG_ERROR("ProtobufServerHandler::handleRequest getProtobufServerByReq fail for:%s",
					req->getServiceName().c_str());
		return RET_FAIL;
	}
	
	ProtobufServerRequest* protobufServerRequest = new ProtobufServerRequest(protobufServerInfo);
	protobufServerRequest->header.seq = ++protobufServerInfo->curSeq;
	protobufServerRequest->header.cmd = STATE_REQUEST;
	int ret = encodeProtobuf(*req, *protobufServerRequest);
	if(ret != RET_OK)
	{
		LOG_ERROR("ProtobufServerHandler::handleRequest encodeProtobuf fail ret:%d for:%s",
					ret, req->getServiceName().c_str());
		delete protobufServerRequest;
		--protobufServerInfo->curSeq;
		return RET_FAIL;
	}

	addToReqList(protobufServerRequest, *protobufServerInfo);

    	Net_Packet *net_packet = new Net_Packet;
		//package for framework

	size_t totalLength= sizeof(uint32_t) *3 + strlen(STATESERVER_REQUEST_TYPE) + protobufServerRequest->databuf_len;
	if((int)totalLength > net_packet->max_packet_length()){
		LOG_WARN("[ProtobufServerHandler::handleRequest packet length:%d exceed max packet length%d\n",totalLength,net_packet->max_packet_length());
		delete net_packet;
		net_packet = new Net_Packet(totalLength);
	}

	makeMessagePacket(STATESERVER_REQUEST_TYPE,protobufServerRequest->databuf_len,protobufServerRequest->databuf,net_packet);
	
	net_packet->length(totalLength);
	LOG_HEX_PREFIX("ProtobufServerHandler::handleRequest send req to agent", 
				net_packet->ptr(), net_packet->length(), utils::L_DEBUG);

	Net_Manager *net_manager = Dispatcher::getNetManager();
	int rc = net_manager->send_packet(protobufServerInfo->net_id, net_packet);
	if (0 != rc) 
	{			
		//此处原本依赖checktimeout对pReq进行清理，但考虑到现网环境下,
		//netmanager繁忙时ERROR并不能及时被上层处理，故此处可能造成大量的
		//无效pReq存在队列中，因此将此处修改为send_packet出错立即删除，不依赖
		//checktimeout完成		
		deleteReqList(protobufServerRequest, *protobufServerInfo);
		delete protobufServerRequest;
		delete net_packet;
		LOG_FATAL("ProtobufServerHandler::handleRequest error, send_packet error, net_id:%d", 
						protobufServerInfo->net_id);
		return RET_NET_SEND_ERROR;
	}

	//记录req,依赖fcgiReq对req进行释放
	protobufServerRequest->request = req;
   	return RET_OK;	
}


int ProtobufServerHandler::makeMessagePacket(string  typeName, uint32_t protobufLength,  char * protobuf,Net_Packet *  net_packet)
{
	uint32_t strLength = typeName.length();
	uint32_t totalLength= sizeof(uint32_t) *3 + strLength + protobufLength;

	if((int)totalLength > net_packet->max_packet_length()){
		LOG_WARN("[ProtobufServerHandler::handleRequest packet length:%d exceed max packet length%d\n",totalLength,net_packet->max_packet_length());
		return -1;
	}

	int offset  = 0 ;
	int n_totalLength = htonl(totalLength);
	memcpy(net_packet->ptr()+offset,(void*)&n_totalLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	int n_strLength = htonl(strLength);
	memcpy(net_packet->ptr()+offset,(void*)&n_strLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(net_packet->ptr() + offset ,(void*)typeName.c_str(),strLength);
	offset+= strLength;
	int n_protobuflength = htonl(protobufLength);
	memcpy(net_packet->ptr() + offset ,(void*)&n_protobuflength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(net_packet->ptr() + offset ,(void*)protobuf,protobufLength);
	offset+= protobufLength;
	return totalLength;
}

int ProtobufServerHandler::encodeProtobuf(ApiPool::Request& req, ProtobufServerRequest& protobufServerRequest)
{
	StateServer::TokenMessage tokenMessage;			
	tokenMessage.set_seq(protobufServerRequest.header.seq);
	tokenMessage.set_apiname(req.getApiName());
	tokenMessage.set_cmd(protobufServerRequest.header.cmd);
	ApiPool::Request::ParamList params = req.getParams();			
	for(ApiPool::Request::ParamList::iterator it = params.begin();it!=params.end();++it) 		
	{					
		StateServer::TokenMessage_Paramlist * param = tokenMessage.add_params(); 				
		param->set_key((*it).first);					
		param->set_value((*it).second); 		
	}					
	if(tokenMessage.ByteSize() > MAX_PROTOBUF_REQ_LEN)
	{
		LOG_WARN("[ProtobufServerHandler:encodeProtobuf] tokenMessage length =%d exceed MAX_PROTOBUF_REQ_LEN=%d\n",tokenMessage.ByteSize(),MAX_PROTOBUF_REQ_LEN);
		return -1;
	}
	tokenMessage.SerializeToArray(protobufServerRequest.databuf,tokenMessage.ByteSize());
	protobufServerRequest.databuf_len = tokenMessage.ByteSize();
	return 0;
}

int ProtobufServerHandler::encodeKeepAlivePack(char * buf)
{
	StateServer::TokenMessage tokenMessage;			
	tokenMessage.set_seq(0);
	tokenMessage.set_apiname("KEEP_ALIVE");
	tokenMessage.set_cmd(STATE_KEEP_ALIVE);
	tokenMessage.SerializeToArray(buf,tokenMessage.ByteSize());
	return tokenMessage.ByteSize();
}

void ProtobufServerHandler::getProtobufServerHandlerInformation(char* buf, int len)
{	
	stringstream logstr;
	logstr<<"***protobufserver info***"<<endl;
	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.begin();

	for(; it != protobufServerServiceInfos.end(); it++)
	{
		ProtobufServerServiceInfo& protobufServerServiceInfo  = *it->second;
		logstr<<"ServiceName:"<<protobufServerServiceInfo.serviceName
			  <<",worker_count:"<<protobufServerServiceInfo.worker_count
			  <<", detail:::"<<endl;
		
		for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
		{
			if(protobufServerServiceInfo.protobufServerInfos[i].ip != "")
			{
			logstr<<"index:"<<i
				  <<",net_id:"<<protobufServerServiceInfo.protobufServerInfos[i].net_id
				<<",ip:"<<protobufServerServiceInfo.protobufServerInfos[i].ip
				<<",port:"<<protobufServerServiceInfo.protobufServerInfos[i].port
				<<",conneted:"<<protobufServerServiceInfo.protobufServerInfos[i].connected
				<<",reconnect_time:"<<protobufServerServiceInfo.protobufServerInfos[i].reconnect_time
				<<",keepalive_time:"<<protobufServerServiceInfo.protobufServerInfos[i].keepalive_time
				<<",last_active_time:"<<protobufServerServiceInfo.protobufServerInfos[i].last_active_time
				<<",req_count:"<<protobufServerServiceInfo.protobufServerInfos[i].req_count
				<<",curSeq:"<<protobufServerServiceInfo.protobufServerInfos[i].curSeq
				<<endl;
			}
		}		
	}
	strncpy(buf, logstr.str().c_str(), len);
	return;
}

 
