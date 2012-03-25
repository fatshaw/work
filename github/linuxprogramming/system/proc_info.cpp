#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <asm/param.h>
struct proc_info{
        int pid;
        char proc_name[50];
        char state;
        int ppid;
        int pgrp;
        int session;
        int tty;
        int tpgid;
        uint32_t flags;
        uint32_t minflt;
        uint32_t cminflt;
        uint32_t majflt;
        uint32_t cmajflt;
        uint32_t utime;
        uint32_t stime;
        int32_t cutime;
        int32_t cstime;
        int32_t priority;
        int32_t nice;
        int32_t num_threads;
        int32_t itrealvalue;
        uint32_t starttime;
        uint32_t vsize;
        int32_t rss;
        uint32_t rlim;
        uint32_t startcode;
        uint32_t endcode;
        uint32_t startstack;
        uint32_t kstkesp;
        uint32_t kstkeip;
        uint32_t signal;
        uint32_t blocked;
        uint32_t sigignore;
        uint32_t sigcatch;
        uint32_t wchan;
        uint32_t nswap;
        uint32_t cnswap;
        int32_t exit_signal;
        int32_t processor;
        uint32_t rt_priority;
        uint32_t policy;
        uint64_t delayacct_blkio_ticks;
};

int get_proc_info(int pid,proc_info * procInfo)
{
	char propath[100];
	sprintf(propath,"/proc/%d/stat",pid);
	FILE * f = fopen(propath,"r");
	if(f == NULL){
		perror("fopen proc error");
		return -1;
	}
	char buf[1024];
	if(fgets(buf,1024,f)==NULL){
		perror("fgets error");
		return -1;
	}
	if(f != NULL)
		fclose(f);
	
	sscanf(buf,"%d%s %c%d%d%d%d%d%lu%lu%lu%lu%lu%lu%lu%ld%ld%ld%ld%ld%ld%lu%lu%ld%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%d%d%lu%lu%llu",
			&procInfo->pid,procInfo->proc_name,&procInfo->state,&procInfo->ppid,&procInfo->pgrp,&procInfo->session,&procInfo->tty,&procInfo->tpgid,
			&procInfo->flags,&procInfo->minflt,&procInfo->cminflt,&procInfo->majflt,&procInfo->cmajflt,&procInfo->utime,&procInfo->stime,&procInfo->cutime,
			&procInfo->cstime,&procInfo->priority,&procInfo->nice,&procInfo->num_threads,&procInfo->itrealvalue,&procInfo->starttime,
			&procInfo->vsize,&procInfo->rss,&procInfo->rlim,&procInfo->startcode,&procInfo->endcode,&procInfo->startstack,&procInfo->kstkesp,
			&procInfo->kstkeip,&procInfo->signal,&procInfo->blocked,&procInfo->sigignore,&procInfo->sigcatch,&procInfo->wchan,&procInfo->nswap,
			&procInfo->cnswap,&procInfo->exit_signal,&procInfo->processor,&procInfo->rt_priority,&procInfo->policy,
			&procInfo->delayacct_blkio_ticks);
	return 0;
}

double get_proc_cpu_usage(int pid,double timeusage)
{
	static long lastTotal = 0;
	proc_info * procInfo = (proc_info*)malloc(sizeof(proc_info));
	memset(procInfo,0,sizeof(proc_info));
	get_proc_info(pid,procInfo);
	uint32_t total = procInfo->utime+procInfo->stime+procInfo->cutime+procInfo->cstime;
	uint32_t totalSpan = (total - lastTotal);
	lastTotal = total;
	free(procInfo);
	double tickcount = timeusage * HZ;
	printf("timeusage = %f ,total = %lu, tickcount = %f, totalspan = %ld\n",timeusage ,total, tickcount,totalSpan);
	if(timeusage != 0){
		return (double)totalSpan / tickcount;
	}
	else{
		return 0;
	}
}

double getTimeSpan(const timeval & end , const timeval & begin)
{	
	return (end.tv_sec - begin.tv_sec)+ (end.tv_usec -begin.tv_usec)*1e-6;
}

int main(int argc,char ** argv)
{
	int pid = 0;
	if(argc ==1){
		pid = getpid();
	}
	else{
		pid = atoi(argv[1]);
	}
	double cpu_usage = 0;
	struct timeval now;
	struct timeval last;
	double timeusage = 0;
	while(1){
		gettimeofday(&now,NULL);
		timeusage = getTimeSpan(now,last);
		cpu_usage = get_proc_cpu_usage(pid,timeusage) *100;
		printf("cpu usage = %f%%\n",cpu_usage);
		last = now;
		sleep(2);
	}

	return 0;
}
