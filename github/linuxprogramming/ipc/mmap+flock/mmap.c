#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

const char string[]="abcdefhijklmnopqrstuvewyz";

int main()
{
		int fd = open("./a.txt",O_CREAT|O_RDWR,0600);
		int lockfd=open("./b.txt",O_CREAT|O_RDWR,0600);
		if(fd<0||lockfd < 0)return 1;
		lseek(fd,1024*1024*10-1,SEEK_SET);
		write(fd,"",1);
		lseek(fd,0,SEEK_SET);
		char * addr = (char*)mmap(0,1024*1024*10,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
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
