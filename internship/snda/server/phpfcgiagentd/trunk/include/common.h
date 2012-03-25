#ifndef _COMMON_H
#define _COMMON_H
#include <stdint.h>
#include <sys/un.h>
#include <stddef.h>
#include "net_manager/src/net_manager.h"
#include "configure.h"


using namespace utils;

extern Thread_Mutex event_put_mutex;
extern Net_Manager *net_manager;

extern uint32_t sleep_interval_ms;
extern char fpmip[100];
extern uint16_t fpmport;
extern uint32_t sock_timeout;
extern struct sockaddr_in fpm_addr;
extern struct timeval timeout;
extern struct sockaddr_un fpm_unix_addr;
extern uint8_t isUnixSocket;
extern char unixSockPath[100];


int init_log(utils::Configure &configure, bool is_reload = false);

int init_Admin_Info();
int loadCfg();

int initSock(int & fd);
int recvFromFpm(int fd, char * buf ,uint32_t* length);
int  sendToFpm(int fd, const  char * buf,uint32_t  length);
int onUsrMsg(Net_Event* net_event);

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)


#endif  /* _COMMON_H */

