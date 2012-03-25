#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LOCKFILE "./lock.file"

int flock(int fd,int start,const char*name)
{
	
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_start=start;
	lock.l_whence=SEEK_SET;
	lock.l_len=1;
	if(fcntl(fd,F_SETLKW,&lock)==-1)
	{
		if(errno==EACCES||errno==EAGAIN)
		{
			perror("cannot fcntl");
			close(fd);
			return -1;
		}
		printf("%s:cannot lock file:%s\n",name,strerror(errno));
		return -1;
	}
	return 0;
}

int main()
{
	int fd = open(LOCKFILE,O_CREAT|O_RDWR,0600);
	if(fd<0){
		printf("open error,%s\n",strerror(errno));
		return 1;
	}
	pid_t pid = fork();
	if(pid == -1){	
		printf("fork error,%s\n",strerror(errno));
		return 1;
	}
	if(pid >0){
		sleep(1);
		flock(fd,1,"parent");
		sleep(1);
		flock(fd,0,"parent");
	}
	else{
		flock(fd,0,"child");		
		sleep(1);
		flock(fd,1,"child");
	}
	return 0;
}
