#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list
{
	struct list * next;
	pthread_rwlock_t rwlock;
	int count;
}list;

list * head;
list * end;

list l;

void init()
{
	head = NULL;
	end = NULL;
}

void *pf1(void * args)
{
	while(1)
	{
		if(pthread_rwlock_rdlock(&l.rwlock)!=0)return;
		printf("rdlock count = %d\n",l.count);
		pthread_rwlock_unlock(&l.rwlock);
		sleep(1);
	}
}

void *pf2(void * args)
{
	while(1)
	{
		if(pthread_rwlock_wrlock(&l.rwlock)!=0)return;
		l.count++;
		printf("wrlock count = %d\n",l.count);
		pthread_rwlock_unlock(&l.rwlock);
		sleep(2);
	}
}

pthread_mutex_t mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
void enqueue(list * msg)
{
	if(msg == NULL)	{
		msg=(list*)malloc(sizeof(list));
		printf("malloc list,%x\n",msg);
		memset((void*)msg,0,sizeof(msg));
	}
	pthread_mutex_lock(&mutex);
	msg->next = head;
	head = msg;
	if(end == NULL)end = head;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
}

void processMsg(list * msg)
{
	free(msg);
	printf("free msg : %x\n",msg);
}

void processQueue()
{
	for(;;){
		pthread_mutex_lock(&mutex);
		if(head==NULL){
			pthread_cond_wait(&cond,&mutex);
			printf("wait,pthreadid=%lu\n",pthread_self());
		}
		if(head == NULL){
			printf("head == NULL\n");
			pthread_mutex_unlock(&mutex);
			continue;
		}
		list * msg = head;
		head=msg->next;
		pthread_mutex_unlock(&mutex);
		processMsg(msg);
//		sleep(1);
		
	}
}


void * pf3(void * args)
{
	while(1){
		enqueue(NULL);	
//		sleep(5);

	}
}

void * pf4(void * args)
{
	//pthread_detach(pthread_self());
	processQueue();
}



int main()
{
	pthread_t p[10];
	int i =0;
	pthread_t tid1;
	pthread_create(&tid1,NULL,pf3,NULL);		
	for(i=0;i<10;i++){
		pthread_t tid;
		pthread_create(&tid,NULL,pf4,NULL);
		p[i]=tid;
	}
	pthread_join(tid1,NULL);
	return 0;
}

