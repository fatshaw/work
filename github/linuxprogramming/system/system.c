#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <signal.h>
int m_system(char * command)
{
	int64_t a;
	sigset_t newmask,oldmask;
	sigemptyset(&newmask);	
	sigemptyset(&oldmask);
	sigaddset(&newmask,SIGCHLD);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);

	struct sigaction ignore,saveintr,savequit;
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0 ;
	sigaction(SIGINT,&ignore,&saveintr);
	sigaction(SIGQUIT,&ignore,&savequit);
	
	if(command == NULL)return -1;
	pid_t pid = fork();
	if(pid == -1){
		return -1;
	}
	if(pid == 0){
		sigaction(SIGINT,&saveintr,NULL);
		sigaction(SIGQUIT,&savequit,NULL);
		sigprocmask(SIG_SETMASK,&oldmask,NULL);
		if(execl("/bin/sh","sh","-c",command,(char*)0) == -1)
		{
			perror("execl error");
		}
		_exit(127);
	}
	else{
		if(waitpid(pid,NULL,0) != pid){
			return -1;
		}
	}
	sigaction(SIGINT,&saveintr,NULL);
	sigaction(SIGQUIT,&savequit,NULL);
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
}

int fork_test()
{
	pid_t pid = fork();
	if(pid == -1){
		return -1;		
	}
	if(pid == 0)
	{
		pid_t tpid = fork();
		if(tpid == 0)
		{
			printf("second child over\n");
			exit(0);
		}	
		else{		
			waitpid(tpid,NULL,0);
			sleep(2);
			printf("first child over\n");
		}
	}
	else
	{
		int ret= waitpid(pid,NULL,0);
		printf("waitchild ok\n");
	}
	return 0;
}

void sig_alarm(int signo){
	printf("alarm\n");
	kill(getpid(),SIGPIPE);
}
void sig_func(int signo){
	printf("receive signo = %d\n",signo);
}

void sig_chld(int signo)
{
	printf("signal child caught\n");
}

int main(int argc,char ** argv)
{
	signal(SIGCHLD,sig_chld);
	signal(SIGTERM,sig_func);
	signal(SIGINT,sig_func);
	signal(SIGCONT,sig_func);
	signal(SIGALRM,sig_alarm);
	alarm(3);
	while(1);
//	fork_test();
m_system("/bin/ed");	
//	system("/bin/ed");
	exit(0);
	alarm(1);
	while(1);
	pid_t gpid = getpgrp();
	gid_t gid = getgid();
	gid_t egid = getegid();
	pid_t pid = getpid();
	fprintf(stderr,"gpid = %d,gid = %d,egid = %d,pid = %d\n",gpid,gid,egid,pid);
}
