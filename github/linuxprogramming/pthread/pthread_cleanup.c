#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanup(void * argv)
{
	if(argv != NULL){
		free(argv);
		printf("free addr = %x\n",argv);
	}
}

void second(void*args)
{
	printf("%s\n",args);
}

void * pf1(void * args)
{
	volatile int b;
	void * t = malloc(1024);
	printf("malloc addr = %x\n",t);
	pthread_cleanup_push(cleanup,t);
	pthread_cleanup_push(second,"second one");
	while(1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return ((void*)2);
}

typedef struct foo
{	
	int a;
	int b;
}foo;


void * pp(void * args)
{
	foo * f = (foo*)malloc(sizeof(foo));
	printf("f == %x\n",f);
	memset((void*)f,0,sizeof(foo));
	if(args==NULL)
	{
		f->a=10;
		f->b=100;
	}
	printf("f->a=%d,f->b=%d\n",f->a,f->b);
	pthread_exit((void*)f);
}



int main()
{
	foo * f;
	pthread_t tid;
	pthread_create(&tid,NULL,pp,NULL);
	pthread_join(tid,(void*)&f);
	printf("f=%x\n",f);
	printf("f->a= %d,f->b=%d\n",f->a,f->b);
	pthread_create(&tid,NULL,pp,(void*)1);
	pthread_join(tid,NULL);
	printf("f->a= %d,f->b=%d\n",f->a,f->b);
	return 0;
}

