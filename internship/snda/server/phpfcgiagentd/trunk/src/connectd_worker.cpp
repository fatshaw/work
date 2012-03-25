#include <sstream>
#include "net_manager/src/net_manager.h"
//#include "net_manager/src/binary_stream.h"

#include "log.h"

#include "protocol.h"
#include "definition.h"
#include "common.h"

#include "connectd_worker.h"

Request_queue connectd_request_queue;

extern Net_Manager *net_manager;
extern Thread_Mutex event_put_mutex;

using namespace std;

Connectd_worker::Connectd_worker(int i)
{
	worker_id = i;
	p_event = NULL;
	
	connection_srcPort = 0;
	net_id = 0;
	local_stop = 0;
}

Connectd_worker::~Connectd_worker()
{
}

int Connectd_worker::handle_event(Net_Event* net_event)
{
	int ret = 0;
	
	//简单解析出包头
	Net_Packet& net_packet = net_event->packet;

	if (net_packet.length() < (int)sizeof(FCGI_AGENT_HDR))
	{
		LOG_WARN("[Connectd_worker]handle_event::net packet length error, length:%d  net=%d from ip=%s port=%u.", 
					net_packet.length(), net_event->id, 
					inet_ntoa((struct in_addr)connection_srcip),
					connection_srcPort); 
		return  0;
	}


	char * ptr = net_packet.ptr();
	FCGI_AGENT_HDR * hdr = (FCGI_AGENT_HDR*)ptr;

	uint16_t cmd = ntohs(hdr->cmd);

	//处理该请求
	switch(cmd)
	{
		case CONNECTD_KEEP_ALIVE:
		{
			ret = on_keep_alive(net_event);
			break;
		}
		case CONNECTD_USER_MSG:
		{				
			ret = on_user_msg(net_event);
			break;
		}
	}
	LOG_DEBUG("[Connectd_worker::handle_event]::process event=%p cmd=%d ret=%d",
						p_event, cmd, ret);
	return ret;
}

int Connectd_worker::on_keep_alive(Net_Event* net_event)
{
	int ret = RET_OK;
	
	Net_Packet& net_packet_recv = net_event->packet;
	
	Net_Packet *net_packet = new Net_Packet;

	memcpy(net_packet->ptr(), net_packet_recv.ptr(), net_packet_recv.length());
	net_packet->length(net_packet_recv.length());
	event_put_mutex.acquire();	
	ret = net_manager->send_packet(net_event->id, net_packet);	
	event_put_mutex.release();
	if(ret != RET_OK)
	{
		delete net_packet;
		LOG_ERROR("Connectd_worker::on_keep_alive send_packet ERROR net:%d", net_event->id);
		return RET_FAIL;
	}
	LOG_DEBUG("Connected_worker::ON KEEP ALIVE SUCCESS\n");
	return RET_OK;
}


int Connectd_worker::on_user_msg(Net_Event* net_event)
{
	int ret = onUsrMsg(net_event);
	return ret;
}


int Connectd_worker::add_request(Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = net_event->remote_addr.get_port();
	//bool ret = false;
	bool ret = connectd_request_queue.push(net_event);
	if(ret)
	{
		LOG_DEBUG("[Connectd_worker]::add_request event=%p success netid=%d ip=%s port=%d size=%d", 
						net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort, (int)connectd_request_queue.size()); 
		return RET_OK;
	}
	else
	{
		LOG_WARN("[Connectd_worker]::add_request event=%p fail netid=%d ip=%s port=%d ret=%d size=%d", 
						net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort, ret, (int)connectd_request_queue.size()); 
		return RET_QUEUE_PUSH_FAIL;
	}
}

int Connectd_worker::clear_request(Net_Event* net_event)
{
	int ret = 0;
	//关闭短连接
	//delete_net(net_event->net_id);
	
	LOG_TRACE("[Connectd_worker]::clear_request event=%p success netid=%d ip=%s port=%d", 
					net_event, net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	
	return ret;
}
	
int Connectd_worker::svc()
{	
	int ret = 0;
	LOG_INFO("Connectd_worker %d start...", worker_id);
	while(!local_stop)
	{
		//Net_Event* net_event = NULL;
		Net_Event* net_event = connectd_request_queue.pop();
		if(NULL == net_event)
		{
			usleep(sleep_interval_ms);
		}
		else
		{	
			LOG_TRACE("Connectd_worker::svc size=%d", (int)connectd_request_queue.size());
			//保存event指针，便于标识一个请求
			p_event = (void*)net_event;
			
			connection_srcip.s_addr = net_event->remote_addr.get_addr();
			connection_srcPort = net_event->remote_addr.get_port();
			net_id = net_event->id;
			
			LOG_TRACE("[Connectd_worker::svc]get event=%p id=%d ip=%s port=%d", 
						p_event,net_id,
						inet_ntoa((struct in_addr)connection_srcip), 
						connection_srcPort);
			
			handle_event(net_event);
			clear_request(net_event);
			delete net_event;
		}	
	}	
	
	LOG_INFO("Connectd_worker %d stop...", worker_id);
	return ret;
}



