#ifndef _WEB_WORKER_H
#define _WEB_WORKER_H
#include <task_base.h>
#include "dispatcher.h"

class Net_Event;
class Web_worker: public utils::Task_Base
{
public:
	Web_worker(int i);
	~Web_worker();

	virtual int svc();

	static int add_request(Net_Event* net_event);
	
public:
	int worker_id;	
	int local_stop;
	
private:
	int handle_event(Net_Event* net_event);
	int clear_request(Net_Event* net_event);
	int on_get_user_msg(Net_Event* net_event);
private:
	void* p_event;
	struct in_addr connection_srcip;
	unsigned int connection_srcPort;
	uint32_t net_id;
};

#endif  /* _WEB_WORKER_H */
