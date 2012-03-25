#ifndef _CONNECTD_WORKER_H
#define _CONNECTD_WORKER_H

#include <task_base.h>

class Connectd_worker
	: public utils::Task_Base
{
public:

	static int add_request(Net_Event* net_event);
	int handle_event(Net_Event* net_event);
	int clear_request(Net_Event* net_event);

	Connectd_worker(int i);
	~Connectd_worker();

	virtual int svc();

public:
	int worker_id;	
	int local_stop;
private:
	
	int on_keep_alive(Net_Event* net_event);	
	int on_user_msg(Net_Event* net_event);

private:
	void* p_event;
	struct in_addr connection_srcip;
	unsigned int connection_srcPort;
	uint32_t net_id;
};


#endif  /* _CONNECTD_WORKER_H */
