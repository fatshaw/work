#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

pipe_test()
{

	int fd[2];
	if(pipe(fd) != 0){
		perror("pipe error");
		exit(1);
	}
	pid_t pid = fork();
	if(pid == -1){
		perror("fork error");
		exit(1);
	}
	
	if(pid != 0){
		char t[100];
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		sleep(1);
		read(fd[0],t,sizeof(t));
		printf("t = %s\n",t);
	}
	else{
		sleep(1);
		char t[100];
		bzero(t,100);
		read(fd[0],t,sizeof(t));
		close(fd[0]);
		printf("t = %s\n",t);
		write(fd[1],"world",sizeof("world"));
		sleep(1);
		read(fd[0],t,sizeof(t));
		printf("t = %s\n",t);
		close(fd[1]);
	}
}

int page_test(const char * filename)
{
	int fd[2];
	if(pipe(fd)!=0){
		perror("pipe error");
		return -1;
	}
	pid_t pid = fork();
	if(pid == -1)return -1;
	if(pid == 0){
		close(fd[1]);
		if(fd[0] != STDIN_FILENO){
			if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO){
				perror("dup2 error");
				return -1;
			}
			close(fd[0]);
		}
		execl("/bin/more","more",(char*)0);
	}
	else
	{
		close(fd[0]);		
		char t[1024];
		FILE * f = fopen(filename,"r");
		while(fgets(t,sizeof(t),f)!=NULL)
		{
			if(write(fd[1],t,strlen(t)) != strlen(t)){
				perror("write error");
				return -1;
			}
		}
		if(ferror(f))
		{

			perror("fgets error");
			return -1;
		}

		close(fd[1]);
		if(waitpid(pid,NULL,0)!=pid)
		{
			perror("waitpid error");
			return -1;
		}
	}
	return 0;
	
}

int pfd1[2],pfd2[2];

int tell_wait()
{
	if(pipe(pfd1) == -1 ||pipe(pfd2) == -1){
		perror("pipe error");
		return -1;
	}
}

int tell_parent()
{
	if(write(pfd1[1],"c",1) != 1){
		perror("write error");
		return -1;
	}
	
	return 0;
}

int tell_child()
{
	if(write(pfd2[1],"p",1) != 1){
		perror("write error");
		return -1;
	}
}

int wait_parent()
{
	char c;
	int ret;
	if((ret = read(pfd2[0],&c,1))!=1){
		perror("read error");
		return -1;
	}
	if(c != 'p')return -1;
	return 0;
}

int wait_child()
{
	char c;
	int ret;
	if((ret= read(pfd1[0],&c,1))!=1){
		perror("read error");
		return -1;
	}
	if(c != 'c')return -1;
	return 0;
}


int main(int argc,char ** argv)
{
	tell_wait();
	pid_t pid = fork();
	if(pid == 0)
	{
		wait_parent();
		printf("child working\n");
		tell_parent();
		printf("child finish\n");
	}
	else if(pid > 0)
	{
		printf("parent working\n");
		tell_child();
		wait_child();
		printf("parent finish\n");
	}
	else
	{
		exit(1);
	}
	close(pfd1[1]);
	close(pfd1[0]);
	close(pfd2[0]);
	close(pfd2[1]);
	return 0;
	
}

