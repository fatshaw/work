#include <sstream>
#include "apipoold_handler.h"
#include "apipoold_listener.h"
#include "state.h"

#include "service_router.h"

#include "dispatcher.h"
#include "options.h"
#include "common.h"
#include "log.h"


#include "AccessControler.h"

using namespace ApiPool;


//到外部业务系统APP的长连接与在MiddleAgent中对应Net_info的Map表
//	NetMap		net_map_longtcp;	//---这里只保存需要超时检查的长连接 
//	NetMap		net_map_shorttcp;	//---这里只保存需要超时检查的短连接


ListMap<NetInfo> ApipooldHandler::net_map_longtcp;	//---这里只保存需要超时检查的长连接 
ListMap<NetInfo> ApipooldHandler::net_map_shorttcp;	//---这里只保存需要超时检查的短连接


// class ApipooldHandler
void ApipooldHandler::NetEventHandler(Net_Event& net_event)
{
	
	switch (net_event.net_event_type)
	{
	case TYPE_EXCEPTION:
	case TYPE_CLOSE:
		onNetError(net_event);
		break;

	case TYPE_DATA:
		onNetData(net_event);
		break;

	default:
		LOG_FATAL("ApipooldHandler::NetEventHandler error, unknown net_event_type:%d, netid:%d", 
					net_event.net_event_type, net_event.id);
		break;
	}
}

void ApipooldHandler::TimerTickHandler(time_t current_time)
{
	// 处理超时用户
	checkTimeoutUser(current_time);
}

void ApipooldHandler::checkTimeoutUser(time_t current_time)
{
	LOG(5)("ApipooldHandler::checkTimeoutUser longtcp size:%d, shorttcp size:%d",
					net_map_longtcp.size(), net_map_shorttcp.size());

	NetInfo* netinfo = NULL;
	int count = 0;
	ListMap<NetInfo>::iterator it = net_map_longtcp.begin();
	for(it = net_map_longtcp.begin(); it != net_map_longtcp.end(); )
	{
		netinfo = net_map_longtcp.get_by_it(it);
		LOG_DEBUG("ApipooldHandler::checkTimeoutUser longtcp connect SCAN net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
				netinfo->net_id, netinfo->ip, netinfo->port, netinfo->updateTime, current_time);
		if((uint64_t)(netinfo->updateTime + Options::apipoold.longtcp_timeout) < current_time)
		{
			//NET INFO TIMEOUT, DISCONNECT
			LOG_WARN("ApipooldHandler::checkTimeoutUser longtcp connect timeout net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
					netinfo->net_id, netinfo->ip, netinfo->port, netinfo->updateTime, current_time);
			Net_Manager *net_manager = Dispatcher::getNetManager();
			net_manager->delete_net(netinfo->net_id);
			net_map_longtcp.erase(it);
			delete netinfo;
			count++;
		}
		else
		{
			// 未超时, 后面的用户也不会超时
			
			break;
		}
	}

	it = net_map_shorttcp.begin();
	for(; it != net_map_shorttcp.end(); )
	{
		netinfo = net_map_shorttcp.get_by_it(it);
		LOG_DEBUG("ApipooldHandler::checkTimeoutUser shorttcp connect SCAN net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
				netinfo->net_id, netinfo->ip, netinfo->port, netinfo->updateTime, current_time);
		if((uint64_t)(netinfo->updateTime + Options::apipoold.shorttcp_timeout) < current_time)
		{
			//NET INFO TIMEOUT, DISCONNECT
			LOG_WARN("ApipooldHandler::checkTimeoutUser shorttcp connect timeout net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
					netinfo->net_id, netinfo->ip, netinfo->port, netinfo->updateTime, current_time);
			Net_Manager *net_manager = Dispatcher::getNetManager();
			net_manager->delete_net(netinfo->net_id);
			net_map_shorttcp.erase(it);
			delete netinfo;
			count++;
		}
		else
		{
			// 未超时, 后面的用户也不会超时
			break;
		}
	}

	LOG_INFO("RouterdHandler::checkTimeoutUser %d timeout", 
				count);
}


void ApipooldHandler::onNetConnect(Net_Event& net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event.remote_addr.get_addr();
	connection_srcPort = net_event.remote_addr.get_port();


	if(ApipooldListener::apipoold_longtcp_info.net_id == net_event.id)
	{
		//长连接需要保存连接信息，以备对连接超时的检查
		NetInfo* p_tempNetInfo = new NetInfo();
		
		p_tempNetInfo->net_id = net_event.new_id;
		p_tempNetInfo->updateTime = time(NULL);
		p_tempNetInfo->port = connection_srcPort;
		
		strncpy(p_tempNetInfo->ip, inet_ntoa((struct in_addr)connection_srcip), 
					sizeof(p_tempNetInfo->ip)-1); 
		
//			net_map_longtcp[net_event.new_id] = tempNetInfo;			

		net_map_longtcp.add_tail(p_tempNetInfo->net_id, p_tempNetInfo);
		
		LOG_INFO("[onNetConnect]Long tcp connect accept ip=%s port=%d COMING, net_id=%d",
						p_tempNetInfo->ip, p_tempNetInfo->port,
						p_tempNetInfo->net_id);	 
		
	}
	else if(ApipooldListener::apipoold_shorttcp_info.net_id == net_event.id)
	{
		//短连接需要保存连接信息，以备对连接超时的检查
		NetInfo* p_tempNetInfo = new NetInfo();
		
		p_tempNetInfo->net_id = net_event.new_id;
		p_tempNetInfo->updateTime = time(NULL);
		p_tempNetInfo->port = connection_srcPort;
		
		strncpy(p_tempNetInfo->ip, inet_ntoa((struct in_addr)connection_srcip), 
					sizeof(p_tempNetInfo->ip)-1); 
		
//			net_map_shorttcp[net_event.new_id] = tempNetInfo;			
		net_map_shorttcp.add_tail(p_tempNetInfo->net_id, p_tempNetInfo);
		LOG_INFO("[Dispatcher]Short tcp connect accept ip=%s port=%d COMING, net_id=%d", 
							inet_ntoa((struct in_addr)connection_srcip), connection_srcPort,
							net_event.new_id);			
	}
	else 
	{
		LOG_WARN("[Dispatcher]Connect coming to unkown listen net=%d from ip=%s port=%d deny it", 
							net_event.id, inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);	
		Net_Manager *net_manager = Dispatcher::getNetManager();
		net_manager->delete_net(net_event.new_id);			
		return;
	}
	LOG_INFO("ApipooldHandler::onNetConnect, net_id:%d, new_id:%d, ip:%s, port:%d", 
					net_event.id, net_event.new_id, 
					inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
}

/******************************************************************************
 * ApipooldHandler.onNetError - 清理
 * DESCRIPTION: - 
 1. 将该connectd上的uin全部离线
 2. 清理apipoold_by_netid, apipoold_by_connectid里面的记录，
 3. 记日志
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 20aug2009, Davidfan written
 * --------------------
 ******************************************************************************/
void ApipooldHandler::onNetError(Net_Event& net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event.remote_addr.get_addr();
	connection_srcPort = net_event.remote_addr.get_port();

	if(net_event.net_event_type == TYPE_CLOSE)
	{
		LOG_INFO("ApipooldHandler::onNetError, NET CLOSE net_id:%d, type:%d, ip:%s, port:%d", 
			net_event.id, net_event.net_event_type, 
			inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
	}
	else
	{
		LOG_FATAL("ApipooldHandler::onNetError, NET EXCEPTION net_id:%d, type:%d, ip:%s, port:%d", 
			net_event.id, net_event.net_event_type,
			inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
	}

	NetInfo* p_net_info = net_map_shorttcp.get(net_event.id);
	if(p_net_info != NULL)
	{
		remove_net(net_event.id);
	}
	else
	{	
		p_net_info = net_map_longtcp.get(net_event.id);
		if(p_net_info != NULL)
		{
			remove_net(net_event.id);
		}
	}
	
	return;	
}

void ApipooldHandler::remove_net(uint32_t net_id)
{
	NetInfo* p_net_info = NULL;
	if(NULL == (p_net_info = net_map_shorttcp.erase(net_id)))
	{
		p_net_info = net_map_longtcp.erase(net_id);
	}
	
	if(p_net_info != NULL)
	{
		delete p_net_info;
	}
	else
	{
		LOG_WARN("ApipooldHandler::remove_net can't find:%d",
				net_id);
	}
	return;
}


/******************************************************************************
 * ApipooldHandler.onNetData - 处理connectd上来的数据包
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 20aug2009, Davidfan written
 * --------------------
 ******************************************************************************/
void ApipooldHandler::onNetData(Net_Event& net_event)
{
	//step1. parse buffer 得到request
	int ret = RET_OK;	
	Net_Packet& net_packet = net_event.packet;
	LOG_HEX_PREFIX("ApipooldHandler::onNetData msg:", 
				net_packet.ptr(), net_packet.length(), utils::L_DEBUG);

	struct in_addr connection_srcip;
	connection_srcip.s_addr = net_event.remote_addr.get_addr();
	string remoteip = inet_ntoa((struct in_addr)connection_srcip);

	NetInfo* p_net_info = net_map_shorttcp.get(net_event.id);
	if(p_net_info != NULL)
	{
		net_map_shorttcp.move_2_tail(p_net_info->net_id);
	}
	else
	{	
		p_net_info = net_map_longtcp.get(net_event.id);
		if(p_net_info != NULL)
		{
			net_map_longtcp.move_2_tail(p_net_info->net_id);
		}
	}
	
	if(p_net_info == NULL)
	{
		LOG_WARN("ApipooldHandler::onNetData can't find:%d, ip:%s, port:%d",
					net_event.id, inet_ntoa((struct in_addr)connection_srcip), 
					net_event.remote_addr.get_port());
		Net_Manager *net_manager = Dispatcher::getNetManager();
		net_manager->delete_net(net_event.id);
		return;			
	}

	p_net_info->updateTime = time(NULL);
	

	Request* request = new Request();


	request->setIp(remoteip);
	request->setPort(net_event.remote_addr.get_port());
	request->setNetId(net_event.id);

	bool parse_res = request->parsePackage(net_packet.ptr(), net_packet.length());
	if(!parse_res)
	{
		LOG_WARN("onNetData unparsePackage fail net_id:%d, ip:%s, port:%d", 
			net_event.id, 
			inet_ntoa((struct in_addr)connection_srcip), net_event.remote_addr.get_port());
		sendCustomResponse(ApiPool::ERR_STATUS_BAD_PACKET_DATA, *request,ApiPool::translateError(ApiPool::ERR_STATUS_BAD_PACKET_DATA));
		delete request;
		return;
	}	

	string reason;
	int check_res = AccessControler::check(request->getRequestSrc().c_str(),
						request->getServiceName().c_str(),
						request->getApiName().c_str(),
						request->getIp().c_str(),
						reason);

	if(-1 == check_res)
	{
		LOG_WARN("[AccessControler::check] check fail for:%s, net_id:%d, ip:%s, port:%d, reason:%s", 
				request->getServiceName().c_str(), net_event.id, 
			inet_ntoa((struct in_addr)connection_srcip), net_event.remote_addr.get_port(),reason.c_str());
		char errorinfo[1024] = {0};
		snprintf(errorinfo, sizeof(errorinfo), "%s reason:%s", 
					ApiPool::translateError(ApiPool::ERR_STATUS_UNAUTHORIZED),
					reason.c_str());
		
		sendCustomResponse(ApiPool::ERR_STATUS_UNAUTHORIZED, *request, errorinfo);
		delete request;
		return;
	}

	ret = ServiceManager::handlerRequest(request);
	if(ret != RET_OK)
	{
		LOG_WARN("onNetData handlerRequest fail for:%s, net_id:%d, ip:%s, port:%d", 
				request->getServiceName().c_str(), net_event.id, 
			inet_ntoa((struct in_addr)connection_srcip), net_event.remote_addr.get_port());
		sendCustomResponse(ApiPool::ERR_STATUS_UNAVAILABLE, 
							*request,ApiPool::translateError(ApiPool::ERR_STATUS_UNAVAILABLE));
		delete request;
		return;
	}
	return;
}

void ApipooldHandler::sendCustomResponse(int http_err_code, Request& request,const char * errorInfo)
{
	Response response;
	response.setError(http_err_code);
	response.setResult(Response::nullValue());
	response.setErrorInfo(errorInfo);
	response.setId(request.getId());
	sendResponse(request, response);
	return;
}

void ApipooldHandler::tryCloseClient(Request& request)
{
	NetInfo* p_net_info = net_map_shorttcp.get((uint32_t)request.getNetId());
	if(p_net_info != NULL)
	{
		Net_Manager *net_manager = Dispatcher::getNetManager();
		net_manager->delete_net((uint32_t)request.getNetId());

		LOG_DEBUG("ApipooldHandler::tryCloseClient CLOSE net_id:%d, updatetime:%ld, ip:%s, port:%d",
						p_net_info->net_id, p_net_info->updateTime,
						p_net_info->ip, p_net_info->port);
		NetInfo* p_net_info_tmp = net_map_shorttcp.erase((uint32_t)request.getNetId());
		if(p_net_info_tmp != NULL)
		{
			delete p_net_info_tmp;
		}
		else
		{
			LOG_WARN("ApipooldHandler::tryCloseClient can't find:%d",
						p_net_info->net_id);
		}
		request.setNetId(0);
	}
}

void ApipooldHandler::sendResponse(Request& request, Response& response)
{
	string out_pack;
	
	if(!response.makePackage(out_pack))
	{
		LOG_WARN("ApipooldHandler::sendResponse makePackage fail for reqsrc:%s, orgin reqid:%s, reqsrv:%s",
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());
	}
	else
	{	
		Net_Packet *net_packet = new Net_Packet;
		if((int)out_pack.length() > net_packet->max_packet_length())
		{
			LOG_ERROR("ApipooldHandler::sendResponse resp len:%d exceed max pack len:%d for reqsrc:%s, orgin reqid:%s, reqsrv:%s",
							(int)out_pack.length(), (int)net_packet->max_packet_length(),
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());
		}
		else
		{
			memcpy(net_packet->ptr(), out_pack.c_str(), out_pack.length());
			net_packet->length(out_pack.length());
			
			LOG_HEX_PREFIX("ApipooldHandler::sendResponse ", net_packet->ptr(), net_packet->length(), utils::L_DEBUG);
			Net_Manager *net_manager = Dispatcher::getNetManager();
			int rc = net_manager->send_packet((uint32_t)request.getNetId(), net_packet);
			if (0 != rc) 
			{				
				delete net_packet;
				LOG_FATAL("ApipooldHandler::sendResponse error, send_packet error, net_id:%d, ip:%s, port:%d for reqsrc:%s, orgin reqid:%s, reqsrv:%s", 
							(uint32_t)request.getNetId(), request.getIp().c_str(), (int)request.getPort(),							
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());
			}
			else
			{
				LOG_INFO("ApipooldHandler::sendResponse succ, net_id:%d, ip:%s, port:%d for reqsrc:%s, orgin reqid:%s, reqsrv:%s", 
							(uint32_t)request.getNetId(), request.getIp().c_str(), (int)request.getPort(),							
							request.getRequestSrc().c_str(), 
							request.getId().asString().c_str(),
							request.getServiceName().c_str());				
			}
		}
	}	
	tryCloseClient(request);
	return;
}

void ApipooldHandler::getApipooldHandlerInformation(char* buf, int len)
{	
	stringstream logstr;
	logstr<<"***ApipooldHandler info***"<<endl;

	static ListMap<NetInfo> net_map_longtcp;	//---这里只保存需要超时检查的长连接 
	static ListMap<NetInfo> net_map_shorttcp;	//---这里只保存需要超时检查的短连接

	logstr<<"longtcp size:"<<net_map_longtcp.size()<<endl;
	logstr<<"shorttcp size:"<<net_map_shorttcp.size()<<endl;

	strncpy(buf, logstr.str().c_str(), len);
	return;
}


