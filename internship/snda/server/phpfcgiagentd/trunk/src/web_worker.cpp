#include <sstream>
#include <mysql.h>
#include <mysqld_error.h>

#include "net_manager/src/net_manager.h"

#include "log.h"
#include "protocol.h"
#include "definition.h"
#include "common.h"


#include "web_worker.h"

Request_queue web_request_queue(NULL, 100000, 500, 0);
extern Net_Manager *net_manager;
extern Thread_Mutex event_put_mutex;


Web_worker::Web_worker(int i)
{
	worker_id = i;
	p_event = NULL;
	
	connection_srcPort = 0;
	net_id = 0;
	local_stop = 0;

}

Web_worker::~Web_worker()
{
	
}

int Web_worker::handle_event(Net_Event* net_event)
{
	int ret = 0;
	
	//简单解析出包头
	Net_Packet& net_packet = net_event->packet;

	char * ptr = net_packet.ptr();
	FCGI_AGENT_HDR * hdr = (FCGI_AGENT_HDR*)ptr;

	uint16_t cmd = ntohs(hdr->cmd);
	
	//处理该请求
	switch(cmd)
	{
		case WEB_GET_USER_MSG:
		{
			ret = on_get_user_msg(net_event);
			break;
		}
	}
	LOG_DEBUG("[Web_worker::handle_event]::process event=%p cmd=%d ret=%d",
						p_event, cmd, ret);
	return ret;
}



int Web_worker::add_request(Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = net_event->remote_addr.get_port();

	bool ret = web_request_queue.push(net_event);
	if(ret)
	{
		LOG_DEBUG("[Web_worker]::add_request event=%p success netid=%d ip=%s port=%d ret=%d", 
						net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort, ret); 
		return RET_OK;
	}
	else
	{
		LOG_WARN("[Web_worker]::add_request event=%p fail netid=%d ip=%s port=%d ret=%d", 
						net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort, ret); 
		return RET_QUEUE_PUSH_FAIL;
	}
}

int Web_worker::clear_request(Net_Event* net_event)
{
	int ret = 0;
	//关闭短连接
	net_manager->delete_net(net_event->id);
	
	LOG_TRACE("[Web_worker]::clear_request event=%p success netid=%d ip=%s port=%d", 
					net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	
	return ret;
}


int Web_worker::on_get_user_msg(Net_Event* net_event)
{
	int ret = onUsrMsg(net_event);
	return ret;
}

int Web_worker::svc()
{	
	int ret = 0;
	LOG_INFO("Web_worker %d start...", worker_id);
	while(!local_stop)
	{
		Net_Event* net_event = web_request_queue.pop();
		if(NULL == net_event)
		{
			usleep(sleep_interval_ms);
		}
		else
		{	
			//保存event指针，便于标识一个请求
			p_event = (void*)net_event;
			
			connection_srcip.s_addr = net_event->remote_addr.get_addr();
			connection_srcPort = net_event->remote_addr.get_port();
			net_id = net_event->id;
			
			LOG_TRACE("[Web_worker::svc]get event=%p id=%d ip=%s port=%d", 
						p_event,net_id,
						inet_ntoa((struct in_addr)connection_srcip), 
						connection_srcPort);
			
			handle_event(net_event);
			clear_request(net_event);
			delete net_event;
		}	
	}	
	LOG_INFO("Web_worker %d stoped...", worker_id);
	return ret;
}



