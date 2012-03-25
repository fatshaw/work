#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#include <vector>

#include <set>

#include <net_manager/src/net_manager.h>

#include <task_base.h>

typedef void (*pfnTimerTickHandler)(time_t current_time);

using namespace std;

class Web_worker;
class Connectd_worker;
class Timer_worker;

class Dispatcher 
		: public utils::Task_Base
{
public:
	Dispatcher();
	void stop_worker();
	virtual int svc();
	static void NetEventHandler(Net_Event& net_event);
	void stop();
public:
	int local_stop;
	static int web_worker_num;
	static int connectd_worker_num;
private:	
	int init();
	int init_worker();
	int checkIncomingIP(Net_Event* net_event);
	int onAPPNetClose(uint8_t net_source, Net_Event* net_event);
	int onAPPNetError(uint8_t net_source, Net_Event* net_event);
	int onAPPNetData(uint8_t net_source, Net_Event* net_event);
	int onAPPNetAccept(Net_Event* net_event);
	int handleEvent(Net_Event* net_event);
	void registerHandler(pfnTimerTickHandler handler);
	void TimerTickHandler(time_t current_time);
	void checkTimeoutUser(time_t current_time);
private:
	uint64_t cur_time;
	vector<Web_worker*> p_web_workers; 
	vector<Connectd_worker*> p_connectd_workers;
	typedef set<pfnTimerTickHandler>TimeHandlerSet;
	TimeHandlerSet timeHandlerSet;
};

#endif  /* _DISPATCHER_H */
