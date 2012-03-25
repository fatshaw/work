#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fifo= mkfifo("./fifo",0600);
	int fd = open("./fifo",O_RDWR);
	if(fd<0)return -1;	
	pid_t pid = fork();
	if(pid < 0)return -1;
	char t[100];
	if(pid == 0){
		_exit(0);
	}
	else{
		while(1)
		{
			write(fd,"asdf",sizeof("asdf"));
			sleep(1);
		}
	}
	unlink("./fifo");
	return 0;
}


