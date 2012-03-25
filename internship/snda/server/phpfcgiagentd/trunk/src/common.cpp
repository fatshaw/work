#include <sys/time.h>
#include <sstream>

#include <rwlock.h>
#include "log.h"

#include "definition.h"
#include "protocol.h"
#include "common.h"

uint32_t sleep_interval_ms = 100;		//没有任何网络事件时的等待时间，单位是us		
uint32_t sock_timeout = 50000;

struct sockaddr_in fpm_addr;
struct sockaddr_un fpm_unix_addr;
char unixSockPath[100];
uint8_t isUnixSocket;
struct timeval timeout;

int initSock(int & fd){
	struct sockaddr * addr = NULL;
	int len  = 0;
	if(isUnixSocket == 1){
		fd = socket(AF_UNIX,SOCK_STREAM,0);
		len = offsetof(struct sockaddr_un,sun_path) + strlen(unixSockPath);
		addr = (struct sockaddr * )&fpm_unix_addr;
	}
	else if(isUnixSocket == 0){
		fd = socket(AF_INET,SOCK_STREAM,0);
		len = sizeof(struct sockaddr_in);
		addr = (struct sockaddr *)&fpm_addr;
	}
	
	if(fd == -1){
		LOG_ERROR("[PhpAgent InitSock]create  fpm  socket error: %s\n",strerror(errno));
		return RET_FAIL;
	}
	if(connect(fd,addr,len) == -1){
		LOG_ERROR("[PhpAgent InitSock]connect error to fpm %s \n",strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		LOG_ERROR("[PhpAgent InitSock]setopt error: %s\n",strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		LOG_ERROR("[PhpAgent InitSock]setopt error: %s\n",strerror(errno));		
		close(fd);
		return RET_FAIL;
	}
	int l = 0;
	if(getsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,(socklen_t *)&l) ==-1){
		LOG_ERROR("[PhpAgent InitSock]get socket opt  error: %s\n",strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	return RET_OK;
}

int recvData(int fd, int recvLen,char * buffer){
	int tmp = 0;
	int recved = 0;
	while(true){
		tmp = recv(fd,buffer+recved,recvLen-recved, 0);
		if(tmp == -1)
		{
			if(errno == EAGAIN){
				LOG_ERROR("[PhpAgent] recv from Fpm TIMEOUT : %s\n",strerror(errno));
				LOG_ERROR("[PhpAgent] RECV TIMEOUT<timeout>%lu(ms)\n",timeout.tv_sec*1000+timeout.tv_usec /1000);
			}
			return -1;
		}
		else if(tmp == 0){
			LOG_DEBUG("[PhpAgent] conect close\n");
			return 0;
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

void recvFcgiHeader(FCGI_HDR * hdr,const char * buf)
{
	FCGI_HDR * phdr = (FCGI_HDR*)(buf);
	hdr->version = phdr->version;
	hdr->type = phdr->type;
	hdr->requestID = ntohs(phdr->requestID);
	hdr->contentLength = ntohs(phdr->contentLength);
	hdr->paddingLength = phdr->paddingLength;
	hdr->reserved = phdr->reserved;
	LOG_DEBUG("FCGI_HDR info version:%d, type:%d, requestID:%d,contentLength:%d,paddingLength:%d,reserved:%d",
			hdr->version, hdr->type, hdr->requestID,
			hdr->contentLength, hdr->paddingLength, hdr->reserved);
}

int recvFromFpm(int fd , char * buf, uint32_t *  length){
	char recvbuf[MAX_PHP_PACK_LEN];
	int temp = 0;
	int recvSize = 0;
	FCGI_HDR fcgi_hdr;
	while(1)
	{
		temp = recvData(fd, sizeof(FCGI_HDR), recvbuf);
		if(temp  == -1)
		{
			LOG_WARN("recvFromFpm recv FCGI_HDR fail.");
			return RET_FAIL;
		}else if(temp == 0)
		{
			LOG_WARN("recvFromFpm recv CLOSE SIGNAL.");
			break;
		}
		else if(temp > 0)
		{
			LOG_DEBUG("recvFromFpm recv len:%d.", temp);
			if(recvSize + temp > MAX_PHP_PACK_LEN)
			{
				LOG_ERROR("[RecvFromFpm]Recv Error From Fpm Buf is Too Large\n");
				return RET_FAIL;
			}
			memcpy(buf+recvSize,recvbuf,temp);
			recvSize += temp;
		}
		recvFcgiHeader(&fcgi_hdr,recvbuf);
		int leng = fcgi_hdr.contentLength + fcgi_hdr.paddingLength;
		LOG_DEBUG("recvFromFpm recv need contentLength:%d, paddingLength:%d", 
						fcgi_hdr.contentLength, fcgi_hdr.paddingLength);
		
		if(leng+ recvSize > MAX_PHP_PACK_LEN)
		{
			LOG_ERROR("[RecvFromFpm]Recv Error From Fpm Buf is Too Large\n");
			return RET_FAIL;
		}
		temp = recvData(fd,leng,recvbuf);
		if(temp == -1)
		{
			LOG_WARN("recvFromFpm recv content fail.");
			return RET_FAIL;
		}
		else if(temp == 0)
		{
			LOG_WARN("recvFromFpm recv content CLOSE SIGNAL.");
			break;
		}
		else if(temp > 0)
		{
			if(recvSize + temp > MAX_PHP_PACK_LEN)
			{
				LOG_ERROR("[RecvFromFpm]Recv Error From Fpm Buf is Too Large\n");
				return RET_FAIL;
			}
			memcpy(buf+recvSize,recvbuf,temp);
			recvSize += temp;			
		}
		if(fcgi_hdr.type == FCGI_END_REQUEST)
		{
			break;
		}
	}
	*length = recvSize;
	return RET_OK;
}
 
int sendToFpm(int fd , const char * sendbuf,uint32_t length){
	int sendSize = 0;
	if( (sendSize = send(fd,sendbuf,length,0))  == -1){
		LOG_ERROR("[PhpAgent InitSock]send  error: %s\n",strerror(errno));
		return RET_FAIL;
	}
	return RET_OK;
}

uint64_t getTimeSpan(struct timeval tv_begin, struct timeval tv_end){
	unsigned long timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 
				+ ((tv_end.tv_usec >= tv_begin.tv_usec) ? (tv_end.tv_usec - tv_begin.tv_usec) : (tv_begin.tv_usec - tv_end.tv_usec));
	return timespan;
}

int onUsrMsg(Net_Event* net_event){
	static uint64_t request_count = 0;
	request_count++;
	int ret = RET_OK;
	
	Net_Packet& net_packet = net_event->packet;

	char * ptr = net_packet.ptr();

	FCGI_AGENT_HDR* hdr = (FCGI_AGENT_HDR*)ptr;
	LOG_DEBUG("[[PhpAgent OnUsrMsg] [RequestCount] %lu for netid:%d, seqid:%d",
					request_count, net_event->id, ntohl(hdr->seq));
	uint32_t length = ntohl(hdr->len);	
	length -=sizeof(FCGI_AGENT_HDR);
	struct timeval tv_begin;
	struct timeval tv_end;
	uint64_t timespan = 0;
	struct timeval total_begin;
	struct timeval total_end;
	gettimeofday(&total_begin,NULL);
	if(length > 0)
	{
		gettimeofday(&tv_begin,NULL);
		char buf[MAX_PHP_PACK_LEN];
		int fd = -1;
		ret = initSock(fd);
		if(ret == RET_FAIL)
		{
			if(isUnixSocket == 0)
			{
				LOG_ERROR("[PhpAgent OnUsrMsg] Connect to fpm ip=%s port = %d error  for netid:%d, seqid:%d",
					fpmip,fpmport, net_event->id, ntohl(hdr->seq));
			}
			else if(isUnixSocket == 1){
				LOG_ERROR("[PhpAgent OnUsrMsg] Connect to fpm unixSockPath = %s error  for netid:%d, seqid:%d",
					unixSockPath, net_event->id, ntohl(hdr->seq));
			}
			return RET_FAIL;
		}
		gettimeofday(&tv_end,NULL);
		timespan = getTimeSpan(tv_begin,tv_end);
		LOG_DEBUG("[STATMSG,PhpAgent OnUsrMsg,Init Socket to fpm Success,time:%ld(us) for netid:%d, seqid:%d]\n",
							timespan, net_event->id, ntohl(hdr->seq));
		gettimeofday(&tv_begin,NULL);
		ret = sendToFpm(fd,ptr+sizeof(FCGI_AGENT_HDR),length);
		if(ret == RET_FAIL)
		{
			if(isUnixSocket == 0){
				LOG_ERROR("[PhpAgent OnUsrMsg] send to fpm ip=%s port = %d error for netid:%d, seqid:%d",
					fpmip,fpmport, net_event->id, ntohl(hdr->seq));
			}
			else if(isUnixSocket == 1){
				LOG_ERROR("[PhpAgent OnUsrMsg] send to fpm unixSockPath = %s error for netid:%d, seqid:%d",
					unixSockPath, net_event->id, ntohl(hdr->seq));
			}
			close(fd);
			return RET_FAIL;
		}

		gettimeofday(&tv_end,NULL);
		timespan = getTimeSpan(tv_begin,tv_end);
		LOG_DEBUG("[STATMSG PhpAgent OnUsrMsg, Send Data to fpm Success, time:%ld(us) for netid:%d, seqid:%d]\n",
						timespan, net_event->id, ntohl(hdr->seq));
		gettimeofday(&tv_begin,NULL);
		uint32_t recvlength = 0;

		ret = recvFromFpm(fd ,buf, &recvlength);
		if(ret != RET_OK)
		{
			if(isUnixSocket == 0){
				LOG_ERROR("[PhpAgent OnUsrMsg] recv From fpm ip=%s port = %d error for netid:%d, seqid:%d",
									fpmip,fpmport, net_event->id, ntohl(hdr->seq));
			}
			else if(isUnixSocket == 1){
				LOG_ERROR("[PhpAgent OnUsrMsg] recv From fpm unixSockPath = %s error for netid:%d, seqid:%d",
								unixSockPath, net_event->id, ntohl(hdr->seq));
			}
			close(fd);
			return RET_FAIL;
		}
		close(fd);
		gettimeofday(&tv_end,NULL);
		timespan = getTimeSpan(tv_begin,tv_end);
		LOG_DEBUG("[STATMSG PhpAgent OnUsrMsg, RecvData from fpm Success, time:%ld(us) for seqid:%d ]\n",
							timespan, ntohl(hdr->seq));
		if(recvlength == 0)
		{
			LOG_WARN("[onUsrMsg]recvFromFpm succ but recvlength is 0 for netid:%d, seqid:%d",
							 net_event->id, ntohl(hdr->seq));
			return ret;
		}
		gettimeofday(&tv_begin,NULL);

		Net_Packet* resp_net_packet = new Net_Packet;
		if((int)(recvlength + sizeof(FCGI_AGENT_HDR)) > resp_net_packet->max_packet_length())
		{
			delete resp_net_packet;
			resp_net_packet = new Net_Packet(recvlength + sizeof(FCGI_AGENT_HDR));
			LOG_WARN("onUsrMsg make big pack for reponse, packlen:%d, standard packlen:%d for seqid:%d",
						(int)(recvlength + sizeof(FCGI_AGENT_HDR)), 
						resp_net_packet->max_packet_length(),
						ntohl(hdr->seq));
		}
		
		hdr->len = htonl(recvlength+sizeof(FCGI_AGENT_HDR));
		memcpy(resp_net_packet->ptr(),hdr,sizeof(FCGI_AGENT_HDR));
		memcpy(resp_net_packet->ptr()+sizeof(FCGI_AGENT_HDR),buf,recvlength);
		resp_net_packet->length(recvlength+sizeof(FCGI_AGENT_HDR));

		LOG_HEX_PREFIX("OnUsrMsg ::send Packet to apipoold:",
						resp_net_packet->ptr(), resp_net_packet->length(), utils::L_DEBUG);

		event_put_mutex.acquire();	
		ret = net_manager->send_packet(net_event->id, resp_net_packet);
		event_put_mutex.release();

		gettimeofday(&tv_end,NULL);
		timespan = getTimeSpan(tv_begin,tv_end);
		LOG_DEBUG("[STATMSG, PhpAgent OnUsrMsg, Send back Packet Success, time:%ld(us), net_id=%d seqid:%d]\n",
						timespan, net_event->id, ntohl(hdr->seq));
		if(ret != RET_OK)
		{		
			delete resp_net_packet;
			LOG_WARN("[onUsrMsg]send packet fail ret=%d net_id=%d for seqid:%d",
							ret, net_event->id, ntohl(hdr->seq));
			return RET_APP_NET_SEND_ERROR;
		}
		gettimeofday(&total_end,NULL);
		timespan = getTimeSpan(total_begin,total_end);
		LOG_INFO("[STATMSG PhpAgent OnUsrMsg,Total time:%ld(us), net_id=%d seqid:%d]\n",
						timespan, net_event->id, ntohl(hdr->seq));
		return ret;
	}
	return ret;
}

