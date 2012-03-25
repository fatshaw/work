#include "header.h"
#define LENGTH 1024
char * buf;

unsigned int recordid = END_RECORD_ID + 1;

void tcp_client()
{
	int retContSize = 0 ;
	int sendSize = 0;
	int recvSize = 0;
	buf = (char*)malloc(LENGTH);
	if(buf == NULL)exit(-1);
	char retBuf[LENGTH];
	int fd = socket(AF_INET,SOCK_STREAM,0);
	
	if(fd == -1){
		sock_err("socket error",fd);	
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(fd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1){
		sock_err("connect error",fd);
	}
	
	string content  = "BEGIN SEND";
	makeBuf(&buf,content,BEGIN_RECORD_ID);
	header * phd = (header*)buf;
	cout<<phd->recordid<<endl;
	cout<<phd->length<<endl;
	sendSize = send(fd,buf,sizeof(header)+phd->length+1,0);
	cout<<"send size is "<<sendSize<<endl;
	sleep(1);
	content = "send record ";
	makeBuf(&buf,content,recordid++);
	sendSize = send(fd,buf,sizeof(header)+phd->length+1,0);
	sleep(1);
	content = "send record 2";
	makeBuf(&buf,content,recordid++);
	sendSize = send(fd,buf,sizeof(header)+phd->length+1,0);
	sleep(1);
	content = "END SEND";
	makeBuf(&buf,content,END_RECORD_ID);
	sendSize = send(fd,buf,sizeof(header)+phd->length+1,0);
	sleep(1);
	while(1){
		int recvSize = recv(fd,buf,sizeof(header),MSG_WAITALL);
		cout<<"receive size is "<<recvSize<<endl;
		header hdr;
		getHeader(buf,&hdr);
		cout<<"recordid is "<<hdr.recordid<<endl;
		cout<<"record length is"<<hdr.length<<endl;
		if(hdr.recordid==END_RECORD_ID){
			close(fd);
			exit(0);	
		}
		recvSize = recv(fd,buf,hdr.length+1,MSG_WAITALL);
		printf("ret buf is %s\n",buf);
	}
}

int main()
{	
	tcp_client();
	return 0;
}

