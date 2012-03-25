#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TIMEOUT 5
typedef void* (*thrfn)(void *);
struct fip{
	struct timespec ts;
	thrfn fn;
	void * args;
};

int maketimeout(struct timespec * when,long timeout)
{
	if(when == NULL)return -1;
	struct timeval tv;
	struct timespec now;
	gettimeofday(&tv,NULL);
	now.tv_sec = tv.tv_sec;
	now.tv_nsec = tv.tv_usec * 1000;
	when->tv_sec = now.tv_sec + timeout;
	when->tv_nsec = now.tv_nsec;
	return 0;
}


int makethread(thrfn fn,void * args)
{
	pthread_t threadid;
	pthread_create(&threadid,NULL,fn,args);
	return 0;
}

void* threadhelper(void * args)
{
	struct fip * fip = (struct fip *) args;
	nanosleep(&fip->ts,NULL);
	(*fip->fn)(args);
	return 0;
}

int timeout(const struct timespec * when,thrfn fn,void * args)
{
	if(when == NULL)return -1;
	struct timeval tv;
	struct timespec now;
	gettimeofday(&tv,NULL);
	now.tv_sec = tv.tv_sec;
	now.tv_nsec = tv.tv_usec * 1000;

	if(when->tv_sec > now.tv_sec || when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec){
		struct fip * fip =(struct fip *) malloc(sizeof(struct fip));
		memset(fip,0,sizeof(struct fip));
		fip->fn = fn;
		fip->args  = args;
		fip->ts.tv_sec = when->tv_sec - now.tv_sec;
		if(when->tv_nsec >= now.tv_nsec){
			fip->ts.tv_nsec = when->tv_nsec - now.tv_nsec;
		}
		else
		{
			fip->ts.tv_sec--;
			fip->ts.tv_nsec = 1000000000 + when->tv_nsec - now.tv_nsec;
		}
		makethread(threadhelper,(void*)fip);
		return 0;
	}
	
	fn(args);
	return 0;
}

void * fnc(void * args)
{
	pthread_detach(pthread_self());
	printf("helloworld\n");
	return 0;
}

int main()
{
	struct timespec ts;
	maketimeout(&ts,5);
	timeout(&ts,fnc,NULL);
	pause();
	return 0;
}
