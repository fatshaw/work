#include "phpservice_handler.h"
#include "service_router.h"
#include "dispatcher.h"
#include "common.h"
#include "apipoold_protocol.h"
#include "apipoold_handler.h"
#include "protobuf_handler.h"
#include <json/json.h>
#include "log.h"
#include "binary_stream.h"

#include <string>

Binary_Splitter ProtobufServerHandler::m_packet_splitter;


void ProtobufServerHandler::NetEventHandler(Net_Event& net_event)
{
	uint32_t net_id = net_event.id;
    	ProtobufServerInfo* p_protobufServerInfo = getProtobufServerByNetId(net_id);
	if (p_protobufServerInfo == NULL)
	{
		LOG_FATAL("ProtobufServerHandler::NetEventHandler error,  protobufServerInfo not exist, net_id:%d", net_id);
		return;
	}
	p_protobufServerInfo->last_active_time = time(NULL);

	switch (net_event.net_event_type)
	{
	case TYPE_CONNECT:
		onConnectOk(*p_protobufServerInfo, net_event);
		break;

	case TYPE_TIMEOUT:
	case TYPE_EXCEPTION:
	case TYPE_CLOSE:
		onConnectError(*p_protobufServerInfo, net_event);
		break;

	case TYPE_DATA:
		onNetData(*p_protobufServerInfo, net_event);
		break;

	default:
		LOG(0)("ProtobufServerHandler::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
		break;
	}
}

void ProtobufServerHandler::TimerTickHandler(time_t current_time)
{	
	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.begin();
	for(; it != protobufServerServiceInfos.end(); it++)
	{		
		ProtobufServerServiceInfo& protobufServerServiceInfo = *it->second;
		
		for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
		{
			ProtobufServerInfo& protobufServerInfo = protobufServerServiceInfo.protobufServerInfos[i];
			if(protobufServerInfo.ip
					== "")
			{
				continue;
			}
			// 处理连接状态
			if ((0 != protobufServerInfo.reconnect_time) && (current_time >= protobufServerInfo.reconnect_time))
			{
				connect(protobufServerInfo);
			}
			
			if(protobufServerInfo.connected)
			{
				checkTimeout(current_time, protobufServerInfo);
			}
			
			if ((0 != protobufServerInfo.keepalive_time) && (current_time >= protobufServerInfo.keepalive_time))
			{
				sendKeepAlive(protobufServerInfo);
			}		
			
			if((0 != protobufServerInfo.last_active_time) 
				&& (current_time - protobufServerInfo.last_active_time) > 3*PROTOBUF_KEEPALIVE_INTERVAL)
			{
				LOG_WARN("ProtobufServerHandler::TimerTickHandler found last_active_time timeout try reconnect protobufServerInfo:[%s:%d]",
							 protobufServerInfo.ip.c_str(), protobufServerInfo.port);
				disconnect(protobufServerInfo);
				connect(protobufServerInfo);			
			}

		}
	}
}

int ProtobufServerHandler::connect()
{
	LOG_INFO("ProtobufServerHandler::connect, connect all protobufServer begin protobufServerServiceInfos size:%d",
					(int)protobufServerServiceInfos.size());

	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.begin();

	for(; it!= protobufServerServiceInfos.end(); it++)
	{
		string serviceName = it->first;
		int ret = connect(serviceName);
		if (ret != RET_OK) 
		{
			LOG_WARN("ProtobufServerHandler::connect fail for sname:%s",
					it->first.c_str());
		}
	}

	LOG_INFO("ProtobufServerHandler::connect, connect all protobufServer end");
	return RET_OK;
}

int ProtobufServerHandler::connect(string& serviceName)
{
	LOG_INFO("ProtobufServerHandler::connect, connect all protobufServer for service:%s",
				serviceName.c_str());

	ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.find(serviceName);
	if(it == protobufServerServiceInfos.end())
	{
		LOG_ERROR("ProtobufServerHandler::connect unknown service:%s",
					serviceName.c_str());
		return RET_FAIL;
	}

	ProtobufServerServiceInfo& stateServerServiceInfo = *it->second;

	int rc = 0;

	for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
	{
		if(stateServerServiceInfo.protobufServerInfos[i].ip
				!= "")
		{
			int r = connect(stateServerServiceInfo.protobufServerInfos[i]);
			if (0 != r) {
				rc = -1;
			}
		}
	}

	LOG_INFO("ProtobufServerHandler::connect, connect all protobufServer end for s_name:%s, count:%d",
					serviceName.c_str(), stateServerServiceInfo.worker_count);
	return rc;

}


int ProtobufServerHandler::connect(ProtobufServerInfo& protobufServerInfo)
{
	LOG_INFO("ProtobufServerHandler::connect, connect to [%s:%d] begin", 
					protobufServerInfo.ip.c_str(), protobufServerInfo.port);

	if (protobufServerInfo.connected) {	// 已经连接成功
		return 0;
	}
	if (0 != protobufServerInfo.net_id) {			// 正在连接
		return 0;
	}

	Net_Manager *net_manager = Dispatcher::getNetManager();
	protobufServerInfo.net_id = net_manager->create_tcp_client(protobufServerInfo.ip.c_str(), protobufServerInfo.port, &m_packet_splitter, 
				NetEventHandler, PROTOBUF_CONNECT_TIMEOUT);
	if (0 == protobufServerInfo.net_id) {			// 创建网络通道失败
		protobufServerInfo.reconnect_time = time(NULL) + PROTOBUF_CONNECT_TIMEOUT;	// 设置重新连接时间
		LOG_FATAL("ProtobufServerHandler::connect error, create_tcp_client error");
		return -1;
	}
	net_ProtobufServer_Map[protobufServerInfo.net_id] = &protobufServerInfo;

	protobufServerInfo.reconnect_time = 0;
	protobufServerInfo.keepalive_time = 0;
	protobufServerInfo.last_active_time = 0;

	LOG_INFO("ProtobufServerHandler::connect ok, net_id:%d", protobufServerInfo.net_id);
	return 0;
}

void ProtobufServerHandler::clearProtobufServerStateinfo(ProtobufServerInfo& protobufServerInfo)
{
	list_head* pos, *next;
	ProtobufServerRequest* req;
	LOG_ERROR("ProtobufServerInfo %s:%u clearProtobufServerStateinfo\n", 
		protobufServerInfo.ip.c_str(), protobufServerInfo.port);			

	list_for_each_safe(pos, next, &protobufServerInfo.req_list)
	{
		req = list_entry(pos, ProtobufServerRequest, lst);

		LOG_ERROR("protobufServerInfo %s:%u clear_req, cmd=%u seq=%u\n", 
			protobufServerInfo.ip.c_str(), protobufServerInfo.port, req->header.cmd, req->header.seq);			
	
		
		deleteReqList(req, protobufServerInfo);
		delete req;		
	}
	
	INIT_LIST_HEAD(&protobufServerInfo.req_list);
	for(int j = 0; j < PROTOBUF_REQUEST_HASH_SIZE; j++)
		INIT_LIST_HEAD(&protobufServerInfo.req_hash[j]);
	
 	protobufServerInfo.curSeq = 0;
	protobufServerInfo.req_count = 0;
	return;
}


void ProtobufServerHandler::onConnectError(ProtobufServerInfo& protobufServerInfo, Net_Event& net_event)
{
	LOG_FATAL("ProtobufServerHandler::onConnectError, net_id:%d, ip:%s, port:%d", protobufServerInfo.net_id,
		protobufServerInfo.ip.c_str(), protobufServerInfo.port);
		clearProtobufServerStateinfo(protobufServerInfo);
	//	连接断开或keepalive超时时调用,清除上层的一些暂存状态

	net_ProtobufServer_Map.erase(protobufServerInfo.net_id);

	protobufServerInfo.net_id = 0;
	protobufServerInfo.connected = false;
	protobufServerInfo.reconnect_time = time(NULL) + PROTOBUF_RECONNECT_INTERVAL;
	protobufServerInfo.keepalive_time = 0;
	protobufServerInfo.last_active_time = 0;
}

void ProtobufServerHandler::stop()
{
	LOG_DEBUG("ProtobufServerHandler Stop\n");
	for(ProtobufServerServiceInfos::iterator it = protobufServerServiceInfos.begin();it!=protobufServerServiceInfos.end();++it)
	{
		ProtobufServerServiceInfo * temp = it->second;
		LOG_DEBUG("delete ProtobufServerServiceInfo: Service name = %s\n",temp->serviceName.c_str());
		delete temp;
	}
}

void ProtobufServerHandler::checkTimeout(time_t current_time, ProtobufServerInfo & protobufServerInfo)
{
	LOG_DEBUG("ProtobufServerHandler::checkTimeout ");
	list_head* pos, *next;
	ProtobufServerRequest* req;

	time_t now = current_time;
	list_for_each_safe(pos, next, &protobufServerInfo.req_list)
	{
		req = list_entry(pos, ProtobufServerRequest, lst);
		if(now < req->timeout)
			break;
		
		LOG_ERROR("ProtobufServerHandler %s:%u request timeout, cmd=%u seq=%u\n", 
			protobufServerInfo.ip.c_str(), protobufServerInfo.port, req->header.cmd, req->header.seq);
		
		req->status = REQ_TIMEOUT;
		ApipooldHandler::sendCustomResponse(ApiPool::ERR_REQUEST_TIMEOUT,*(req->request),ApiPool::translateError(ApiPool::ERR_REQUEST_TIMEOUT));
		deleteReqList(req, protobufServerInfo);
		delete req;
	}
}

void ProtobufServerHandler::onNetData(ProtobufServerInfo& protobufServerInfo, Net_Event& net_event)
{
	Net_Packet& net_packet = net_event.packet;

	if (net_packet.length() < (int)sizeof(uint32_t))
	{
		LOG_FATAL("ProtobufServerHandler::onNetData error, packet length error, length:%d", net_packet.length());
		return;
	}
	LOG_HEX_PREFIX("ProtobufServerHandler::onNetData msg:", 
				net_packet.ptr(), net_packet.length(), utils::L_DEBUG);
	ProtobufServerRequest* pReq = NULL;
	int offset = 0;
	 uint32_t packet_length = 0;
	 uint32_t strLength = 0;
	 uint32_t protocolLength   = 0;
	memcpy((void*)&packet_length, net_packet.ptr(),sizeof(uint32_t));
	packet_length = ntohl(packet_length);
	offset += sizeof(uint32_t);
	memcpy((void*)&strLength, net_packet.ptr()+offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	strLength= ntohl(strLength);
	string request_typename(net_packet.ptr()+offset,strLength);
	offset += strLength;
	memcpy((void*)&protocolLength, net_packet.ptr()+offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	protocolLength = ntohl(protocolLength);
	StateServer::TokenMessage tokenMessage;
	tokenMessage.ParsePartialFromArray(net_packet.ptr()+offset,protocolLength);

	uint32_t cmd = tokenMessage.cmd();
	uint32_t seq = tokenMessage.seq();
	LOG_DEBUG("ProtobufServerHandler::onNetData cmd:%u seq:%d len:%d from servicename:%s, ip:%s, port:%d", 
				cmd,seq,packet_length,
				protobufServerInfo.serviceName.c_str(),
				protobufServerInfo.ip.c_str(), protobufServerInfo.port);
	
	if (cmd == STATE_KEEP_ALIVE)
	{
		onRecvKeepAlive(&protobufServerInfo);
	}
	else if(cmd == STATE_RESPONSE)
	{
		LOG_HEX_PREFIX("protobufServerInfo resp msg:", net_packet.ptr(), net_packet.length(), utils::L_DEBUG);
		pReq = findReqList(seq, protobufServerInfo);
		if(pReq)
		{			
			pReq->status = REQ_SUCC;
			switch(cmd)
			{
				case STATE_RESPONSE:
					handleResponse(pReq,tokenMessage);
					break;
				default:
					break;
			}
		}		
		else
		{
			LOG_WARN("ProtobufServerHandler::onNetData can't find ProtobufServerRequest cmd:%u seq:%d, srvname:%s, ip:%s, port:%d", 
							cmd, seq,
							protobufServerInfo.serviceName.c_str(),
							protobufServerInfo.ip.c_str(), protobufServerInfo.port);	
		}
		
	}
	else
	{
		LOG_WARN("ProtobufServerHandler::onNetData can't find cmd:%u seq:%d, srvname:%s, ip:%s, port:%d", 
						cmd, seq,
						protobufServerInfo.serviceName.c_str(),
						protobufServerInfo.ip.c_str(), protobufServerInfo.port);
	}
	if(pReq != NULL)
	{
		deleteReqList(pReq, protobufServerInfo);
		delete pReq;
	}
	return;
}

void ProtobufServerHandler::handleResponse(ProtobufServerRequest* pReq, StateServer::TokenMessage & tokenMessage)
{	
	Response resp;
	if(tokenMessage.params_size()> 0 )
	{
		resp.setResult(tokenMessage.params(0).value());
	}
	else
	{
		resp.setResult(Json::Value());
	}
	resp.setError(Response::nullValue());
	resp.setId(pReq->request->getId());
	ApipooldHandler::sendResponse(*pReq->request,resp);
	return;	
}





