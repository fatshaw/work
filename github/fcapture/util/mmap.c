#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mmap.h"

char * addr;
int lockfd;
int fd;
char * prefix = "/root/share/";

int lockfile(int fd,short type)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_whence=SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 1;
	return (fcntl(fd,F_SETLK,&lock));
}

int mmapInit()
{
	char filename[100];
	sprintf(filename,"%s%s",prefix,SHARE_FILE);
	fd = open(filename,O_CREAT|O_RDWR|O_TRUNC,0600);
	sprintf(filename,"%s%s",prefix,LOCK_FILE);
	lockfd=open(filename,O_CREAT|O_RDWR,0600);
	if(fd<0||lockfd < 0)return -1;
	lseek(fd,MMAP_LENGTH-1,SEEK_SET);
	write(fd,"",1);
	lseek(fd,0,SEEK_SET);
	addr = NULL;
	addr = (char*)mmap(0,MMAP_LENGTH,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		finalize();
		addr = NULL;
		return -1;
	}
	return 0;
}

int writeMap(const char * data)
{
	if(data == NULL||addr == NULL||lockfd <0)return -1;
	int i = 0;
	while(lockfile(lockfd,WRITE_LOCK) != 0||*addr != 0)
	{
		if(++i>=3)exit(0);
		sleep(WAIT_INTERVAL);
	}
	snprintf(addr,strlen(data)+1,"%s",data);
	lockfile(lockfd,UNLOCK);
	
	return 0;
}

void  finalize()
{
	if(fd >0){
		close(fd);
	}
	if(lockfd>0){
		close(lockfd);
	}
}
