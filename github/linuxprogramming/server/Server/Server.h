#ifndef SERVER
#define SERVER
#include "Client.h"
#include <sys/select.h>

#define ALARM_SEC 5
#define ALARM_USEC 0

class Server{
	typedef list<Client*> ClientList;
private:
	int server_sock;
	ClientList clientList;
	bool isRunning;
	static pthread_mutex_t lock;
	void sendPluse();
public:
	void stop();
	Server():isRunning(false){}
	bool initServer();
	static void * cleanResource(void * argv);
	static void * recvAcceptThread(void*argv);
	static void * setAlarm(void * argv);
	void showClientList();
};

#endif
