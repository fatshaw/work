/*
 * forktest.cpp
 *
 *  Created on: Aug 9, 2010
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
char buf[]="hello world\n";
int glob = 6;

void pr_exit(int status){
	if(WIFEXITED(status)){
		printf("nornal termination, exit status is %d \n",WEXITSTATUS(status));
	}
	else if(WIFSIGNALED(status)){
		printf("abnornal termination, signal number = %d%s\n",WTERMSIG(status));
	}
}

int waitProcess()
{
	pid_t pid;
	int status= 0 ;
	if((pid = fork()) < 0){
		perror("fork error\n");
	}else if(pid == 0){
		exit(7);
	}

	if(wait(&status) != pid){
		perror("wait error");
		exit(1);
	}
	pr_exit(status);

	if((pid = fork()) == -1){
		perror("fork error");
	}else if(pid == 0){
		abort();
	}

	if(wait(&status)!=pid){
		perror("wait error");
		exit(1);
	}

	pr_exit(status);
	return 0;
}

int forktest()
{
	int var;
	pid_t pid;

	var = 88;
	if(write(STDOUT_FILENO,buf,sizeof(buf)-1) != sizeof(buf)-1){
		perror("write error");
		exit(1);
	}

	printf("before fork\n");
	if((pid = fork()) == -1){
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		++var;
		glob  = 7;
	}
	else
	{
		sleep(2);
	}
	printf("pid = %d, glob = %d,var = %d\n",getpid(),glob,var);
	return 0;

}

static void charactertime(char * time){
	setbuf(stdout,NULL);
	char * ptr;
	int c;
	for(ptr = time;(c = *ptr++) != 0;){
		putc(c,stdout);
	}
}

void waitChild(pid_t pid)
{
	int status;
	waitpid(pid,&status,0);
}

void waitParent()
{
	while(getppid() != 1)
		sleep(1);
}

int raceTest()
{
	pid_t pid;
	if((pid = fork()) == -1){
		perror("fork error");
		exit(0);
	}
	if(pid == 0){
		//waitParent();
		charactertime("output from child\n");
	}else if(pid > 0){
		waitChild(pid);
		charactertime("output from parent\n");
	}
	return 0;
}

int main(int argc ,char ** argv){
	return raceTest();
}
