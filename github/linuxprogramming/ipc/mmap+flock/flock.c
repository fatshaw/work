#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LOCKFILE "./lock.file"

int fd;
int already_running()
{
	fd = open(LOCKFILE,O_CREAT|O_TRUNC|O_RDWR,0600);
	if(fd==-1){
		printf("cannot open file,%s\n",strerror(errno));
		return -1;
	}
	char buf[16];
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;
	if(fcntl(fd,F_SETLK,&lock)==-1)
	{
		if(errno==EACCES||errno==EAGAIN)
		{
			perror("cannot fcntl");
			close(fd);
			return -1;
		}
		printf("cannot lock file,%s\n",strerror(errno));
		return -1;
	}
	ftruncate(fd,0);
	sprintf(buf,"%ld",(long)getpid());
	write(fd,buf,strlen(buf)+1);
	return 0;
}

int main()
{
	int f=open("./a",O_CREAT|O_RDWR,0600);
	close(f);
	return 0;
	char t[16];
	sprintf(t,"2");
	if(already_running()==-1){
		printf("lock file error\n");
		return 1;
	}
	while(1){
		write(fd,t,strlen(t));
		sleep(1);
	}
	return 0;
}
