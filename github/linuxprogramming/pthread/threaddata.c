#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_key_t key1;
pthread_key_t key2;
pthread_once_t flag1 = PTHREAD_ONCE_INIT;

void destroy1(void * args)
{
	if(args){
		printf("destroy1 : %s\n",args);
		free(args);
	}	
}

void destroy2(void * args)
{
	
}

void key_init()
{
	pthread_key_create(&key1,destroy1);
	pthread_key_create(&key2,destroy2);
}


int setThreadData(pthread_key_t key, void * addr)
{
	return pthread_setspecific(key,addr);
}

void * getThreadData(pthread_key_t key)
{
	return pthread_getspecific(key);
}

void * func2(void*args)
{
	void * addr = malloc(1024);
	void * threadData = NULL;
	setThreadData(key1,addr);
	sprintf(addr,"tellmesth\n");
	int i =10;
	while(i--){
		threadData = getThreadData(key1);
		if(threadData == NULL)break;
		printf("%s,%d\n",threadData,i);
		sleep(1);
	}
	
}

void * func1(void * args)
{
	void * addr = malloc(1024);
	void * threadData = NULL;
	int ret = setThreadData(key1,addr);
	if(ret >0)printf("%s\n",strerror(ret));
	sprintf(addr,"holksldfn\n");
	int i = 10 ;
	while(i--){
		threadData = getThreadData(key1);
		if(threadData == NULL)break;
		printf("%s,%d\n",threadData,i);
		sleep(1);
	}
	setThreadData(key1,NULL);
}

int main()
{
	pthread_once(&flag1,key_init);
	pthread_t thread_id1,thread_id2;
	pthread_create(&thread_id1,NULL,func1,NULL);
	pthread_create(&thread_id2,NULL,func2,NULL);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);

	return 0;

	
}
