#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	char t[1024];
	char tt[1024];
	int fd1=open("./clamd.log",O_RDWR);
	int fd2=open("./output",O_RDWR|O_CREAT,0600);
	//read(fd1,t,1024);
	//read(fd2,tt,1024);
	struct iovec iovecs[2];
	iovecs[0].iov_base = t;
	iovecs[0].iov_len= 1024;
	iovecs[1].iov_base=tt;
	iovecs[1].iov_len=1024;
	readv(fd1,iovecs,2);
	writev(fd2,iovecs,2);
	close(fd1);
	close(fd2);
	return 0;	
}
