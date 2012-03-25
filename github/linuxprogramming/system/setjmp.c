#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
jmp_buf jmpbuf;

void sig_int(int signo)
{
	siglongjmp(jmpbuf,1);
}

	
int main()
{
	umask(0);
	signal(SIGINT,sig_int);
	int fd=open("./a",O_CREAT|O_EXCL|O_RDWR,0666);
	if(sigsetjmp(jmpbuf,1) == 0)
	{
		for(;;){
			int ret = write(fd,"abc",3);
			sleep(1);
			if(ret == -1){
				perror("write error");
				break;
			}
			if(ret == 0){
				printf("nothing written\n");
				break;
			}
		}
	}
	close(fd);
	return 0;
}
