#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LOCKFILE "./lock.file"
#define SHAREFILE "./share.file"
#define FILESIZE 1024*1024*10


#define READ_LOCK F_RDLCK
#define WRITE_LOCK F_WRLCK
#define UNLOCK	F_UNLCK

int lockfile(int fd,short type)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_whence=SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 1;
	return (fcntl(fd,F_SETLK,&lock));
}

int createShareFile()
{
	int fd = open(SHAREFILE,O_CREAT|O_RDWR,0600);
	if(fd < 0)return -1;
	if(lseek(fd,FILESIZE-1,SEEK_SET)<0)return -1;
	if(write(fd,"",1)!=1)return -1;
	if(close(fd)<0)return -1;
	return 0;
}

const char string[]="abcdefhijklmnopqrstuvewyz";

int main()
{
	if(createShareFile()<0)return 1;
	int fd = open(SHAREFILE,O_CREAT|O_RDWR,0600);
	int lockfd=open(LOCKFILE,O_CREAT|O_RDWR,0600);
	if(fd<0||lockfd<0)return 1;
	void * addr = mmap(0,FILESIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	pid_t pid = fork();
	if(pid == -1)return 1;
	if(pid == 0)
	{
		while(1){
			if(lockfile(lockfd,READ_LOCK)==0){
				char line[1024];
				int len = 0;
				while(sscanf(addr+len,"%s",line)!=EOF){
					printf("line = %s\n",line);
					len+=strlen(line)+1;
				}
				lockfile(lockfd,UNLOCK);
			}
			sleep(1);
		}
		close(fd);
		close(lockfd);
	}
	else
	{	srand(time(NULL));
		while(1){
			if(lockfile(lockfd,WRITE_LOCK) == 0){	
				int k =0,t = 0,j,len =0 ;
				char r[11];
				r[10]=0;
				for(k =0 ; k < 10;k++)
				{
					for(j=0;j<10;j++){
						t = rand()%strlen(string);	
						r[j] = string[t];
					}
					sprintf(addr+len,"%s\n",r);
					len+=strlen(r)+1;
				}
				lockfile(lockfd,UNLOCK);
			}
			sleep(1);
		}
		close(fd);
		close(lockfd);
	}	
}
