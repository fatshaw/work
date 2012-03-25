
#include "options.h"
#include <stdlib.h>

#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "AccessControler.h"

#include "state.h"
#include "dispatcher.h"
#include "log.h"


// class Dispatcher
Dispatcher::NetEventHandlerMap Dispatcher::m_net_event_handler_map;

Dispatcher::TimerTickHandlerSet Dispatcher::m_timer_tick_handler_set;

Net_Manager Dispatcher::m_net_manager;

bool Dispatcher::m_stop;


void Dispatcher::registerNetEventHandler(uint32_t net_id, pfnNetEventHandler handler)
{
	m_net_event_handler_map[net_id] = handler;
}

void Dispatcher::removeNetEventHandler(uint32_t net_id)
{
	m_net_event_handler_map.erase(net_id);
}

void Dispatcher::registerTimerTickHandler(pfnTimerTickHandler handler)
{
	m_timer_tick_handler_set.insert(handler);
}

void Dispatcher::removeTimerTickHandler(pfnTimerTickHandler handler)
{
	m_timer_tick_handler_set.erase(handler);
}

Net_Manager* Dispatcher::getNetManager()
{
	return &m_net_manager;
}

void Dispatcher::run()
{
	m_stop = false;
	time_t last_time = time(NULL);
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	struct in_addr connection_local_ip;
	unsigned int connection_local_port = 0;


	//Try Reload visit history
	int access_dump_shmid = 0; 
	char *p_access_dump_addr = NULL;
	if((access_dump_shmid = shmget(Options::apipoold.shm_key+1,STATE_SHM_SIZE,IPC_CREAT | 0666))==-1) 
	{ 
		LOG_FATAL("[ServerState]Create Share Memory for accessinfo Error: SHMKEY=%u\n\a", 
					Options::apipoold.shm_key+1); 
		return;
	} 
	p_access_dump_addr = (char*)shmat(access_dump_shmid,0,0); 
	p_access_dump_addr[STATE_SHM_SIZE-1] = 0; 
	AccessControler::loadVisitorHistory(p_access_dump_addr);

	time_t  curtime,lasttime;
	int requestcnt = 0;
	while (false == m_stop)
	{
		ServerState::g_pulse_mainthread++;
		// 处理网络事件
		Net_Event *net_event = m_net_manager.get_event();
		
		if (NULL == net_event)	// 没有网络事件发生
		{
			usleep(10);
		}
		else	// 取到一个网络事件
		{
			curtime= time(NULL);
			if(curtime - lasttime >= 1)
			{
				LOG_DEBUG("[Dispatcher::run]RECV COUNT = %d\n",requestcnt);
				lasttime = curtime;
				requestcnt= 0 ;
			}
			requestcnt++;
			
			connection_srcip.s_addr = net_event->remote_addr.get_addr();
			connection_srcPort = net_event->remote_addr.get_port();
			connection_local_ip.s_addr = net_event->local_addr.get_addr();
			connection_local_port = net_event->local_addr.get_port();

			LOG_DEBUG("Dispatcher::run send_size=%d, recv_size=%d", 
						m_net_manager.get_send_size(),
						m_net_manager.get_recv_size());			
			if (TYPE_SYS_ERROR == net_event->net_event_type) 
			{
				// net manager error
				LOG_FATAL("Dispatcher::got TYPE_SYS_ERROR on netmanager");
				
				break;
			}

			if (net_event->handler != NULL) {
				net_event->handler(*net_event);
			}
			else {
				// packet not handle
				LOG_FATAL("Dispatcher::packet not handle, socket %d from ip=%s port=%u",
							net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
			}

			delete net_event;
		}

		// 处理时间事件
		time_t current_time = time(NULL);
		if ((current_time - last_time) >= 1)	// 每秒一次
		{
			ServerState::serialize2shm(current_time);
			for (TimerTickHandlerSet::iterator iter = m_timer_tick_handler_set.begin(); iter != m_timer_tick_handler_set.end(); iter++)
			{
				pfnTimerTickHandler TimerTickHandler = *iter;
				TimerTickHandler(current_time);
			}
			last_time = current_time;
		}
	}
}

void Dispatcher::stop()
{
	m_stop = true;
}

