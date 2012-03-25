#ifndef _SESSION_H
#define _SESSION_H

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <sys/un.h>
#include <stddef.h>
#include <string>
#include <map>
#include <sstream>
#include <list>

using namespace std;

#define RET_OK 0 
#define RET_FAIL 1
#define RET_SOCK_CLOSE 2
extern "C"{
class StateSession{
public:
typedef map<string,string> Args;
	int fd;
	unsigned int seq;
	unsigned short recordID;
	string error;
	string host;
	unsigned short port;
	struct timeval timeout;
	unsigned char open;	
	int sendPack(const char * buf, int len);
public:
	StateSession():fd(0),recordID(1),seq(1),host("127.0.0.1"),port(8110){
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
	}
	
	void closeConnection(){
		if(fd > 0){
			open = 0;
			close(fd);
			fd = 0;
		}
	}
	
	unsigned char isOpen(){
		return open;	
	}
	
	~StateSession(){
		if(fd > 0){
			open = 0;
			close(fd);	
			fd = 0;
		}
	}
	
	void displayTimeout()
	{
		cout<<"timeout tv_sec = "<<timeout.tv_sec<<endl;
		cout<<"timeout tv_usec= "<<timeout.tv_usec<<endl;
	}

	string & getLastError(){
		return error;	
	}

	void setServer(const string & host , const unsigned short port){
		this->host = host;
		this->port = port;	
	}
	
	int setTimeout(const long timeout_sec = 0, const long timeout_usec = 5000000){
		this->timeout.tv_sec = timeout_sec;
		this->timeout.tv_usec = timeout_usec;	
	}
	int sendAliveMsg();
	int recvData(int recvLen,char * buffer);
	int createSocket();
	int connectServer();
	uint64_t getTimeSpan(struct timeval tv_begin, struct timeval tv_end);
	int makeMessagePacket(string typeName, uint32_t protobufLength, char * protobuf,char * buf);
	int request(char * method,Args&pararms,string & result);
public:
	unsigned short local_port;
};
}
#endif

