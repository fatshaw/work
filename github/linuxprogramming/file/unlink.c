#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>

int main()
{
	int fd1=open("./a",O_RDWR);
	if(fd1==-1)return;
	unlink("./a");
	char t[1024];
	int n =0;
	while((n=read(fd1,t,sizeof(t)))>0){
		printf("t=%s\n",t);
	}
	if(n==-1)perror("read error");
	printf("n=%d\n",n);
	return 0;
}
