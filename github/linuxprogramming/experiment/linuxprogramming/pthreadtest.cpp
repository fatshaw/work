#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
struct foo{
	pthread_mutex_t lock;
	int var;
};

void * threadHandler1(void *argv){
	foo * foo_var = static_cast<foo*>(argv);
	//pthread_mutex_lock(&foo_var->lock);
	foo_var->var++;
	//pthread_mutex_unlock(&foo_var->lock);	
}

void * threadHandler2(void *argv){
	foo * foo_var = static_cast<foo*>(argv);
	//pthread_mutex_lock(&foo_var->lock);
	foo_var->var--;
	//pthread_mutex_unlock(&foo_var->lock);	
}


int main(int argc,char ** argv){
	foo * foo_var = (foo*)malloc(sizeof(foo));
	pthread_t pthread_id;
	void * status;
	if(foo_var == NULL){
		printf("malloc error %s\n",strerror(errno));
	}
	memset(foo_var,0,sizeof(foo));
	if(pthread_mutex_init(&foo_var->lock,NULL) != 0){
		free(foo_var);
		return 1;	
	}
	int ret = pthread_create(&pthread_id,NULL,threadHandler1,(void*)foo_var);
	if(ret != 0){
		printf("create thread error %s\n",strerror(ret));	
	}
	
	ret = pthread_create(&pthread_id,NULL,threadHandler2,(void*)foo_var);
	if(ret != 0){
		printf("create thread error %s\n",strerror(ret));	
	}
	sleep(1);
	pthread_mutex_destroy(&foo_var->lock);
	printf("foo var = %d\n",foo_var->var);
	free(foo_var);
	return 0;	
}

