#ifndef HEADER
#define HEADER

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <list>
#include <stdarg.h>
#include <sys/un.h>
#include <stddef.h>
using namespace std;

#define LENGTH 1024
#define BEGIN_RECORD_ID	 	1
#define END_RECORD_ID 		2
#define PLUSE_RECORD_ID	 	3
#define DATA_RECORD_ID		PLUSE_RECORD_ID + 1

extern const char unix_socket[];

extern unsigned int recordid;

struct header{
	unsigned int length;
	unsigned int recordid;
	header(){}
	header(unsigned int length,unsigned int recordid):length(length),recordid(recordid){}
};

void err_exit(const char * err);
void sock_err(const char * err,int fd = -1);
void makeBuf(char ** buf, string & content,unsigned int recordid);
void getHeader(const char * buf,header * pheader);
void err_doit(int errnoflag,int error,const char * fmt,va_list ap);
void debug(const char* fmt,...);

#endif

