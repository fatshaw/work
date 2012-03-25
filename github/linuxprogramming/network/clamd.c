#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int get_unix_socket(const char * socket_name)
{
	int fd = 0;
	struct sockaddr_un address;;
	bzero(&address,sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path,socket_name);
	if((fd = socket(AF_UNIX,SOCK_STREAM,0))< 0){
		perror("socket error");
		return -1;
	}
	if(connect(fd,(struct sockaddr*)&address,sizeof(struct sockaddr_un)) == -1){
		perror("connect error");
		return -1;
	}
	return fd;
}

int scan(int fd,const char * filename)
{
	if(fd == -1 || filename == NULL)return -1;
	const char *  scanart  = "SCAN ";
	char * request = (char*)malloc(1024);
	bzero(request,1024);
	strncpy(request,scanart,strlen(scanart));
	strcat(request,filename);
	if(write(fd,request,strlen(request))!= strlen(request))
	{
		perror("write error");
		free(request);
		return -1;
	}
	printf("client request = %s\n",request);
	free(request);
	
	char * buffer = (char*)malloc(1024);
	bzero(buffer,1024);
	if(read(fd,buffer,1024) > 0)
	{
		printf("clamav response  = %s\n",buffer);
	}	
	free(buffer);
	return 0;
}

int main()
{
	int fd = get_unix_socket("/root/clamd.socket");
	scan(fd,"/root/sambalog/log");
	return 0;
}

