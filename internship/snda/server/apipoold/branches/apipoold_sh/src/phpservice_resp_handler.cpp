#include "phpservice_handler.h"
#include "service_router.h"
#include "dispatcher.h"
#include "common.h"
#include "apipoold_protocol.h"
#include "apipoold_handler.h"

#include <json/json.h>

#include "binary_stream.h"

#include <string>

Binary_Splitter FcgiAgentHandler::m_packet_splitter;


void FcgiAgentHandler::NetEventHandler(Net_Event& net_event)
{
	uint32_t net_id = net_event.id;
    FcgiAgentInfo* p_fcgiagent_info = getFcgiAgentByNetId(net_id);
	if (p_fcgiagent_info == NULL)
	{
		LOG_FATAL("FcgiAgentHandler::NetEventHandler error, fcgiagent not exist, net_id:%d", net_id);
		return;
	}
	p_fcgiagent_info->last_active_time = time(NULL);

	switch (net_event.net_event_type)
	{
	case TYPE_CONNECT:
		onConnectOk(*p_fcgiagent_info, net_event);
		break;

	case TYPE_TIMEOUT:
	case TYPE_EXCEPTION:
	case TYPE_CLOSE:
		onConnectError(*p_fcgiagent_info, net_event);
		break;

	case TYPE_DATA:
		onNetData(*p_fcgiagent_info, net_event);
		break;

	default:
		LOG(0)("FcgiAgentHandler::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
		break;
	}
}

void FcgiAgentHandler::TimerTickHandler(time_t current_time)
{	
	FcgiServiceInfos::iterator it = fcgiServiceInfos.begin();
	for(; it != fcgiServiceInfos.end(); it++)
	{		
		FcgiServiceInfo& fcgiServiceInfo = *it->second;
		
		for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
		{
			FcgiAgentInfo& fcgiagent_info = fcgiServiceInfo.agentinfos[i];
			if(fcgiagent_info.ip
					== "")
			{
				continue;
			}
			// 处理连接状态
			if ((0 != fcgiagent_info.reconnect_time) && (current_time >= fcgiagent_info.reconnect_time))
			{
				connect(fcgiagent_info);
			}
			
			if(fcgiagent_info.connected)
			{
				checkTimeout(current_time, fcgiagent_info);
			}
			
			if ((0 != fcgiagent_info.keepalive_time) && (current_time >= fcgiagent_info.keepalive_time))
			{
				sendKeepAlive(fcgiagent_info);
			}		
			
			if((0 != fcgiagent_info.last_active_time) 
				&& (current_time - fcgiagent_info.last_active_time) > 3*FCGIAGENT_KEEPALIVE_INTERVAL)
			{
				LOG_WARN("FcgiAgentHandler::TimerTickHandler found last_active_time timeout try reconnect fcgiagent_info:[%s:%d]",
							 fcgiagent_info.ip.c_str(), fcgiagent_info.port);
				disconnect(fcgiagent_info);
				connect(fcgiagent_info);			
			}

		}
	}
}

int FcgiAgentHandler::connect()
{
	LOG_INFO("FcgiAgentHandler::connect, connect all fcgiagent begin fcgiServiceInfos size:%d",
					(int)fcgiServiceInfos.size());

	FcgiServiceInfos::iterator it = fcgiServiceInfos.begin();

	for(; it!= fcgiServiceInfos.end(); it++)
	{
		string serviceName = it->first;
		int ret = connect(serviceName);
		if (ret != RET_OK) 
		{
			LOG_WARN("FcgiAgentHandler::connect fail for sname:%s",
					it->first.c_str());
		}
	}

	LOG_INFO("FcgiAgentHandler::connect, connect all fcgiagent end");
	return RET_OK;
}

int FcgiAgentHandler::connect(string& serviceName)
{
	LOG_INFO("FcgiAgentHandler::connect, connect all fcgiagent for service:%s",
				serviceName.c_str());

	FcgiServiceInfos::iterator it = fcgiServiceInfos.find(serviceName);
	if(it == fcgiServiceInfos.end())
	{
		LOG_ERROR("FcgiAgentHandler::connect unknown service:%s",
					serviceName.c_str());
		return RET_FAIL;
	}

	FcgiServiceInfo& fcgiServiceInfo = *it->second;

	int rc = 0;

	for (size_t i = 0; i < MAX_SERVICE_WORKER_COUNT; i++)
	{
		if(fcgiServiceInfo.agentinfos[i].ip
				!= "")
		{
			int r = connect(fcgiServiceInfo.agentinfos[i]);
			if (0 != r) {
				rc = -1;
			}
		}
	}

	LOG_INFO("FcgiAgentHandler::connect, connect all fcgiagent end for s_name:%s, count:%d",
					serviceName.c_str(), fcgiServiceInfo.worker_count);
	return rc;

}


int FcgiAgentHandler::connect(FcgiAgentInfo& fcgiagent_info)
{
	LOG_INFO("FcgiAgentHandler::connect, connect to [%s:%d] begin", 
					fcgiagent_info.ip.c_str(), fcgiagent_info.port);

	if (fcgiagent_info.connected) {	// 已经连接成功
		return 0;
	}
	if (0 != fcgiagent_info.net_id) {			// 正在连接
		return 0;
	}

	Net_Manager *net_manager = Dispatcher::getNetManager();
	fcgiagent_info.net_id = net_manager->create_tcp_client(fcgiagent_info.ip.c_str(), fcgiagent_info.port, &m_packet_splitter, 
				NetEventHandler, FCGIAGENT_CONNECT_TIMEOUT);
	if (0 == fcgiagent_info.net_id) {			// 创建网络通道失败
		fcgiagent_info.reconnect_time = time(NULL) + FCGIAGENT_RECONNECT_INTERVAL;	// 设置重新连接时间
		LOG_FATAL("FcgiAgentHandler::connect error, create_tcp_client error");
		return -1;
	}
	net_fcgiagent_map[fcgiagent_info.net_id] = &fcgiagent_info;

	fcgiagent_info.reconnect_time = 0;
	fcgiagent_info.keepalive_time = 0;
	fcgiagent_info.last_active_time = 0;

	LOG_INFO("FcgiAgentHandler::connect ok, net_id:%d", fcgiagent_info.net_id);
	return 0;
}

void FcgiAgentHandler::clearFcgiAgentStateinfo(FcgiAgentInfo& fcgiagent_info)
{
	list_head* pos, *next;
	FcgiAgentRequest* req;
	LOG_ERROR("FcgiAgent %s:%u clearFcgiAgentStateinfo\n", 
		fcgiagent_info.ip.c_str(), fcgiagent_info.port);			

	list_for_each_safe(pos, next, &fcgiagent_info.req_list)
	{
		req = list_entry(pos, FcgiAgentRequest, lst);

		LOG_ERROR("FcgiAgent %s:%u clear_req, cmd=%u seq=%u\n", 
			fcgiagent_info.ip.c_str(), fcgiagent_info.port, req->header.cmd, req->header.seq);			
	
		
		deleteReqList(req, fcgiagent_info);
		delete req;		
	}
	
	INIT_LIST_HEAD(&fcgiagent_info.req_list);
	for(int j = 0; j < FCGIAGENT_REQUEST_HASH_SIZE; j++)
		INIT_LIST_HEAD(&fcgiagent_info.req_hash[j]);
	
 	fcgiagent_info.curSeq = 0;
	fcgiagent_info.req_count = 0;
	return;
}


void FcgiAgentHandler::onConnectError(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event)
{
	LOG_FATAL("FcgiAgentHandler::onConnectError, net_id:%d, ip:%s, port:%d", fcgiagent_info.net_id,
		fcgiagent_info.ip.c_str(), fcgiagent_info.port);
		clearFcgiAgentStateinfo(fcgiagent_info);
	//	连接断开或keepalive超时时调用,清除上层的一些暂存状态

	net_fcgiagent_map.erase(fcgiagent_info.net_id);

	fcgiagent_info.net_id = 0;
	fcgiagent_info.connected = false;
	fcgiagent_info.reconnect_time = time(NULL) + FCGIAGENT_RECONNECT_INTERVAL;
	fcgiagent_info.keepalive_time = 0;
	fcgiagent_info.last_active_time = 0;
}

void FcgiAgentHandler::stop()
{
	LOG_DEBUG("fcgiAgentHandler Stop\n");
	for(FcgiServiceInfos::iterator it = fcgiServiceInfos.begin();it!=fcgiServiceInfos.end();++it)
	{
		FcgiServiceInfo * temp = it->second;
		LOG_DEBUG("delete FcgiServiceInfo: Service name = %s\n",temp->serviceName.c_str());
		delete temp;
	}
}

int FcgiAgentHandler::updateFcgiAgent(string servicename, uint16_t index, string ip, uint16_t port)
{
	LOG_INFO("FcgiAgentHandler::updateFcgiAgent, server:[%s:%d]", ip.c_str(), port);

	FcgiServiceInfos::iterator it = fcgiServiceInfos.find(servicename);
	if(it == fcgiServiceInfos.end())
	{
		LOG_ERROR("FcgiAgentHandler::updateFcgiAgent unknown service:%s",
					servicename.c_str());
		return RET_FAIL;
	}

	FcgiServiceInfo& fcgiServiceInfo = *it->second;

	FcgiAgentInfo& fcgiagent_info = fcgiServiceInfo.agentinfos[index];
	if ((fcgiagent_info.ip != ip) || (fcgiagent_info.port != port))
	{
		//服务器发生变化
		LOG_INFO("FcgiAgentHandler::updateFcgiAgent, server addr changed, oldip:%s, oldport:%d, newip:%s, newport:%d, reconnect",
				fcgiagent_info.ip.c_str(), fcgiagent_info.port, ip.c_str(), port);

		disconnect(fcgiagent_info);
		fcgiagent_info.ip = ip;
		fcgiagent_info.port = port;
		connect(fcgiagent_info);
		return 0;
	}

	// 服务器没有发生变化
	LOG_INFO("FcgiAgentHandler::updateFcgiAgent, server addr not changed, ignore");
	return 0;
}


void FcgiAgentHandler::checkTimeout(time_t current_time, FcgiAgentInfo & fcgiagent_info)
{
	LOG_DEBUG("FcgiAgentHandler::checkTimeout ");
	list_head* pos, *next;
	FcgiAgentRequest* req;

	time_t now = current_time;
	list_for_each_safe(pos, next, &fcgiagent_info.req_list)
	{
		req = list_entry(pos, FcgiAgentRequest, lst);
		if(now < req->timeout)
			break;
		
		LOG_ERROR("FcgiAgentHandler %s:%u request timeout, cmd=%u seq=%u\n", 
			fcgiagent_info.ip.c_str(), fcgiagent_info.port, req->header.cmd, req->header.seq);
		
		req->status = REQ_TIMEOUT;
		ApipooldHandler::sendCustomResponse(ApiPool::ERR_REQUEST_TIMEOUT,*(req->request),ApiPool::translateError(ApiPool::ERR_REQUEST_TIMEOUT));
		deleteReqList(req, fcgiagent_info);
		delete req;
	}
}

void FcgiAgentHandler::onNetData(FcgiAgentInfo& fcgiagent_info, Net_Event& net_event)
{
	Net_Packet& net_packet = net_event.packet;

	if (net_packet.length() < (int)sizeof(FCGI_AGENT_HDR))
	{
		LOG_FATAL("FcgiAgentHandler::onNetData error, packet length error, length:%d", net_packet.length());
		return;
	}

	FcgiAgentRequest* pReq = NULL;
	Binary_Read_Stream<FCGI_AGENT_HDR> rstream(net_packet.ptr(), net_packet.length());

	uint32_t len = ntohl(rstream.head().len);
	uint16_t ver = ntohs(rstream.head().ver);
	uint16_t cmd = ntohs(rstream.head().cmd);
	uint32_t seq = ntohl(rstream.head().seq);


	LOG_DEBUG("FcgiAgentHandler::onNetData cmd:%u ver:%d seq:%d len:%d from servicename:%s, ip:%s, port:%d", 
				cmd, ver, seq, len,
				fcgiagent_info.serviceName.c_str(),
				fcgiagent_info.ip.c_str(), fcgiagent_info.port);
	
	if (cmd == CMD_FCGIAGENT_KEEPALIVE)
	{
		onRecvKeepAlive(&fcgiagent_info);
	}
	else
	{
		LOG_HEX_PREFIX("fcgiagent resp msg:", net_packet.ptr(), net_packet.length(), utils::L_DEBUG);
		pReq = findReqList(seq, fcgiagent_info);
		if(pReq)
		{			
			pReq->status = REQ_SUCC;
			switch(cmd)
			{
				case CMD_FCGIAGENT_RPC:
				{					
					handleResponse(pReq, net_event, fcgiagent_info);
					break;
				}
				default:
					break;
			}
		}		
		else
		{
			LOG_WARN("FcgiAgentHandler::onNetData can't find cmd:%u ver:%d seq:%d, srvname:%s, ip:%s, port:%d", 
							cmd, ver, seq,
							fcgiagent_info.serviceName.c_str(),
							fcgiagent_info.ip.c_str(), fcgiagent_info.port);
							
		}
	}
	if(pReq != NULL)
	{
		deleteReqList(pReq, fcgiagent_info);
		delete pReq;
	}
	return;
}

void FcgiAgentHandler::handleResponse(FcgiAgentRequest* pReq, Net_Event& net_event, FcgiAgentInfo & fcgiagent_info)
{	
	int ret = RET_OK;
	Net_Packet& net_packet = net_event.packet;
	
	Response resq;
//		LOG_DEBUG("FcgiAgentHandler::handleResponse from:%p, len:%d", 
//						net_packet.ptr(),
//						net_packet.length());
	ret = transfer_from_fcgi_proto(*pReq->request, net_packet.ptr(),
					net_packet.length(), resq);
	
	ApipooldHandler::sendResponse(*pReq->request, resq);

	return;	
}



