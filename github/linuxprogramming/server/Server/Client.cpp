#include "../header/header.h"
#include "Client.h"

void Client::startRunning(){
	pthread_t threadid;
	int err = pthread_create(&threadid,NULL,Client::recvThread,(void*)this);
	if(err != 0){
		printf("create pthread err: %s\n",strerror(err));
		this->isOver = true;
		this->isConnected = false;
	}
}

int Client::recvData(char * buf,int length){
	int recvSize = recv(client_sock,buf,length,MSG_WAITALL);
	if(recvSize == -1){
		printf("recv error %s\n",strerror(errno));
		return -1;
	}
	else if(recvSize == 0){
		printf("client connect close\n");
		return 0;
	}else if(recvSize > 0){
		return recvSize;
	}
}

void * Client::recvThread(void * argv){
	Client * client = (Client*)argv;
	int recvSize = 0;
	while(client->isConnected){
	 	recvSize = recv(client->client_sock,client->buf,sizeof(header),0);
	 	if(recvSize <=0){break;}
		if(recvSize > 0){
			getHeader(client->buf,&client->hdr);
			printf("recordid is %d\n",client->hdr.recordid);
			if(client->hdr.recordid==END_RECORD_ID){
				send(client->client_sock,client->buf,recvSize,0);
				break;
			}
			if(client->hdr.recordid == PLUSE_RECORD_ID){
				printf("recv pluse record\n");
				client->isAlive = true;
				continue;
			}
			if(client->hdr.length > 0){
				recvSize = recv(client->client_sock,client->buf+sizeof(header),client->hdr.length,0);
				if(recvSize <=0)break;
				printf("recv buf is %s\n",client->buf+sizeof(header));	
				printf("send size is %d\n",send(client->client_sock,client->buf,sizeof(header)+recvSize,0));				
			}
		}
		sleep(1);
	}
	client->isOver = true;
	client->isConnected = false;
}


void Client::sendPluse(){
	const int size= sizeof(header);
	char pluseBuf[size];
	header hdr;
	hdr.recordid= PLUSE_RECORD_ID;
	hdr.length = 0;
	memcpy(pluseBuf,(void*)&hdr,size);
	int sendSize = send(this->client_sock,pluseBuf,size,0);
	printf("send size is %d\n",sendSize);
	if(sendSize == -1){
		printf("send error when send  pluse record: %s\n",strerror(errno));
		this->isOver = true;
		this->isConnected = false;	
	}
}

