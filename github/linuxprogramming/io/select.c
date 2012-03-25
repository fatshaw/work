#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
int main()
{
	int fd1=open("./a",O_CREAT|O_RDWR,0600);
	int fd[2];
	if(pipe(fd)!=0){
		printf("pipe error,%s\n",strerror(errno));
		return 1;
	}
	pid_t pid =fork();
	if(pid == -1){
		printf("fork error,%s\n",strerror(errno));
		return 1;
	}
	if(pid == 0)
	{
		close(fd[1]);
		close(fd[0]);	
	}
	else if(pid > 0)
	{
		close(fd[0]);
		fd_set wr_set,rd_set;
		while(1)
		{
			sleep(1);
			FD_ZERO(&wr_set);
			FD_ZERO(&rd_set);
			FD_SET(fd[1],&wr_set);
//			FD_SET(fd[0],&rd_set);
			int fnum=select(fd[1]+1,&rd_set,&wr_set,NULL,NULL);
			if(fnum==-1)
			{
				printf("select error,%s\n",strerror(errno));
				return 1;
			}
			else if(fnum == 0)
			{
				printf("no fd is ready\n");
				continue;
			}
			else
			{		
				if(FD_ISSET(fd[1],&wr_set))
				{
					printf("canwrite\n");
					if(write(fd[1],"1",1)!=1)
					{
						printf("write error,%s\n",strerror(errno))	;
						return 1;
					}
				}
				if(FD_ISSET(fd[0],&rd_set))
				{
					printf("canread\n");
					char g='0';
					int ret = read(fd[0],&g,1);
					if(ret == 0)
					{
						printf("no data read\n");
						return 0;
					}
					else if(ret ==-1)
					{
						printf("ret error,%s\n",strerror(errno));
						return 1;
					}
					printf("%c",g);
				}				
			}
		}
	}
}

