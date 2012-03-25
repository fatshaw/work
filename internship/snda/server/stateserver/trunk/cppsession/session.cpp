#include <sys/time.h>
#include "session.h"
#include "tokenMessage.pb.h"
int StateSession::sendPack(const char * buf, int len){
	if(open == 1){
		int sendSize = 0;
		sendSize = send(fd,buf,len,0);
		if(sendSize == - 1){
			error = string(strerror(errno));
			closeConnection();
			return RET_FAIL;
		}
		++seq;
		return RET_OK;
	}
	else {
		return RET_FAIL;	
	}
}


int StateSession::createSocket(){
	struct sockaddr * addr = NULL;
	int len  = 0;
	char * unixSockPath = "/tmp/php-fpm.sock";
#ifdef UNIX_SOCK
	struct sockaddr_un fpm_unix_addr;
	memset(&fpm_unix_addr,0,sizeof(struct sockaddr_un));
	fpm_unix_addr.sun_family = AF_UNIX;
	strcpy(fpm_unix_addr.sun_path , unixSockPath);
	fd = socket(AF_UNIX,SOCK_STREAM,0);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(unixSockPath);
	addr = (struct sockaddr * )&fpm_unix_addr;
#else
	struct sockaddr_in iaddr;
	iaddr.sin_family = AF_INET;
	iaddr.sin_port = htons(port);
	iaddr.sin_addr.s_addr = inet_addr(host.c_str());
	fd = socket(AF_INET,SOCK_STREAM,0);
	len = sizeof(struct sockaddr_in);
	addr = (struct sockaddr *)&iaddr;
#endif

	if(fd == -1){
		return RET_FAIL;
	}
	if(connect(fd,addr,len) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		close(fd);
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		close(fd);
		return RET_FAIL;
	}
	int l = 0;
	if(getsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,(socklen_t *)&l) ==-1){
		close(fd);
		return RET_FAIL;
	}
	open = 1;
}

int StateSession::connectServer(){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	if(connect(fd,(struct sockaddr * )&addr,sizeof(struct sockaddr)) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	open = 1;
	struct sockaddr_in local_sock;
	socklen_t len = sizeof(sockaddr);
	getsockname(fd, (struct sockaddr*)&local_sock, &len);
	local_port = local_sock.sin_port;
	return RET_OK;
}

int StateSession::sendAliveMsg(){
	return 0;
}

int StateSession::recvData(int recvLen,char * buffer){
	int tmp = 0;
	int recved = 0;
	while(true){
		tmp = recv(fd,buffer+recved,recvLen-recved, 0);
		if(tmp == -1)
		{
				if(errno == EAGAIN){
					cerr<<"timeout"<<strerror(errno)<<endl;
				}
				else
				{
					perror("recv error");
					return RET_FAIL;
				}
		}
		else if(tmp == 0){
			return RET_OK;
		}
		else if(tmp > 0)
		{
			recved += tmp;
			if(recved >= recvLen){
				return recved;	
			}
		}
	}
}

uint64_t StateSession::getTimeSpan(struct timeval tv_begin, struct timeval tv_end){
	uint64_t timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec  - tv_begin.tv_usec);
	return timespan;
}
	
int StateSession::makeMessagePacket(string typeName, uint32_t protobufLength, char * protobuf,char * buf)
{
	uint32_t strLength = typeName.length();
	uint32_t totalLength= sizeof(uint32_t) *3 + strLength + protobufLength;

	if((int)totalLength > 1024)
	{
		return RET_FAIL;
	}
	int offset  = 0 ;
	int n_totalLength = htonl(totalLength);
	memcpy(buf+offset,(void*)&n_totalLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	int n_strLength = htonl(strLength);
	memcpy(buf+offset,(void*)&n_strLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(buf + offset ,(void*)typeName.c_str(),strLength);
	offset+= strLength;
	int n_protobuflength = htonl(protobufLength);
	memcpy(buf + offset ,(void*)&n_protobuflength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(buf + offset ,(void*)protobuf,protobufLength);
	offset+= protobufLength;
	return totalLength;
}

int StateSession::request(char * method,Args&params,string & result)
{
		assert(method);
		result.clear();
		StateServer::TokenMessage tokenMessage;			
		tokenMessage.set_seq(1);
		tokenMessage.set_apiname(method);
		tokenMessage.set_cmd(1);
		for(Args::iterator it = params.begin();it!=params.end();++it)
		{
				StateServer::TokenMessage_Paramlist * param = tokenMessage.add_params();
				param->set_key(it->first);
				param->set_value(it->second);
		}
		char pbuf[102400];
		char totalbuf[1024];
		int totallength = tokenMessage.ByteSize();
		if(tokenMessage.SerializeToArray(pbuf,totallength)==-1)
		{
			this->error = "protobuf serializer error";
			return RET_FAIL ;
		}
		totallength = makeMessagePacket("StateServer::StateServerRequest",totallength,pbuf,totalbuf);
		if(sendPack(totalbuf,totallength) == RET_FAIL)
		{
			return RET_FAIL;
		}
		if(recvData(4,totalbuf) == RET_FAIL)
		{
			return RET_FAIL;
		}
		else
		{
			int offset = 0 , strLength = 0 , protocolLength = 0;
			memcpy(&totallength,totalbuf,sizeof(int));
			totallength = ntohl(totallength);
			recvData(totallength-sizeof(int),totalbuf+sizeof(int));
			offset +=sizeof(int);
			memcpy((void*)&strLength, totalbuf+offset,sizeof(uint32_t));
			offset += sizeof(uint32_t);
			strLength= ntohl(strLength);
			string request_typename(totalbuf+offset,strLength);
			offset += strLength;
			memcpy((void*)&protocolLength, totalbuf+offset,sizeof(uint32_t));
			offset += sizeof(uint32_t);
			protocolLength = ntohl(protocolLength);
			StateServer::TokenMessage tokenMessage;
			tokenMessage.ParsePartialFromArray(totalbuf+offset,protocolLength);
			if(tokenMessage.params_size()>0)
				result = tokenMessage.params(0).value();
		}
}
#ifdef EXECUTE
int main(int argc,char ** argv)
{

	StateSession session;
	session.setServer("127.0.0.1",8110);
	session.setTimeout(1,0);//1s
	if(session.createSocket() == RET_FAIL){
		cerr<<"create socket error : "<<session.getLastError()<<endl;
		return RET_FAIL;	
	}
	string result;
	StateSession::Args args;
	args.insert(make_pair("session_id","123"));
	args.insert(make_pair("session_data","123"));
	args.insert(make_pair("expired_time","100"));
	session.request("SetSession",args,result);
	cout<<"result = "<<result<<endl;
	args.clear();
	args.insert(make_pair("session_id","123"));
	session.request("GetSession",args,result);
	cout<<"result = "<<result<<endl;
	return RET_OK;
}
#endif
